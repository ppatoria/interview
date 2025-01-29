/** CircularBuffer.cpp **/
#include "CircularBuffer.h"

#include <util/log.h>

#include <thread>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

#include <immintrin.h>// for _mm_pause()
#include <string>

namespace persistence
{

CircularBuffer::~CircularBuffer()
{
    destroy();
}

CircularBuffer::Header* CircularBuffer::getHeader(ObjectSnapshots* snap)
{
    auto ptr = reinterpret_cast<char*>(snap) - snapShift;
    return reinterpret_cast<Header*>(ptr);
}

bool CircularBuffer::isSnapInMemFile(ObjectSnapshots* snap) const
{
    return snap->head.size() <= headerSize && snap->buffer.size() <= bodySize;
}

void CircularBuffer::resetSnapToMemFile(ObjectSnapshots* snap) const
{
    auto data = reinterpret_cast<char*>(snap) + sizeof(ObjectSnapshots);
    snap->head.reset(data, headerSize);
    snap->buffer.reset(data + headerSize, bodySize);
}

void CircularBuffer::dumpMemFile()
{
    // With MS_SYNC the call takes around 1ms * size in MB
    msync(firstHeader, lastHeader-firstHeader+itemSize(), MS_ASYNC);
}

size_t CircularBuffer::itemSize() const
{
    return sizeof(Header) + headerSize + bodySize;
}

CircularBuffer::Header* CircularBuffer::prevHeader(Header* hdr) const
{
    if (hdr == firstHeader)
        return lastHeader;

    auto ptr = reinterpret_cast<char*>(hdr);
    ptr -= itemSize();
    return reinterpret_cast<Header*>(ptr);
}

CircularBuffer::Header* CircularBuffer::nextHeader(Header* hdr) const
{
    if (hdr == lastHeader)
        return firstHeader;

    auto ptr = reinterpret_cast<char*>(hdr);
    ptr += itemSize();
    return reinterpret_cast<Header*>(ptr);
}

bool CircularBuffer::create(const std::string& fileName, size_t snapCount,
        size_t hSize, size_t bSize)
{
    return createImpl(fileName, snapCount, hSize, bSize, true);
}

bool CircularBuffer::open(const std::string& fileName, size_t snapCount,
        size_t hSize, size_t bSize)
{
    return createImpl(fileName, snapCount, hSize, bSize, false);
}

bool CircularBuffer::createImpl(const std::string& fileName_, size_t snapCount,
        size_t hSize, size_t bSize, bool createNew)
{
    destroy();

    if (snapCount == 0)
    {
        LOG_ERROR << "CircularBuffer[" << fileName << "] wrong count of snapshots: " << snapCount;
        return false;
    }

    if (hSize == 0 || bSize == 0)
    {
        LOG_ERROR << "CircularBuffer[" << fileName << "] zero header and/or body size of snapshots";
        return false;
    }

    headerSize = hSize;
    bodySize = bSize;

    const auto totalSize = snapCount*itemSize();

    fileName = fileName_;

    memFile = ::open(fileName.c_str(), O_RDWR | (createNew ? O_CREAT : 0), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (memFile < 0)
    {
        LOG_ERROR << "Failed to " << (createNew ? "create" : "open")
                << " CircularBuffer[" << fileName << "]: " << std::strerror(errno);
        return false;
    }

    struct stat stat;
    fstat(memFile, &stat);

    if (stat.st_size == 0)// it is the first attempt to map the file
    {
        if (::ftruncate(memFile, (off_t)totalSize) != 0)
        {
            LOG_ERROR << "Failed to resize CircularBuffer[" << fileName << "] to "
                    << totalSize << " bytes: " << std::strerror(errno);
            destroy();
            return false;
        }

        LOG_LOG << "CircularBuffer[" << fileName << "] new file has been created, size=" << totalSize;
    }
    else if (stat.st_size != (off_t)totalSize)
    {
        LOG_ERROR << "CircularBuffer[" << fileName << "] wrong size, "
                << stat.st_size << " instead of " << totalSize;
        destroy();
        return false;
    }

    auto mmapData = (char*)mmap(0, totalSize, PROT_READ | PROT_WRITE, MAP_SHARED, memFile, 0);
    if (mmapData == MAP_FAILED)
    {
        LOG_ERROR << "CircularBuffer[" << fileName << "] cannot mmap: " << std::strerror(errno);
        destroy();
        return false;
    }

    LOG_LOG << (createNew ? "Created" : "Opened ") << " CircularBuffer[" << fileName << "]. Indexing it...";

    firstHeader = reinterpret_cast<Header*>(mmapData);
    lastHeader  = reinterpret_cast<Header*>(mmapData + totalSize - itemSize());

    auto curHeader = firstHeader;
    auto lastFilledHeader = firstHeader;
    curFreeHeader = nullptr;
    curFilledHeader = nullptr;
    bool prevSnapWasFree = false;
    do
    {
        auto snap = curHeader->snap();

        if (stat.st_size == 0/* || createNew*/)// it is the first attempt to map the file
        {
            new (curHeader) Header(0, snap->sender);
            resetSnapToMemFile(snap);
            snap->clear();
        }
        else// file can have some snaps to persist
        {
            if (!isSnapInMemFile(snap))// never in Free status
            {
                new (curHeader) Header(curHeader->getSeqNo(), snap->sender);
                curHeader->setFilled();
                resetSnapToMemFile(snap);
                snap->clear();
            }
            else
            {
                auto h = snap->head.size();
                auto b = snap->buffer.size();

                new (curHeader) Header(curHeader->getRawState(), snap->sender);
                resetSnapToMemFile(snap);

                snap->head.setSize(h);
                snap->buffer.setSize(b);
            }

            if (curHeader->isFilling())
            {
                curHeader->setFilled();
                snap->clear();
            }
            else if (curHeader->isPersisting())
            {
                curHeader->setFree();
                snap->clear();
            }

            if (curHeader->isFree())
            {
                if (!prevSnapWasFree)
                    curFreeHeader = curHeader;
                prevSnapWasFree = true;
            }
            else// Filled or Persisting
            {
                prevSnapWasFree = false;

                if (!curFilledHeader || curFilledHeader->getSeqNo() > curHeader->getSeqNo())
                {
                    curFilledHeader = curHeader;
                    curFilledSeqNo = curFilledHeader->getSeqNo();
                }

                if (curHeader->getSeqNo() > curFreeSeqNo)
                {
                    curFreeSeqNo = curHeader->getSeqNo();
                    lastFilledHeader = curHeader;
                }
            }
        }

        curHeader = nextHeader(curHeader);
    }
    while (curHeader != firstHeader);

    if (!curFreeHeader)// no Free elements
        curFreeHeader = lastFilledHeader;

    if (!curFilledHeader)// all elements are Free
    {
        curFilledHeader = curFreeHeader;
        curFilledSeqNo = curFilledHeader->getSeqNo() + 1;
    }

    LOG_LOG << "CircularBuffer[" << fileName << "] is ready";
    return true;
}

void CircularBuffer::destroy()
{
    if (firstHeader != nullptr)
        munmap(firstHeader, lastHeader-firstHeader+itemSize());

    if (memFile != -1)
    {
        ::close(memFile);
        memFile = -1;

        LOG_LOG << "CircularBuffer[" << fileName << "] closed";
    }

    firstHeader = nullptr;
    lastHeader = nullptr;
    headerSize = 0;
    bodySize = 0;
}

ObjectSnapshots* CircularBuffer::nextFree(std::chrono::microseconds timeout)
{
    using Clock = std::chrono::steady_clock;

    Clock::time_point startTime;

    for (size_t i = 0;; ++i)
    {
        if (curFreeHeader->isFree())
            break;

        auto next = nextHeader(curFreeHeader);

        if (next->isFree())
        {
            curFreeHeader = next;
            break;
        }

        __builtin_prefetch(curFreeHeader);
        __builtin_prefetch(next);

        if(i == 0) // first run
            startTime = Clock::now();

        if (i % 1024 == 0) // do not bother system too much
            dumpMemFile();

        if(startTime + timeout < Clock::now())
            return nullptr;

        _mm_pause();// a hit to CPU that it's a spin-wait loop
    }

    curFreeHeader->setFilling(++curFreeSeqNo);

    return curFreeHeader->snap();
}

void CircularBuffer::returnFree(ObjectSnapshots* snap)
{
    snap->clear();

    putFilled(snap);
}

void CircularBuffer::putFilled(ObjectSnapshots* snap)
{
    auto header = getHeader(snap);
    header->setFilled();
}

ObjectSnapshots* CircularBuffer::nextFilled()
{
    auto header = curFilledHeader;
    for (;;)
    {
        if (not header->isFilled(curFilledSeqNo))
        {
            header = nextHeader(header);
            if (not header->isFilled(curFilledSeqNo))
            {
                return nullptr;
            }
            curFilledHeader = header;
        }

        ++curFilledSeqNo;

        auto snap = header->snap();
        if (snap->empty())
        {
            releaseFilled(snap);
            continue;
        }

        curFilledHeader->setPersisting();

        return snap;
    }
    return nullptr;
}

void CircularBuffer::releaseFilled(ObjectSnapshots* snap)
{
    if (!isSnapInMemFile(snap))
        resetSnapToMemFile(snap);

    snap->clear();

    getHeader(snap)->setFree();
}

bool CircularBuffer::hasSnap(ObjectSnapshots* snap) const
{
    const auto shiftedToHeaderPtr = (void*)((char*)snap - snapShift);
    return firstHeader <= shiftedToHeaderPtr && shiftedToHeaderPtr <= lastHeader;
}

bool CircularBuffer::isEmpty() const
{
    for(auto header = firstHeader; header < lastHeader; header = nextHeader(header))
    {
        if(!header->isFree())
            return false;
    }

    return lastHeader->isFree();
}

size_t CircularBuffer::getUnprocessedCount() const
{
    // there is no synchronisation between this counters
    // so the result won't be too precise in real multi-threaded case
    auto b = curFilledSeqNo;
    auto e = curFreeSeqNo + 1;
    return e > b ? (e-b) : 0;
}
