# Custom Allocators:
## Interface:
Creating a custom allocator in C++ involves implementing a specific set of functions as required by the C++ Standard Library.
These functions form the interface that your custom allocator must adhere to.
Here's a breakdown of the interface, the responsibilities of each function, and how they should behave.

### Key Interface Functions for a Custom Allocator

1. **`allocate`**
2. **`deallocate`**
3. **`construct`**
4. **`destroy`**
5. **`rebind`**

### 1. `allocate`

#### **Purpose**:  
Allocates raw memory for `n` objects of type `T`.

#### **Parameters**:
- `std::size_t n`: The number of objects (of type `T`) for which memory should be allocated.
  
#### **Expected Implementation**:
- Allocate the raw memory using a system call or memory management mechanism. Typically, this can be done using `::operator new` (for raw memory) or using a custom allocation strategy (e.g., from a memory pool).
- If the allocation fails, throw a `std::bad_alloc` exception (if memory cannot be allocated).
  
#### **Return**:
- Return a pointer to the allocated raw memory (of type `T*`).

#### **Error/Exception Handling**:
- **Throw `std::bad_alloc`** if the allocation fails.
- **Don't catch exceptions** inside the allocator since the standard containers rely on the allocator throwing the exception when allocation fails.
  
#### **Multithreading Considerations**:
- **Thread Safety**: `allocate` should be thread-safe. If you're using a custom memory pool or shared resource, proper synchronization (e.g., mutexes) is necessary to prevent race conditions in multi-threaded environments. If `std::allocator` is used under the hood, it's inherently thread-safe for single allocations but not for pooled allocations.

### 2. `deallocate`

#### **Purpose**:  
Deallocates raw memory previously allocated by `allocate`.

#### **Parameters**:
- `T* ptr`: A pointer to the allocated memory block.
- `std::size_t n`: The number of objects to deallocate.

#### **Expected Implementation**:
- Free the raw memory using the corresponding `::operator delete` or your custom deallocation strategy.
  
#### **Return**:
- This function returns `void` and does not need to return anything.

#### **Error/Exception Handling**:
- **No exception** should be thrown in `deallocate` since `std::allocator` is designed to never throw exceptions in deallocation.
- **Don't attempt to deallocate `nullptr`** (it’s a no-op).
- If you are managing a pool or custom memory management strategy, ensure that `deallocate` properly handles memory cleanup.

#### **Multithreading Considerations**:
- **Thread Safety**: Similar to `allocate`, `deallocate` must be thread-safe when managing shared resources like a memory pool.

### 3. `construct`

#### **Purpose**:  
Constructs an object of type `T` in the allocated memory block.

#### **Parameters**:
- `T* ptr`: Pointer to the memory block where the object should be constructed.
- `Args&&... args`: Forwarded arguments for the constructor of `T`.

#### **Expected Implementation**:
- Call the constructor of `T` using `new (ptr) T(std::forward<Args>(args)...)`. This placement `new` allows you to construct an object in a pre-allocated memory block.
- **No allocation** is performed here; only the object is constructed at the provided memory location.
  
#### **Return**:
- This function returns `void`.

#### **Error/Exception Handling**:
- If the construction fails (e.g., if an exception is thrown from the constructor of `T`), it’s acceptable to propagate the exception.
- **Avoid catching exceptions** within `construct` since it's part of the allocator's responsibility to let the container or user handle construction failure.

#### **Multithreading Considerations**:
- **Thread Safety**: If multiple threads may access the same memory block concurrently, ensure synchronization when constructing objects.

### 4. `destroy`

#### **Purpose**:  
Destroys the object at the specified memory location, but does not deallocate the memory.

#### **Parameters**:
- `T* ptr`: Pointer to the object to destroy.

#### **Expected Implementation**:
- Call the destructor of `T` using `ptr->~T()`.
- **Don't deallocate memory** here—just call the destructor and leave the memory intact for later deallocation.

#### **Return**:
- This function returns `void`.

#### **Error/Exception Handling**:
- **Avoid exceptions**. The destructor should not throw exceptions because `destroy` doesn't guarantee the success of destructor calls.

#### **Multithreading Considerations**:
- **Thread Safety**: Destruction should be thread-safe if multiple threads might be accessing the same memory location. Be careful when destroying objects in multithreaded scenarios, especially if destructors involve shared resources.

### 5. `rebind`

#### **Purpose**:  
Defines a mechanism to allocate memory for a different type than `T`.

#### **Parameters**:
- `U`: The type for which memory should be allocated (different from `T`).

#### **Expected Implementation**:
- The `rebind` struct helps the allocator allocate memory for a different type, typically used in containers when the container stores different types of objects in different internal containers.
- You would define a `rebind` type that points to the allocator for type `U` instead of `T`.

#### **Return**:
- This is a nested type definition (`struct rebind`), not a function returning a value.

#### **Error/Exception Handling**:
- **No error handling** is required in `rebind`. It's simply a type alias used to facilitate allocating memory for a different type.

#### **Multithreading Considerations**:
- **Thread Safety**: `rebind` doesn’t involve memory management or allocation, so no locking or thread safety concerns arise.

---
## Implementation (Example):
### Example Custom Allocator Interface

Here’s a basic structure that summarizes the required interface:

```cpp
template <typename T>
struct MyAllocator {
    using value_type = T;

    // 1. Allocate raw memory
    T* allocate(std::size_t n) {
        if (n == 0) return nullptr;
        void* ptr = ::operator new(n * sizeof(T));  // Allocate raw memory
        if (!ptr) throw std::bad_alloc();  // Throw on allocation failure
        return static_cast<T*>(ptr);
    }

    // 2. Deallocate memory
    void deallocate(T* ptr, std::size_t n) noexcept {
        if (ptr) {
            ::operator delete(ptr);  // Deallocate memory
        }
    }

    // 3. Construct an object in allocated memory
    template <typename U, typename... Args>
    void construct(U* ptr, Args&&... args) {
        new (ptr) U(std::forward<Args>(args)...);  // Placement new to construct
    }

    // 4. Destroy an object
    template <typename U>
    void destroy(U* ptr) noexcept {
        ptr->~U();  // Call destructor
    }

    // 5. Rebind to a different type
    template <typename U>
    struct rebind {
        using other = MyAllocator<U>;
    };
};
```
Here's a simple `main` function that demonstrates how to use the custom allocator (`MyAllocator`) in practice. This will show how it works with a standard container (like `std::vector`) and manually invoking the allocator to allocate and deallocate memory for objects.

### Example `main` Function:

```cpp
#include <iostream>
#include <vector>

template <typename T>
struct MyAllocator {
    using value_type = T;

    // 1. Allocate raw memory
    T* allocate(std::size_t n) {
        if (n == 0) return nullptr;
        void* ptr = ::operator new(n * sizeof(T));  // Allocate raw memory
        if (!ptr) throw std::bad_alloc();  // Throw on allocation failure
        std::cout << "Allocated " << n * sizeof(T) << " bytes." << std::endl;
        return static_cast<T*>(ptr);
    }

    // 2. Deallocate memory
    void deallocate(T* ptr, std::size_t n) noexcept {
        if (ptr) {
            ::operator delete(ptr);  // Deallocate memory
            std::cout << "Deallocated " << n * sizeof(T) << " bytes." << std::endl;
        }
    }

    // 3. Construct an object in allocated memory
    template <typename U, typename... Args>
    void construct(U* ptr, Args&&... args) {
        new (ptr) U(std::forward<Args>(args)...);  // Placement new to construct
    }

    // 4. Destroy an object
    template <typename U>
    void destroy(U* ptr) noexcept {
        ptr->~U();  // Call destructor
    }

    // 5. Rebind to a different type
    template <typename U>
    struct rebind {
        using other = MyAllocator<U>;
    };
};

int main() {
    // Using custom allocator with a standard container (std::vector)
    std::cout << "Using MyAllocator with std::vector" << std::endl;
    std::vector<int, MyAllocator<int>> vec;

    // Add some elements
    for (int i = 0; i < 5; ++i) {
        vec.push_back(i);
    }

    // Display elements
    std::cout << "Vector contents: ";
    for (const auto& val : vec) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Manually using MyAllocator to allocate and deallocate memory
    std::cout << "\nManually allocating and deallocating memory" << std::endl;
    MyAllocator<int> allocator;

    // Allocate memory for 5 integers
    int* ptr = allocator.allocate(5);

    // Construct objects in the allocated memory
    for (int i = 0; i < 5; ++i) {
        allocator.construct(&ptr[i], i * 10);
    }

    // Display constructed objects
    std::cout << "Constructed objects in allocated memory: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << ptr[i] << " ";
    }
    std::cout << std::endl;

    // Destroy the objects
    for (int i = 0; i < 5; ++i) {
        allocator.destroy(&ptr[i]);
    }

    // Deallocate the memory
    allocator.deallocate(ptr, 5);

    return 0;
}
```

### Explanation:

- **Using `std::vector` with Custom Allocator**:
  - The `std::vector<int, MyAllocator<int>>` uses the custom allocator to manage its memory. The allocator handles the memory allocation, construction, and deallocation of objects in the vector.
  - As you add elements to the vector, the `allocate` and `construct` functions are invoked internally, and memory management happens via the custom allocator.

- **Manual Memory Management**:
  - The `allocator.allocate(5)` allocates memory for 5 `int` objects.
  - The `allocator.construct(&ptr[i], i * 10)` constructs `int` objects in the allocated memory, initializing them with values `0, 10, 20, 30, 40`.
  - The `allocator.destroy(&ptr[i])` manually destroys the objects.
  - Finally, `allocator.deallocate(ptr, 5)` deallocates the raw memory.

### Output Example:

```
Using MyAllocator with std::vector
Allocated 20 bytes.
Allocated 20 bytes.
Allocated 20 bytes.
Allocated 20 bytes.
Allocated 20 bytes.
Vector contents: 0 1 2 3 4 

Manually allocating and deallocating memory
Allocated 20 bytes.
Constructed objects in allocated memory: 0 10 20 30 40 
Deallocated 20 bytes.
```

### Key Points:
- **Memory Allocation and Deallocation**: You can see how the custom allocator is being used to manage memory in both the container (`std::vector`) and manually allocated memory.
- **Custom Logic**: You can modify the allocator to add custom behavior, such as logging or managing memory in pools, while still conforming to the C++ Standard's required interface.

### Summary of Error and Exception Handling
- **Allocate/Deallocate**: Always ensure that `allocate` throws `std::bad_alloc` on failure. `deallocate` should never throw exceptions and should safely handle `nullptr`.
- **Construct/Destroy**: Exception handling should be avoided inside `construct` and `destroy`. Let any exceptions from object construction propagate outwards.
- **Thread Safety**: Ensure thread safety where appropriate (e.g., using locks for shared memory management or pools).

By adhering to this interface, you can create a custom allocator that fits the requirements of the C++ Standard Library and is suitable for use with containers like `std::vector`, `std::map`, etc.


# Allocators
    When it comes to memory allocation and access strategies, the choice between
    a managed pointer allocator and a container allocator depends on the
    specific context and requirements of your application.

# Managed Pointer Allocator:
    is typically used when you want to control the memory allocation and
    deallocation of individual objects or resources. It allows you to specify a
    custom allocation strategy for managing the memory of individual objects.
    This can be useful in scenarios where you need fine-grained control over
    memory allocation, such as when dealing with objects of varying sizes or
    when you have specific alignment requirements.

    You might choose to use a managed pointer allocator when:

## Allocate and deallocate individual objects dynamically based on certain conditions or criteria:
    In a trading application, you may have different types of trade objects that
    need to be allocated and deallocated dynamically based on specific
    conditions or criteria. For example, you may need to create trades of
    different types based on the market conditions or trading strategies. Using
    a `std::shared_ptr` allocator, you can dynamically allocate and deallocate
    trade objects as needed, based on the specific criteria.

    Here's a simplified code example to demonstrate dynamic allocation and
    deallocation of trade objects using a `std::shared_ptr` allocator:

    ```cpp
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
    ```


2. **Specific memory alignment requirements**: In a trading application, you may
have specific memory alignment requirements, especially when dealing with
high-performance systems or optimizing cache utilization. For example, you might
require trade objects to be aligned on cache line boundaries to minimize cache
misses during data access. By using a `std::shared_ptr` allocator with a custom
alignment allocator, you can ensure that each trade object is allocated with the
required alignment.

Here's an example demonstrating specific memory alignment for trade objects
using a custom allocator with `std::shared_ptr`:

```cpp
#include <memory>

class Trade {
    // Trade data and member functions
};

template <typename T, std::size_t Alignment>
class AlignedAllocator {
public:
    using value_type = T;

    T* allocate(std::size_t n) {
        void* memory = aligned_alloc(Alignment, n * sizeof(T));
        if (!memory)
            throw std::bad_alloc();
        return static_cast<T*>(memory);
    }

    void deallocate(T* ptr, std::size_t) noexcept {
        free(ptr);
    }
};

using TradeAllocator = AlignedAllocator<Trade, CACHE_LINE_SIZE>;
using SharedTradePtr = std::shared_ptr<Trade>;

int main() {
    TradeAllocator tradeAllocator;

    // Allocate trade objects with specific alignment
    SharedTradePtr trade1 = std::allocate_shared<Trade>(tradeAllocator);
    SharedTradePtr trade2 = std::allocate_shared<Trade>(tradeAllocator);

    // Use the allocated trade objects

    // Deallocate the trade objects when no longer needed
    trade1.reset();
    trade2.reset();

    return 0;
}
```

In this example, we define a custom allocator `AlignedAllocator` that aligns the
memory allocation of trade objects based on the specified `Alignment` template
parameter. We use this aligned allocator with `std::shared_ptr` to allocate
trade objects with the required alignment.

3. **Optimize memory access patterns for individual objects**: In a trading
application, optimizing memory access patterns can be vital for achieving
low-latency performance. By using a `std::shared_ptr` allocator, you can align
individual trade objects in a specific way to improve cache efficiency and
minimize cache misses during data access. For example, you might align trades on
cache line boundaries to ensure that frequently accessed data is stored closer
together, reducing memory latency.

Here's a code example demonstrating optimized memory access patterns for trade
objects using a `std::shared_ptr` allocator:

```cpp
#include <memory>

class Trade {
    // Trade data and member functions
};

template <typename T, std::size_t Alignment>
class CacheOptimizedAllocator {
public:
    using value_type = T;

    T* allocate(std::size_t n) {
        void* memory = aligned_alloc(Alignment, n * sizeof(T));
        if (!memory)
            throw std::bad_alloc();
        return static_cast<T*>(memory);
    }

    void deallocate(T* ptr, std::size_t) noexcept {
        free(ptr);
    }
};

using TradeAllocator = CacheOptimizedAllocator<Trade, CACHE_LINE_SIZE>;
using SharedTradePtr = std::shared_ptr<Trade>;

int main() {
    TradeAllocator tradeAllocator;

    // Allocate trade objects with optimized memory access pattern
    SharedTradePtr trade1 = std::allocate_shared<Trade>(tradeAllocator);
    SharedTradePtr trade2 = std::allocate_shared<Trade>(tradeAllocator);

    // Use the allocated trade objects

    // Deallocate the trade objects when no longer needed
    trade1.reset();
    trade2.reset();

    return 0;
}
```

In this example, we define a custom allocator `CacheOptimizedAllocator` that
aligns the memory allocation of trade objects on cache line boundaries. By
specifying this allocator with `std::shared_ptr`, we can allocate trade objects
with optimized memory access patterns, improving cache efficiency and
potentially reducing latency.

4. **Control memory allocation and deallocation independently from
container-like structures**: In some cases, you may want fine-grained control
over the memory allocation and deallocation process, independently of
container-like structures. By using a `std::shared_ptr` allocator, you can have
precise control over the memory management of individual objects, separate from
the container's memory management.

Here's an example demonstrating independent memory allocation and deallocation
control using a `std::shared_ptr` allocator:

```cpp
#include <memory>

class Trade {
    // Trade data and member functions
};

using TradeAllocator = std::allocator<Trade>;
using SharedTradePtr = std::shared_ptr<Trade>;

int main() {
    TradeAllocator tradeAllocator;

    // Allocate trade objects independently
    Trade* trade1 = tradeAllocator.allocate(1);
    Trade* trade2 = tradeAllocator.allocate(1);

    // Use the allocated trade objects

    // Deallocate trade objects independently
    tradeAllocator.deallocate(trade1, 1);
    tradeAllocator.deallocate(trade2, 1);

    return 0;
}
```

In this example, we use the `std::allocator` as the allocator for trade objects.
We allocate and deallocate individual trade objects independently using the
allocator's `allocate` and `deallocate` functions. This allows us to have full
control over the memory allocation and deallocation process without being tied
to any container-like structures.

# Container Allocator:
    is used when you want to control the memory allocation and deallocation of a
    container object, such as a vector, list, or map, along with its elements.
    It allows you to specify a custom allocation strategy for managing the
    memory of the container and its elements as a whole. This can be useful when
    you want to optimize memory usage and improve performance for container
    operations.

    You might choose to use a container allocator when:

    1. You want to control the memory allocation and deallocation of a container and its elements as a single unit.
    2. You want to optimize memory usage and minimize memory fragmentation for container operations.
    3. You want to apply specific memory allocation strategies, such as using a memory pool or a custom allocator, for the container and its elements.

    In summary, a managed pointer allocator is useful when you want fine-grained control over memory allocation and deallocation for individual objects, 
    while a container allocator is suitable when you want to optimize memory usage and performance for container-based operations as a whole.

    It's important to consider the specific requirements and constraints of your application when choosing between a managed pointer allocator and a container allocator. 
    You may also consider using both in combination, depending on the needs of different components within your application.

















```cpp
#include <iostream>
#include <memory>
#include <cstdlib> // for std::malloc and std::free
#include <concepts> // for std::same_as

template <typename T> concept Allocator = requires(T allocator, std::size_t n) {
typename T::value_type; { allocator.allocate(n) } -> std::same_as<typename
T::value_type*>; { allocator.deallocate(nullptr, n) }; };

template <Allocator Alloc>
struct CustomAllocator {
    using value_type = typename Alloc::value_type;

    CustomAllocator() noexcept {}

    template <typename U>
    CustomAllocator(const CustomAllocator<U>&) noexcept {}

    value_type* allocate(std::size_t n) {
        if (n > std::size_t(-1) / sizeof(value_type)) {
            throw std::bad_alloc();
        }

        if (auto p = static_cast<value_type*>(std::malloc(n * sizeof(value_type)))) {
            return p;
        }

        throw std::bad_alloc();
    }

    void deallocate(value_type* p, std::size_t n) {
        std::free(p);
    }
};

template <Allocator Alloc>
void useCustomAllocator(Alloc& allocator) {
    using T = typename Alloc::value_type;

    T* ptr = allocator.allocate(5);

    for (std::size_t i = 0; i < 5; i++) {
        new (&ptr[i]) T(i);
    }

    for (std::size_t i = 0; i < 5; i++) {
        std::cout << ptr[i] << " ";
        ptr[i].~T();
    }

    allocator.deallocate(ptr, 5);
}

int main() {
    CustomAllocator<std::allocator<int>> customAllocator;
    useCustomAllocator(customAllocator);

    return 0;
}
```
