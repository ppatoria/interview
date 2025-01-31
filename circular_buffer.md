# Code
```c++
/** CircularBuffer.h **/
#pragma once

#include <vector>
#include <stdexcept>
#include <sstream>
#include <string>
#include <algorithm>

namespace Common
{

template <typename T>
class CircularBuffer
{
public:
    CircularBuffer(const std::size_t capacity)
    : _data(capacity + 1)
    , _begin(0)
    , _end(0)
    {}

    bool empty() const
    {
        return _begin == _end;
    }

    bool full() const
    {
        return _end == _data.size() - 1 ? _begin == 0: _end + 1 == _begin;
    }

    std::size_t size() const
    {
        return _end >= _begin ? _end - _begin : _data.size() - _begin + _end;
    }

    std::size_t capacity() const
    {
        return _data.size() - 1;
    }

    void push_front(const T& value)
    {
        if (full()) {
            _end = reverse(_end);
        }
        _data[_begin] = value;
        _begin = reverse(_begin);
    }

    void push_back(const T& value)
    {
        if (full()) {
            _begin = advance(_begin);
        }
        _data[_end] = value;
        _end = advance(_end);
    }

    T pop_front()
    {
        return pop_front_ref();
    }

    T& pop_front_ref()
    {
        if (empty()) {
            throw std::out_of_range("Nothing to pop");
        }
        std::size_t pos = _begin;
        _begin = advance(_begin);
        return _data[pos];
    }

    T pop_back()
    {
        if (empty()) {
            throw std::out_of_range("Nothing to pop");
        }
        std::size_t pos = _end;
        _end = reverse(_end);
        return _data[pos];
    }

    T& front()
    {
        if (empty()) {
            throw std::out_of_range("No front element");
        }
        return _data[_begin];
    }

    T& back()
    {
        if (empty()) {
            throw std::out_of_range("No back element");
        }
        return _end > 0 ? _data[_end - 1] : _data[_data.size() - 1];
    }

    const T& get_from_back(const std::size_t idx) const
    {
        std::size_t sz = size();
        if (idx >= sz)
        {
            std::stringstream ss;
            ss << "Index out of range for index " << idx << " and size is " << sz << " also begin is " << _begin << " and end is " << _end;
            std::string s = ss.str();
            throw std::out_of_range(s);
        }

        std::size_t back_idx = _end > 0 ? _end - 1 : _data.size() - 1;
        return back_idx >= idx ? _data[back_idx - idx] : _data[_data.size() - (idx - back_idx)];
    }

private:
    std::size_t advance(const std::size_t idx) const
    {
        return idx < _data.size() - 1 ? idx + 1 : 0;
    }

    std::size_t reverse(const std::size_t idx) const
    {
        return idx > 0 ? idx - 1 : _data.size() - 1;
    }

private:
    std::vector<T>   _data;
    std::size_t      _begin;
    std::size_t      _end;
};

} // namespace

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

```

The **CircularBuffer.cpp** implementation demonstrates several techniques to improve **cache friendliness**, which is crucial for performance, especially in high-throughput or low-latency systems. Here's an analysis of its cache-friendly design and techniques:

---
# Summary:
### **1. Memory-Mapped Files (mmap)**
- **Technique**: The buffer uses memory-mapped files (`mmap`) to map file data directly into the process's address space.
- **Cache Friendliness**:
  - Memory-mapped files allow the operating system to handle caching efficiently. Frequently accessed data is likely to remain in the CPU cache, reducing the need for expensive disk I/O.
  - The use of `msync` with `MS_ASYNC` ensures that changes are written back to the file asynchronously, minimizing disruptions to the cache.

---

### **2. Sequential Memory Access**
- **Technique**: The circular buffer is designed to access data sequentially (e.g., moving from one header to the next using `nextHeader` and `prevHeader`).
- **Cache Friendliness**:
  - Sequential access patterns are highly cache-friendly because modern CPUs prefetch data into the cache when they detect sequential access.
  - This reduces cache misses and improves performance.

---

### **3. Prefetching**
- **Technique**: The code explicitly uses `__builtin_prefetch` to prefetch data into the cache before it is needed.
  ```cpp
  __builtin_prefetch(curFreeHeader);
  __builtin_prefetch(next);
  ```
- **Cache Friendliness**:
  - Prefetching reduces cache misses by loading data into the cache ahead of time, ensuring that the CPU doesn't stall waiting for data.
  - This is particularly useful in spin-wait loops, where the CPU repeatedly checks for a condition (e.g., waiting for a free slot).

---

### **4. Data Locality**
- **Technique**: The buffer stores headers and snapshot data in contiguous memory regions.
- **Cache Friendliness**:
  - Contiguous memory layouts improve spatial locality, meaning that accessing one piece of data is likely to bring nearby data into the cache as well.
  - For example, the `Header` and `ObjectSnapshots` are stored close together, reducing the likelihood of cache misses when accessing related data.

---

### **5. Spin-Wait with `_mm_pause`**
- **Technique**: The spin-wait loop uses `_mm_pause` to reduce CPU contention and improve cache efficiency.
  ```cpp
  _mm_pause(); // a hint to the CPU that it's a spin-wait loop
  ```
- **Cache Friendliness**:
  - `_mm_pause` (or equivalent instructions) reduces the power consumption and contention on the CPU core, allowing other threads or processes to utilize the cache more effectively.
  - This is particularly important in multi-threaded environments where cache lines are shared between cores.

---

### **6. Fixed-Size Headers and Snapshots**
- **Technique**: The buffer uses fixed-size headers (`Header`) and snapshots (`ObjectSnapshots`), with sizes determined by `headerSize` and `bodySize`.
- **Cache Friendliness**:
  - Fixed-size structures allow the CPU to predict memory access patterns more accurately, improving cache utilization.
  - This also reduces fragmentation and ensures that data is aligned in memory, which is beneficial for cache lines.

---

### **7. Avoiding Unnecessary Copies**
- **Technique**: The buffer avoids unnecessary data copies by directly manipulating memory-mapped regions and reusing snapshots.
- **Cache Friendliness**:
  - Avoiding copies reduces memory bandwidth usage and keeps the cache focused on the relevant data, improving overall performance.

---

### **8. Efficient State Management**
- **Technique**: The buffer uses bitwise operations and efficient state transitions (e.g., `isFree`, `isFilled`, `setFilling`, etc.) to manage the state of headers and snapshots.
- **Cache Friendliness**:
  - Compact state representations reduce memory usage and improve cache efficiency by minimizing the amount of data that needs to be loaded into the cache.

---

### **Potential Improvements for Cache Friendliness**
While the implementation is already cache-friendly, some additional optimizations could be considered:
1. **Alignment**: Ensure that `Header` and `ObjectSnapshots` are aligned to cache line boundaries to avoid false sharing in multi-threaded scenarios.
2. **Batch Processing**: Process multiple snapshots in batches to amortize the cost of cache misses.
3. **NUMA Awareness**: If running on NUMA systems, ensure that memory is allocated from the local node to reduce remote memory access latency.

---

### **Conclusion**
The **CircularBuffer.cpp** implementation is highly cache-friendly, leveraging techniques like **sequential memory access**, **prefetching**, **data locality**, and **spin-wait optimizations**. These techniques ensure that the buffer performs well in high-throughput and low-latency scenarios by minimizing cache misses and maximizing CPU cache utilization.

Certainly! Below is a **pseudo-code** representation of the `CircularBuffer.cpp` implementation, broken down into simple, step-by-step logic. This pseudo-code abstracts away low-level details (e.g., memory-mapped files, system calls) and focuses on the core functionality.

---


# Pseudo-Code: Circular Buffer Implementation

#### **1. Initialize the Circular Buffer**
```plaintext
FUNCTION create(fileName, snapCount, headerSize, bodySize):
    IF snapCount == 0 OR headerSize == 0 OR bodySize == 0:
        LOG_ERROR "Invalid parameters"
        RETURN False

    totalSize = snapCount * (sizeof(Header) + headerSize + bodySize)

    OPEN file with fileName
    IF file does not exist:
        CREATE file with size = totalSize
    ELSE IF file size != totalSize:
        LOG_ERROR "File size mismatch"
        RETURN False

    MAP file into memory (memory-mapped file)
    firstHeader = start of memory-mapped region
    lastHeader = end of memory-mapped region

    FOR each header in the buffer:
        IF file is newly created:
            INITIALIZE header with default values
        ELSE:
            CHECK if snapshot is valid
            IF snapshot is invalid:
                RESET snapshot to default values
            ELSE:
                UPDATE header state (e.g., Free, Filled, Persisting)

    SET curFreeHeader = first free header
    SET curFilledHeader = first filled header
    SET curFreeSeqNo = sequence number of last filled header + 1

    LOG "CircularBuffer is ready"
    RETURN True
```

---

#### **2. Destroy the Circular Buffer**
```plaintext
FUNCTION destroy():
    IF memory-mapped region exists:
        UNMAP memory-mapped region
    IF file is open:
        CLOSE file
    RESET all pointers and variables to default values
    LOG "CircularBuffer closed"
```

---

#### **3. Get the Next Free Snapshot**
```plaintext
FUNCTION nextFree(timeout):
    startTime = CURRENT_TIME

    WHILE True:
        IF curFreeHeader is Free:
            BREAK

        nextHeader = NEXT header in the circular buffer
        IF nextHeader is Free:
            curFreeHeader = nextHeader
            BREAK

        IF timeout exceeded:
            RETURN None

        PAUSE CPU (spin-wait loop)

    INCREMENT sequence number
    SET curFreeHeader state to Filling
    RETURN snapshot associated with curFreeHeader
```

---

#### **4. Return a Free Snapshot**
```plaintext
FUNCTION returnFree(snap):
    CLEAR snapshot data
    SET header state to Filled
```

---

#### **5. Get the Next Filled Snapshot**
```plaintext
FUNCTION nextFilled():
    header = curFilledHeader

    WHILE True:
        IF header is not Filled:
            header = NEXT header in the circular buffer
            IF header is not Filled:
                RETURN None
            curFilledHeader = header

        INCREMENT sequence number

        IF snapshot is empty:
            RELEASE snapshot
            CONTINUE

        SET header state to Persisting
        RETURN snapshot
```

---

#### **6. Release a Filled Snapshot**
```plaintext
FUNCTION releaseFilled(snap):
    IF snapshot is not in memory-mapped file:
        RESET snapshot to memory-mapped file
    CLEAR snapshot data
    SET header state to Free
```

---

#### **7. Check if a Snapshot Exists in the Buffer**
```plaintext
FUNCTION hasSnap(snap):
    shiftedPtr = (char*)snap - snapShift
    RETURN shiftedPtr is within the memory-mapped region
```

---

#### **8. Check if the Buffer is Empty**
```plaintext
FUNCTION isEmpty():
    FOR each header in the buffer:
        IF header is not Free:
            RETURN False
    RETURN True
```

---

#### **9. Get the Number of Unprocessed Snapshots**
```plaintext
FUNCTION getUnprocessedCount():
    RETURN (curFreeSeqNo + 1) - curFilledSeqNo
```

---


# Detailed Implementation Steps
    
#### **Step 1: Create the Circular Buffer**
1. Validate input parameters (`snapCount`, `headerSize`, `bodySize`).
2. Calculate the total size of the buffer.
3. Open or create the file. If the file exists, ensure its size matches the expected size.
4. Map the file into memory using `mmap`.
5. Initialize headers and snapshots:
   - For a new file, set all headers to default values.
   - For an existing file, validate and update header states.
6. Set the initial `curFreeHeader`, `curFilledHeader`, and `curFreeSeqNo`.

#### **Step 2: Destroy the Circular Buffer**
1. Unmap the memory-mapped region using `munmap`.
2. Close the file.
3. Reset all pointers and variables to default values.

#### **Step 3: Get the Next Free Snapshot**
1. Spin-wait until a free header is found or the timeout is exceeded.
2. Mark the header as `Filling` and increment the sequence number.
3. Return the associated snapshot.

#### **Step 4: Return a Free Snapshot**
1. Clear the snapshot data.
2. Mark the header as `Filled`.

#### **Step 5: Get the Next Filled Snapshot**
1. Iterate through headers to find the next `Filled` snapshot.
2. If the snapshot is empty, release it and continue searching.
3. Mark the header as `Persisting` and return the snapshot.

#### **Step 6: Release a Filled Snapshot**
1. Reset the snapshot to the memory-mapped file if necessary.
2. Clear the snapshot data.
3. Mark the header as `Free`.

#### **Step 7: Check if a Snapshot Exists in the Buffer**
1. Calculate the pointer to the header.
2. Check if the pointer is within the memory-mapped region.

#### **Step 8: Check if the Buffer is Empty**
1. Iterate through all headers.
2. If any header is not `Free`, return `False`.
3. Otherwise, return `True`.

#### **Step 9: Get the Number of Unprocessed Snapshots**
1. Calculate the difference between `curFreeSeqNo` and `curFilledSeqNo`.
2. Return the result.

---

### **Key Points**
- The circular buffer uses **memory-mapped files** for persistence and efficient memory access.
- It maintains **header states** (Free, Filling, Filled, Persisting) to manage snapshots.
- **Spin-wait loops** and **prefetching** are used for performance optimization.
- The buffer is designed to be **thread-safe** and **cache-friendly**.

This pseudo-code provides a high-level overview of the implementation, making it easier to understand the core logic and flow of the circular buffer.
You're welcome! Let’s walk through the **implementation** of the `CircularBuffer` using **example scenarios** for each key use case. This will help illustrate how the circular buffer works in practice.

---

# Walkthrough:
### **Scenario 1: Creating a New Circular Buffer**
#### **Use Case**: Initialize a new circular buffer with 3 snapshots, header size = 16 bytes, and body size = 64 bytes.
```plaintext
1. Call `create("buffer_file", 3, 16, 64)`.
2. Validate parameters:
   - snapCount = 3 (valid)
   - headerSize = 16 (valid)
   - bodySize = 64 (valid)
3. Calculate total size:
   - itemSize = sizeof(Header) + 16 + 64 = 84 + 16 + 64 = 164 bytes
   - totalSize = 3 * 164 = 492 bytes
4. Create a new file "buffer_file" with size 492 bytes.
5. Map the file into memory.
6. Initialize headers and snapshots:
   - Header 1: State = Free, Sequence Number = 0
   - Header 2: State = Free, Sequence Number = 0
   - Header 3: State = Free, Sequence Number = 0
7. Set:
   - curFreeHeader = Header 1
   - curFilledHeader = Header 1
   - curFreeSeqNo = 1
8. Log: "CircularBuffer[buffer_file] is ready".
```

---

### **Scenario 2: Writing Data to the Buffer**
#### **Use Case**: Write a snapshot to the buffer.
```plaintext
1. Call `nextFree()` to get a free snapshot.
   - curFreeHeader = Header 1 (Free)
   - Set Header 1 state = Filling
   - Increment curFreeSeqNo to 1
   - Return snapshot associated with Header 1
2. Write data to the snapshot:
   - Set snapshot.head = "Header Data 1"
   - Set snapshot.buffer = "Body Data 1"
3. Call `returnFree(snap)` to mark the snapshot as filled.
   - Set Header 1 state = Filled
```

---

### **Scenario 3: Reading Data from the Buffer**
#### **Use Case**: Read the next filled snapshot.
```plaintext
1. Call `nextFilled()` to get the next filled snapshot.
   - curFilledHeader = Header 1 (Filled)
   - Increment curFilledSeqNo to 1
   - Return snapshot associated with Header 1
2. Process the snapshot:
   - Read snapshot.head = "Header Data 1"
   - Read snapshot.buffer = "Body Data 1"
3. Call `releaseFilled(snap)` to mark the snapshot as free.
   - Clear snapshot data
   - Set Header 1 state = Free
```

---

### **Scenario 4: Buffer Full (Overwriting Old Data)**
#### **Use Case**: Write a new snapshot when the buffer is full.
```plaintext
1. Write two more snapshots:
   - Header 2: State = Filled, Sequence Number = 2
   - Header 3: State = Filled, Sequence Number = 3
2. Buffer is now full (all headers are Filled).
3. Call `nextFree()` to get a free snapshot.
   - curFreeHeader = Header 1 (Free)
   - Set Header 1 state = Filling
   - Increment curFreeSeqNo to 4
   - Return snapshot associated with Header 1
4. Write new data to the snapshot:
   - Set snapshot.head = "Header Data 4"
   - Set snapshot.buffer = "Body Data 4"
5. Call `returnFree(snap)` to mark the snapshot as filled.
   - Set Header 1 state = Filled
```

---

### **Scenario 5: Reading All Snapshots**
#### **Use Case**: Read all snapshots in the buffer.
```plaintext
1. Call `nextFilled()` to get the next filled snapshot.
   - curFilledHeader = Header 2 (Filled)
   - Increment curFilledSeqNo to 2
   - Return snapshot associated with Header 2
2. Process the snapshot:
   - Read snapshot.head = "Header Data 2"
   - Read snapshot.buffer = "Body Data 2"
3. Call `releaseFilled(snap)` to mark the snapshot as free.
   - Clear snapshot data
   - Set Header 2 state = Free

4. Repeat for Header 3:
   - curFilledHeader = Header 3 (Filled)
   - Increment curFilledSeqNo to 3
   - Return snapshot associated with Header 3
5. Process the snapshot:
   - Read snapshot.head = "Header Data 3"
   - Read snapshot.buffer = "Body Data 3"
6. Call `releaseFilled(snap)` to mark the snapshot as free.
   - Clear snapshot data
   - Set Header 3 state = Free
```

---

### **Scenario 6: Buffer Empty**
#### **Use Case**: Check if the buffer is empty.
```plaintext
1. Call `isEmpty()`.
2. Iterate through all headers:
   - Header 1: Free
   - Header 2: Free
   - Header 3: Free
3. RETURN True (buffer is empty).
```

---

### **Scenario 7: Buffer Persistence**
#### **Use Case**: Ensure data is persisted to disk.
```plaintext
1. Call `dumpMemFile()`.
2. Use `msync` to asynchronously write changes to the file.
3. Log: "Data synced to file".
```

---

### **Scenario 8: Destroying the Buffer**
#### **Use Case**: Clean up the buffer.
```plaintext
1. Call `destroy()`.
2. Unmap the memory-mapped region.
3. Close the file.
4. Reset all pointers and variables.
5. Log: "CircularBuffer[buffer_file] closed".
```

---

### **Summary of Scenarios**
1. **Create Buffer**: Initialize a new buffer with 3 snapshots.
2. **Write Data**: Write snapshots to the buffer, overwriting old data when full.
3. **Read Data**: Read and process snapshots, marking them as free after processing.
4. **Buffer Full**: Handle overwriting old data when the buffer is full.
5. **Read All Snapshots**: Read and release all snapshots in the buffer.
6. **Buffer Empty**: Check if the buffer is empty.
7. **Persistence**: Sync data to disk.
8. **Destroy Buffer**: Clean up resources.

These scenarios demonstrate how the circular buffer handles common use cases, ensuring efficient data storage, retrieval, and persistence. Let me know if you'd like further clarification!
