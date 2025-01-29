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

