#include <iostream>
#include <algorithm>

// Step 1: Base iterator traits class (mimics std::iterator_traits)
template <typename T>
struct iterator_traits {
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;
};

// Step 2: Define a generic RandomAccessIterator (like STL)
template <typename T>
class RandomAccessIterator {
private:
    T* ptr;  // Pointer to the element

public:
    // Constructor
    explicit RandomAccessIterator(T* p = nullptr) : ptr(p) {}

    // Dereferencing operator
    T& operator*() { return *ptr; }

    // Pointer access operator
    T* operator->() { return ptr; }

    // Pre-increment (++it)
    RandomAccessIterator& operator++() {
        ++ptr;
        return *this;
    }

    // Post-increment (it++)
    RandomAccessIterator operator++(int) {
        RandomAccessIterator temp = *this;
        ++ptr;
        return temp;
    }

    // Pre-decrement (--it)
    RandomAccessIterator& operator--() {
        --ptr;
        return *this;
    }

    // Post-decrement (it--)
    RandomAccessIterator operator--(int) {
        RandomAccessIterator temp = *this;
        --ptr;
        return temp;
    }

    // Addition (it + n)
    RandomAccessIterator operator+(int n) const {
        return RandomAccessIterator(ptr + n);
    }

    // Subtraction (it - n)
    RandomAccessIterator operator-(int n) const {
        return RandomAccessIterator(ptr - n);
    }

    // Difference (it1 - it2)
    std::ptrdiff_t operator-(const RandomAccessIterator& other) const {
        return ptr - other.ptr;
    }

    // Comparison operators
    bool operator==(const RandomAccessIterator& other) const { return ptr == other.ptr; }
    bool operator!=(const RandomAccessIterator& other) const { return ptr != other.ptr; }
    bool operator<(const RandomAccessIterator& other) const { return ptr < other.ptr; }
    bool operator>(const RandomAccessIterator& other) const { return ptr > other.ptr; }
    bool operator<=(const RandomAccessIterator& other) const { return ptr <= other.ptr; }
    bool operator>=(const RandomAccessIterator& other) const { return ptr >= other.ptr; }
};

// Step 3: Custom container that returns STL-style iterators
template <typename T, size_t Size>
class CustomArray {
private:
    T data[Size];  // C-style array
    size_t count;  // Number of elements

public:
    CustomArray() : count(0) {}

    void add(const T& value) {
        if (count < Size) data[count++] = value;
    }

    // Provide STL-compatible iterators
    using iterator = RandomAccessIterator<T>;

    iterator begin() { return iterator(data); }
    iterator end() { return iterator(data + count); }
};

// Step 4: Client code using STL-style iterators
int main() {
    CustomArray<int, 5> arr;
    arr.add(10);
    arr.add(20);
    arr.add(30);

    for (auto it = arr.begin(); it != arr.end(); ++it) {
        std::cout << *it << " ";
    }

    return 0;
}
