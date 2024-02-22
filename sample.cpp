#include <iostream>
#include <memory>

// Custom allocator
template <class T>
struct MyAllocator {
    using value_type = T;

    MyAllocator() noexcept = default;

    template <class U>
    MyAllocator(const MyAllocator<U>&) noexcept {}

    T* allocate(std::size_t n) {
        std::cout << "Custom allocation of " << n << " objects\n";
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, std::size_t n) noexcept {
        std::cout << "Custom deallocation of " << n << " objects\n";
        ::operator delete(p);
    }
};

int main() {
    // Using std::allocate
    {
        std::allocator<int> alloc;
        int* ptr = alloc.allocate(5);  // Allocating memory for 5 ints
        alloc.deallocate(ptr, 5);      // Deallocating the memory
    }

    std::cout << "-----------------\n";

    // Using custom allocator
    {
        MyAllocator<int> alloc;
        int* ptr = alloc.allocate(5);  // Allocating memory for 5 ints using custom allocator
        alloc.deallocate(ptr, 5);      // Deallocating the memory using custom allocator
    }

    std::cout << "-----------------\n";

    // Using std::allocate_shared
    {
        std::allocator<int> alloc;
        auto ptr = std::allocate_shared<int>(alloc, 42);  // Allocating and constructing an int using std::allocate_shared
        std::cout << *ptr << '\n';                        // Accessing the value
    }

    std::cout << "-----------------\n";

    // Using custom allocator with std::allocate_shared
    {
        MyAllocator<int> alloc;
        auto ptr = std::allocate_shared<int>(alloc, 42);  // Allocating and constructing an int using custom allocator with std::allocate_shared
        std::cout << *ptr << '\n';                        // Accessing the value
    }

    return 0;
}
