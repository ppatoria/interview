<style>
/* Solarized Light color theme */
body {
  background-color: #fdf6e3;
  color: #657b83;
  font-family: Arial, sans-serif;
}

h1 {
  color: #268bd2;
  font-size: 22px;
  font-weight: bold;
  margin-top: 30px;
}

h2 {
  color: #2aa198;
  font-size: 18px;
  font-weight: bold;
  margin-top: 20px;
  margin-bottom: 10px;
}

h3 {
  color: #859900;
  font-size: 14px;
  font-weight: bold;
  margin-top: 20px;
  margin-bottom: 10px;
}

p {
  color: #657b83;
}

<!-- code { -->
<!--   background-color: #eee8d5; -->
<!--   color: #657b83; -->
<!--   font-family: Consolas, monospace; -->
<!--   padding: 2px 4px; -->
<!--   border-radius: 4px; -->
<!-- } -->

<!-- pre { -->
<!--   background-color: #f8f8f8; -->
<!--   color: #657b83; -->
<!--   padding: 10px; -->
<!--   border-radius: 4px; -->
<!--   overflow-x: auto; -->
<!-- } -->

<!-- /* Additional styling for FAQ sections */ -->
<!-- h2[id^="scenario"] { -->
<!--   color: #268bd2; -->
<!--   font-weight: bold; -->
<!--   font-size: 18px; -->
<!-- } -->

<!-- /* Table of Contents (TOC) */ -->
<!-- #toc { -->
<!--   position: fixed; -->
<!--   top: 0; -->
<!--   left: 0; -->
<!--   width: 200px; -->
<!--   padding: 10px; -->
<!--   background-color: #eee8d5; -->
<!-- } -->

/* Adjust the content area to make space for TOC */
body {
  margin-left: 220px;
}
</style>
# Shared Pointer
## Ownership:
- `std::shared_ptr` represents *shared ownership* of a dynamically allocated object.
It allows multiple `std::shared_ptr` instances to point to the same object.
- When a `std::shared_ptr` is created, it takes ownership of the object by incrementing the reference count in its control block.
- The reference count keeps track of the number of `std::shared_ptr` instances pointing to the object.
As long as the reference count is greater than zero, the object remains alive.

## Borrowing:
- `std::shared_ptr` allows borrowing or sharing of ownership.
Multiple `std::shared_ptr` instances can be created from an existing `std::shared_ptr` to share ownership of the object.
- When a new `std::shared_ptr` is created from an existing one,
both the original and the new `std::shared_ptr` point to the same object,
and the reference count is incremented.
- Each `std::shared_ptr` instance can access the object and modify its contents if necessary.

## Lifetime:
- The lifetime of the object managed by `std::shared_ptr` is tied to the lifetime of the `std::shared_ptr` instances that own it.
- When the last `std::shared_ptr` owning the object goes out of scope or is explicitly reset, the reference count is decremented.
- If the reference count reaches zero, indicating that no `std::shared_ptr` instances own the object anymore, 
the object is automatically deleted and the memory is freed.



# Control Block

The control block is an internal data structure associated with each `std::shared_ptr` instance.
It contains the necessary information to manage the shared ownership of the object.
The control block typically consists of the following components:

## Reference Count:
- The reference count is an integer value that keeps track of the number of `std::shared_ptr` instances that share ownership of the object.
- Each time a new `std::shared_ptr` is created or an existing one is copied, the reference count is incremented.
- When a `std::shared_ptr` goes out of scope or is reset, the reference count is decremented.
- If the reference count reaches zero, it means that no `std::shared_ptr` instances own the object, and it can be safely deleted.
- **Reference counting is typically implemented using atomic operations to ensure thread safety.**

## Managed Object Pointer:
- The managed object pointer is a raw pointer that points to the dynamically allocated object being managed.
- It allows the `std::shared_ptr` to access and manipulate the object's contents.

## Deleter Function Pointer (optional):
- The deleter function pointer is an optional component that specifies a custom deleter function to be called when the object is deleted.
- It allows customization of the cleanup process, such as releasing additional resources associated with the object.

```
+------------------------+
| Control Block          |
|                        |
| +------------------+   |
| | Reference Count  |   |
| +------------------+   |
| | Managed Object   |   |
| | Pointer          |   |
| +------------------+   |
| | Deleter Function |   |
| | Pointer (optional)|  |
| +------------------+   |
+------------------------+
```

## Multiple `std::shared_ptr` instances can share the same control block.
When a `std::shared_ptr` is created, a control block is also created and associated with it.
Multiple `std::shared_ptr` instances can share the same control block if they are created from the same original `std::shared_ptr` or by copying each other.

## The control block is responsible for deleting the managed object and freeing the associated memory.
The reference count is incremented and decremented as `std::shared_ptr` instances are created, copied, or destroyed.
When the reference count reaches zero, the control block is responsible for deleting the managed object and freeing the associated memory.


# Concurrent Updates to the `shared_ptr` itself (i.e. during reassigning or changing shared ownership) :

## Description
In a multi-threaded environment, if multiple threads concurrently modify the shared ownership of a `std::shared_ptr`, it can lead to race conditions.
The control block of `std::shared_ptr` which includes the reference count is thread-safe, meaning that you can safely create copies of a `shared_ptr` ;
which increments the reference count; from multiple threads.
However, this does not mean that you can update the `shared_ptr` itself (i.e., make it point to a new object) from multiple threads without external synchronization.

## Example
```cpp

    std::shared_ptr<int> ptr = std::make_shared<int>(42);

    // Thread 1
    std::thread t1([&]() {
        ptr = std::make_shared<int>(43);
    });

    // Thread 2
    std::thread t2([&]() {
        ptr = std::make_shared<int>(44);
    });

    t1.join();
    t2.join();
```

## Example:
```cpp

std::shared_ptr<int> sharedPtr = std::make_shared<int>(42);

   // Thread 1
   sharedPtr.reset(new int(24));

   // Thread 2
   sharedPtr.reset(new int(36));
```

## Data races or Memory Leak:
In this scenario, if both threads execute concurrently.
It can result in the
(1) `sharedPtr` being reset to different memory locations, leading to potential memory leaks.
or
(2) accessing invalid memory due to data races, which is an undefined behaviour in C++.

## Solution
To fix this, you would need to protect the assignments with a mutex or other synchronization primitive.

# Accessing a destroyed object while other threads still hold references:
If a `std::shared_ptr` is accessed by multiple threads concurrently, and one thread destroys the object while others are still holding references to it.
It can result in accessing a destroyed object, leading to undefined behaviour.

## Example:
```cpp

   std::shared_ptr<int> sharedPtr = std::make_shared<int>(42);

   // Thread 1
   sharedPtr.reset();

   // Thread 2
   int value = *sharedPtr; // Accessing a destroyed object
```
## Undefined behaviour:
In this case, if Thread 2 tries to access the object pointed to by `sharedPtr` after it has been reset by Thread 1, it will result in undefined behaviour.

## Solution:
To prevent this issue, you need to ensure proper synchronization and lifetime management.
One approach is to use additional synchronization mechanisms, such as mutexes or condition variables.
Coordinate the destruction of the shared object among threads to ensure that no thread is accessing a destroyed object.


# Circular References or Circular dependencies:

    While this is not specific to multithreaded environments, it's a common pitfall with `std::shared_ptr` that can also cause issues in multithreaded code.
    If you have two objects that hold `shared_ptr` to each other, you have a circular reference;
    and the reference count for each object will never reach zero, causing a memory leak.
    A circular dependency occurs when two or more objects hold `std::shared_ptr` references to each other, preventing them from being destroyed.
    This can result in a memory leak because the reference counts of these objects never reach zero.
    To avoid this, you can break the circular dependency by using `std::weak_ptr` or other alternatives.

## Example:
- Order Book Management:
    In a trading platform, the order book contains information about buy and sell orders for various financial instruments.
    The order book may have complex data structures, such as linked lists or trees, where `std::shared_ptr` is used to manage the memory of the nodes.
    If a circular dependency exists between these nodes, it can lead to memory leaks and prevent the proper deallocation of memory.
    This can impact the efficiency and stability of the trading platform.

- Trade Execution and Settlement:
    When executing trades and settling transactions, multiple objects may be involved, such as trade orders, trade execution reports, and settlement records.
    These objects may have interdependencies, and incorrect usage of `std::shared_ptr` can lead to circular dependencies.
    For example,
    a trade order may hold a `std::shared_ptr` reference to a trade execution report, while the trade execution report holds a `std::shared_ptr` reference to the trade order.
    This circular dependency can prevent the proper destruction of these objects and lead to memory leaks.


```cpp
#include <iostream>
#include <memory>

class B; // Forward declaration

class A
{
public:
    std::shared_ptr<B> bSharedPtr; // Declare a shared pointer to B
};

class B
{
public:
    std::shared_ptr<A> aSharedPtr; // Declare a shared pointer to A
};

int main()
{
    std::shared_ptr<A> aSharedPtr = std::make_shared<A>();
    std::shared_ptr<B> bSharedPtr = std::make_shared<B>();

    aSharedPtr->bSharedPtr = bSharedPtr; // Assign the shared pointer to B
    bSharedPtr->aSharedPtr = aSharedPtr; // Assign the shared pointer to A

    return 0;
}
```

    In this example, we have two classes `A` and `B` that have a circular dependency.
    `A` holds a `std::shared_ptr<B>` and `B` holds a `std::shared_ptr<A>`.
    This creates a circular reference, where each object holds a shared pointer to the other.

    The `main()` function creates `std::shared_ptr` instances of `A` and `B`.
    We assign these instances to the respective shared pointers in `A` and `B` using the `shared_ptr` assignment.

## Memory Leak:
    Circular reference using `shared_ptr` can lead to a memory leak.
    The reference count of the objects will never reach zero because each `shared_ptr` keeps the reference count non-zero.
    This prevents the objects from being deallocated, resulting in memory leaks.

## Solution (weak pointer):
    To resolve this issue, you can use `std::weak_ptr` for back-references or secondary references that don't need to keep the object alive.
    A `std::weak_ptr` allows you to access an object if it exists but doesn't increment the reference count.
    This way, when the last `std::shared_ptr` to an object is destroyed, the object will be cleaned up, even if there are still `std::weak_ptr`s pointing to it.

``` cpp

#include <iostream>
#include <memory>

class B; // Forward declaration

class A
{
public:
    std::weak_ptr<B> bWeakPtr; // Declare a weak pointer to B

    void doSomething()
    {
        std::shared_ptr<B> bSharedPtr = bWeakPtr.lock(); // Attempt to lock the weak pointer

        if (bSharedPtr) // Check if the object is still valid
        {
            std::cout << "Doing something with B\n";
        }
        else
        {
            std::cout << "B no longer exists\n";
        }
    }
};

class B
{
public:
    std::weak_ptr<A> aWeakPtr; // Declare a weak pointer to A
};

int main()
{
    std::shared_ptr<A> aSharedPtr = std::make_shared<A>();
    std::shared_ptr<B> bSharedPtr = std::make_shared<B>();

    aSharedPtr->bWeakPtr = bSharedPtr; // Assign the shared pointer to the weak pointer in A
    bSharedPtr->aWeakPtr = aSharedPtr; // Assign the shared pointer to the weak pointer in B

    aSharedPtr->doSomething(); // A tries to access B through the weak pointer

    bSharedPtr.reset(); // Reset the shared pointer to B

    aSharedPtr->doSomething(); // A tries to access B again

    return 0;
}
```

    In this example, we have two classes A and B that have a circular dependency.
    A holds a std::weak_ptr<B> and B holds a std::weak_ptr<A>.
    This weak pointer usage breaks the circular reference, allowing the objects to be deallocated properly.

    In the doSomething() member function of A, we attempt to lock the std::weak_ptr<B> using the lock() function.
    This returns a std::shared_ptr<B> if the object is still valid, or an empty std::shared_ptr otherwise.

    We then check if the std::shared_ptr<B> is valid (not empty).
    If it is, we can safely perform operations with B.
    If it is not, it means that B no longer exists, and we handle that case accordingly.

    In the main() function, we create std::shared_ptr instances of A and B.
    We assign these instances to the respective weak pointers in A and B using the shared_ptr assignment.

    After that, we call the doSomething() function of A, where it attempts to access B through the weak pointer.
    Since B is still valid, the output will be "Doing something with B".

    Then, we reset the shared pointer to B using reset(), which effectively releases ownership of the object.
    After resetting, B is no longer valid.

    Finally, we call doSomething() again, where A attempts to access B.
    Since B no longer exists, the output will be "B no longer exists".

    This example demonstrates how std::weak_ptr can be used to break a circular reference and handle scenarios where objects may have been deleted.
    It allows you to safely access the object through the weak pointer without causing undefined behaviour.

# Incorrect usage with raw pointers:
    If you mix `std::shared_ptr` with raw pointers and accidentally delete the object managed by the shared pointer;
    It can result in a use-after-free error or undefined behaviour.

## Example:
   ```cpp

   int* rawPtr = new int(42);
   std::shared_ptr<int> sharedPtr(rawPtr);

   // ...

   delete rawPtr; // Incorrectly deleting the object

   // Accessing sharedPtr now results in undefined behaviour
   ```

To avoid this, always use `std::shared_ptr` consistently and avoid manually deleting the objects managed by shared pointers.


# Inappropriate use in performance-critical code:
    While `std::shared_ptr` provides convenience and safety, it comes with some performance overhead due to reference counting and thread safety mechanisms.
    In performance-critical code where the overhead of `std::shared_ptr` is a concern, using alternatives like `std::unique_ptr`
    or custom memory management techniques might be more appropriate.

    `std::shared_ptr` provides convenience and safety by managing the lifetime of dynamically allocated objects through reference counting. 
    However, this convenience comes at a cost in terms of both space and time.

## Space Overhead:
   - `std::shared_ptr` requires additional memory to store the reference count and a control block associated with the managed object. 
     The control block contains the reference count, a weak reference count, and any custom deleters or allocators.
   - The reference count keeps track of the number of `shared_ptr` instances that share ownership of the object. 
     This allows the object to be deallocated when the reference count reaches zero.
   - The weak reference count is used by `std::weak_ptr` to determine if the object is still alive or has been deleted.
   - The control block also stores custom deleters or allocators if provided, increasing the memory overhead.

## Time Overhead:
   - Every time a `shared_ptr` is copied or assigned, the reference count is incremented. 
     When a `shared_ptr` goes out of scope or is explicitly reset, the reference count is decremented. 
     This atomic increment and decrement operations can introduce synchronization overhead in multi-threaded scenarios.
   - The reference count must be accessed and modified atomically to ensure thread safety, 
     which adds additional overhead compared to raw pointers or other smart pointers that do not require atomic operations.
   - Additionally, the dynamic memory allocation and deallocation performed by `std::shared_ptr` can introduce performance overhead 
     compared to other smart pointers or raw pointers that do not involve dynamic memory management.

## Comparison with other managed pointers and raw pointers:
###  std::unique_ptr:
   - `std::unique_ptr` is a lightweight smart pointer that manages the lifetime of a single object. 
     It does not have the overhead of reference counting or thread safety mechanisms.
   - `std::unique_ptr` is typically used when exclusive ownership is required, and it provides better performance compared to `std::shared_ptr` in performance-critical code.
   - However, `std::unique_ptr` cannot be easily shared or transferred between different parts of the code, as it enforces exclusive ownership.

### Raw Pointers:
   - Raw pointers do not have the overhead of reference counting or thread safety mechanisms.
   - However, raw pointers do not manage the lifetime of objects automatically.
     It becomes the responsibility of the programmer to manually allocate and deallocate memory,
        which can lead to memory leaks or dangling pointer issues if not handled correctly.
   - Raw pointers also do not provide the safety and convenience features of smart pointers, 
     such as automatic deallocation and exception safety.

# Misuse of custom deleters:
    ``std::shared_ptr` allows you to provide custom deleters to handle the destruction of the managed object.
    If the custom deleter is not implemented correctly or misused, it can result in memory leaks, double deletions, or other undefined behaviour.
    Ensure that custom deleters are implemented properly and handle the destruction of the object correctly.
## Example:
    Suppose we have a `Trade` class that represents a trade executed in the trading platform.
    The `Trade` class has a member variable `tradeData`, which is a dynamically allocated object of type `TradeData`.
    The `TradeData` class contains the actual data related to the trade.

```cpp
class TradeData
{
public:
    TradeData(const std::string& symbol, double price)
    : symbol_(symbol)
    , price_(price) {}
    // ...
private:
    std::string symbol_;
    double price_;
    // ...
};

class Trade
{
public:
    Trade(const std::string& symbol, double price)
    : tradeData_(new TradeData(symbol, price)) {}
    // ...
private:
    std::shared_ptr<TradeData> tradeData_;
    // ...
};
```

    Now, let's look at the potential issues that can arise due to the misuse of `std::shared_ptr` deleters:

### Memory Leak:
    If a custom deleter is not implemented correctly or omitted, it can result in a memory leak.
    Let's say we have a custom deleter that is responsible for freeing the memory of the `TradeData` object.
    However, due to a mistake, we forgot to provide the custom deleter when constructing the `std::shared_ptr`. 

```cpp
class Trade
{
public:
    Trade(const std::string& symbol, double price)
        : tradeData_(new TradeData(symbol, price),
        [](TradeData* data)
        {
            delete data; /* Incorrect deleter */
        }) {}
    // ...
};
```

    In this case, when the `std::shared_ptr` goes out of scope, the default deleter will be used, which is `delete`.
    Since the `TradeData` object was allocated with `new`, but the default `delete` operator is used instead of the custom deleter, it leads to a memory leak.
    The memory allocated for the `TradeData` object is not properly released.

    The deleter function in the `std::shared_ptr` constructor is incorrect.
    In the deleter function, it is generally not recommended to delete the managed object itself.
    Instead, the deleter function should be responsible for cleaning up any dynamically allocated dependencies or resources associated with the object.

    The corrected code for the `std::shared_ptr` constructor with a proper deleter function:

```cpp
tradeData_(new TradeData(symbol, price),
        [](TradeData* data)
        {
            // Delete or reset any dynamically allocated dependencies or resources here
            // For example:
            delete data->dependency;
            // Or:
            data->dependency.reset();

            delete data; // Now it is safe to delete the managed object itself
        })
```

### Undefined Behavior:
    If a custom deleter is misused or incorrectly implemented, it can result in other forms of undefined behaviour.
    For example, consider a scenario where the custom deleter tries to access a member variable of the `Trade` class:

```cpp
class Trade
{
public:
    Trade(const std::string& symbol, double price)
        : tradeData_(new TradeData(symbol, price),
        [](TradeData* data)
        {
            delete data;
            /* Accessing `Trade` object here */
        }) {}
    // ...
};
```

# Issue with initializing `std::shared_ptr` using `new`:
## Exception thrown during the construction (from constructor):
    Initializing a `std::shared_ptr` using `new` can lead to issues if an exception is thrown during the construction of the managed object.

### Example:
      ```cpp

      #include <iostream>
      #include <memory>

      class MyClass
      {
      public:
          MyClass()
          {
              std::cout << "MyClass constructor\n";
              throw std::runtime_error("Exception during construction");
          }

          ~MyClass()
          {
              std::cout << "MyClass destructor\n";
          }
      };

      int main()
      {
          try
          {
              std::shared_ptr<MyClass> ptr(new MyClass());
          }
          catch (const std::exception& e)
          {
              std::cout << "Exception caught: " << e.what() << '\n';
          }

          return 0;
      }
      ```

    In this example, the `MyClass` constructor throws an exception.
      As a result, the destructor is not called, and the memory allocated by `new` is not deallocated.
      This leads to a memory leak.
### Memory Leak:

    If an exception occurs, the object may not be properly deallocated, leading to a memory leak.

### Solution: (std::make_shared)
    using `std::make_shared` guarantees the deallocation of memory if an exception occurs during the construction of the object.

#### Example:
```cpp

#include <iostream>
#include <memory>

class MyClass
{
public:
    MyClass()
    {
        std::cout << "MyClass constructor\n";
        throw std::runtime_error("Exception during construction");
    }

    ~MyClass()
    {
        std::cout << "MyClass destructor\n";
    }
};

int main()
{
    try
    {
        std::shared_ptr<MyClass> ptr = std::make_shared<MyClass>();
    }
    catch (const std::exception& e)
    {
        std::cout << "Exception caught: " << e.what() << '\n';
    }

    return 0;
}
```

    In this example, the constructor of `MyClass` throws an exception. 
    Since `std::make_shared` internally uses `new` to allocate memory and construct the object, 
    the exception thrown during the construction process will propagate out of `std::make_shared`.

    The exception can then be caught and handled in the `try-catch` block. 
    In this case, the output will be:

```
MyClass constructor
Exception caught: Exception during construction
```

    As you can see if the constructor of the object being created with `std::make_shared` throws an exception, 
    the exception is caught in the `try-catch` block surrounding the `std::make_shared` call. 
    This allows you to handle the exception appropriately and prevent it from propagating further.

    It's important to note that when the constructor throws an exception, the memory allocated by `std::make_shared` is automatically deallocated. 
    This ensures proper cleanup and prevents resource leaks.

## Issue when `new` throws an exception:
### Result will be the same for `new` or `std::make_shared` the exception will propagate out.
    When `new` throws an exception during the memory allocation phase, the behaviour of `std::make_shared` depends on the implementation. 
    In most implementations, `std::make_shared` internally uses `new` to allocate memory for both the object and the control block. 
    If `new` throws an exception during this allocation, `std::make_shared` will not have a chance to create the control block and construct the object. 

# Benefit of using `std::make_shared` instead of `new` to initialize a `std::shared_ptr`.
    `std::make_shared` performs a single dynamic memory allocation for both the control block and the managed object.
    This reduces the potential for issues related to exception safety and improves performance.

## Using `new` with `std::shared_ptr`:
    In this case, when using `new` with `std::shared_ptr`, separate memory allocations are performed for the integer object and the control block.

```
Memory Diagram:

        +-----------------+       +-----------------+
ptr1 -->|  shared_ptr<>   |       |       int       |
        +-----------------+       +-----------------+
        | control block   |       |     memory      |
        +-----------------+       +-----------------+

```

    In the above diagram, `ptr1` is a `std::shared_ptr` that manages an `int` object created with `new`. 
    The control block, represented by the shared_ptr<> box, is a separate allocation that stores the reference count and other bookkeeping information. 
    The actual memory for the integer object is allocated separately.

## Using `std::make_shared`:
    When using `std::make_shared`, a single memory allocation is performed to store both the integer object and the control block.

```
Memory Diagram:

        +-----------------+
ptr2 -->|  shared_ptr<>   |
        +-----------------+
        | control block   |
        |    +--------+   |
        |    |  int   |   |
        |    +--------+   |
        +-----------------+

```

    In this diagram, `ptr2` is a `std::shared_ptr` that manages an `int` object created with `std::make_shared`. 
    The control block, represented by the shared_ptr<> box, is allocated together with the memory for the integer object. 
    This combines both the object and control block into a single allocation, improving efficiency and reducing memory overhead.

    It's important to note that the control block stores the reference count and other necessary information for managing the shared ownership of the object. 
## Conclusion:`std::make_shared` reduces the overall memory usage and provides better performance
    By combining the control block and object memory in a single allocation, `std::make_shared` reduces the overall memory usage and provides better performance.
### This memory layout has several advantages:
#### Reduced memory overhead:
    Allocating the control block and the object together reduces memory overhead compared to separate allocations.
    With separate allocations, the control block requires additional memory to store the extra pointer or reference to track the object's memory location,
    resulting in higher memory usage. By combining them, you eliminate the need for separate memory allocations for the control block and the object.

#### Improved cache locality: 
    Having the control block and the object in contiguous memory improves cache locality. 
    Modern processors operate more efficiently when accessing consecutive memory locations. 
    By placing the control block and the object together, you increase the chances of both being in the same cache line, 
    reducing the number of cache misses and improving performance.

#### Fewer system calls:
    With std::make_shared, you perform a single memory allocation for both the control block and the object. 
    This reduces the number of system calls and can improve performance, especially when creating many std::shared_ptr instances.

# Allocators
    Allocators in `std::shared_ptr` allow you to customize the memory allocation strategy for the control block and the object it manages. 
    They are used to allocate and deallocate memory for the control block and the object separately, which can be useful in certain scenarios.
## Example: Separate Control Block and Object Pools.
    One common use case for custom allocators is when you want to allocate the control block and the object from different memory pools or memory regions. 
    This can be beneficial in situations where you have specific memory requirements or want to optimize memory usage.


    In the context of `std::shared_ptr` and custom allocators, the concept is similar to placement new.
    By providing a custom allocator, you can control the memory allocation for both the control block and the object. 
    This allows you to allocate the control block and the object from different memory pools or regions.

    For example, you might have separate memory pools for the control block and the object, each with its own specific characteristics or requirements. 
    By using a custom allocator, you can allocate the control block from one memory pool and the object from another memory pool.

    Here's an example that demonstrates how to use a custom allocator with `std::shared_ptr` to allocate the control block and the object from different memory pools:

```cpp

#include <iostream>
#include <memory>

// Custom allocator for the control block
template <typename T>
struct ControlBlockAllocator
{
    using value_type = T;

    T* allocate(std::size_t n)
    {
        std::cout << "Allocating control block\n";
        return static_cast<T*>(std::malloc(n * sizeof(T)));
    }

    void deallocate(T* p, std::size_t n)
    {
        std::cout << "Deallocating control block\n";
        std::free(p);
    }
};

// Custom allocator for the object
template <typename T>
struct ObjectAllocator
{
    using value_type = T;

    T* allocate(std::size_t n)
    {
        std::cout << "Allocating object\n";
        return static_cast<T*>(std::malloc(n * sizeof(T)));
    }

    void deallocate(T* p, std::size_t n)
    {
        std::cout << "Deallocating object\n";
        std::free(p);
    }
};

int main()
{
    using MyType = int;

    // Create a shared_ptr with custom allocators
    std::shared_ptr<MyType> ptr(
        new (ObjectAllocator<MyType>()) MyType(42),
        ControlBlockAllocator<std::shared_ptr<MyType>>()
    );

    std::cout << *ptr << std::endl;

    return 0;
}
```
    In this example, we define two custom allocators: `ControlBlockAllocator` and `ObjectAllocator`. 
    The `ControlBlockAllocator` allocates memory for the control block, while the `ObjectAllocator` allocates memory for the object.

    When creating the `std::shared_ptr`, we use the custom allocator `ObjectAllocator<MyType>()` as the allocator for the object and 
    `ControlBlockAllocator<std::shared_ptr<MyType>>()` as the allocator for the control block. 
    This ensures that the control block and the object are allocated from different memory pools.

    The output of the example would be:

    ```
    Allocating control block
    Allocating object
    42
    Deallocating object
    Deallocating control block
    ```

    As you can see, the custom allocators are used to allocate and deallocate memory for the control block and the object separately.

    This approach allows you to have more control over the memory allocation for the control block and the object, 
    enabling you to allocate them from different memory pools or regions that may have different characteristics or requirements.


## Example: Memory allocation on stack.
```cpp

#include <iostream>
#include <memory>

template <typename T, std::size_t Size>
struct StackAllocator
{
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = std::size_t;

    T stackMemory[Size];
    std::size_t index = 0;

    template <typename U>
    struct rebind
    {
        using other = StackAllocator<U, Size>;
    };

    StackAllocator() = default;

    template <typename U>
    StackAllocator(const StackAllocator<U, Size>&) noexcept {}

    T* allocate(std::size_t n)
    {
        if (index + n > Size)
        {
            throw std::bad_alloc();
        }
        T* result = stackMemory + index;
        index += n;
        std::cout << "Allocated " << n << " objects on the stack\n";
        return result;
    }

    void deallocate(T* /*ptr*/, std::size_t /*n*/) noexcept {}

    template <typename U, typename... Args>
    void construct(U* ptr, Args&&... args)
    {
        new (ptr) U(std::forward<Args>(args)...);
    }

    template <typename U>
    void destroy(U* ptr)
    {
        ptr->~U();
    }
};

int main()
{
    using MyType = int;
    constexpr std::size_t StackSize = 10;

    std::shared_ptr<MyType> ptr(new (StackAllocator<MyType, StackSize>()) MyType(42));

    std::cout << *ptr << std::endl;

    return 0;
}
#include <iostream>
#include <memory>

struct MyAllocator
{
    using pointer = int*;

    int* allocate(size_t n)
    {
        std::cout << "Custom allocation for " << n << " objects\n";
        return new int[n];
    }

    void deallocate(int* p, size_t n)
    {
        std::cout << "Custom deallocation for " << n << " objects\n";
        delete[] p;
    }
};

int main()
{
    std::shared_ptr<int> ptr(new int(42), MyAllocator());

    return 0;
}
```

    In this example, a custom allocator called `MyAllocator` is defined.
    It provides a custom `allocate` function to allocate memory for the object and a custom `deallocate` function to deallocate the memory.

    When creating a `std::shared_ptr`, you can pass an instance of `MyAllocator` as an argument to the constructor.
    This tells the `std::shared_ptr` to use the custom allocator for allocating and deallocating the memory.

    The output of the above example would be:

    ```
    Custom allocation for 1 object
    ```

    As you can see, the custom allocator is used to allocate memory for the object.
    It allows to customize the memory allocation strategy for the object portion of the std::shared_ptr by providing a custom allocator.

# std::allocate_shared.
`std::allocate_shared<Trade>(tradeAllocator)` and `std::shared_ptr<Trade> ptr(new Trade(), tradeAllocator)`
are two different ways to create a `std::shared_ptr` object with a custom allocator.

## `std::allocate_shared<Trade>(tradeAllocator)`:
   - This function template is provided by the C++ Standard Library and is specifically designed for creating a `std::shared_ptr` object with a custom allocator.
   - It allocates memory for the object using the specified allocator and constructs the object in that memory.
   - It combines the allocation and construction in a single step,
     resulting in potentially better performance and memory utilization compared to separate allocation and construction.
   - It requires the allocator to provide a `std::allocator_traits`-compatible allocator that supports the appropriate allocation and deallocation operations.

## Example of using `std::allocate_shared`:

    ```cpp
    SharedTradePtr trade = std::allocate_shared<Trade>(tradeAllocator);
    ```

## `std::shared_ptr<Trade> ptr(new Trade(), tradeAllocator)`:
   - This is a traditional way to create a `std::shared_ptr` object with a custom allocator,
     using the constructor of `std::shared_ptr` that takes a pointer and an allocator as arguments.
   - It involves manually allocating memory for the object using the specified allocator
     and then constructing the object in that memory using placement new.
   - It separates the allocation and construction steps,
     which may result in slightly less efficient memory utilization compared to `std::allocate_shared`.
   - It provides more flexibility as you can use different allocation strategies or perform any additional setup before constructing the object.

    Here's an example of using the `std::shared_ptr` constructor with a custom allocator:

    ```cpp
   std::shared_ptr<Trade> ptr(new Trade(), tradeAllocator);
   ```

    In this example, we create a `std::shared_ptr` object named `ptr` by manually allocating memory using the `tradeAllocator` and
    constructing a `Trade` object in that memory using placement new.
    The `ptr` object takes ownership of the allocated memory and will handle the deallocation when it goes out of scope.

    To summarize, `std::allocate_shared` is the recommended approach when you want to create a `std::shared_ptr` with a custom allocator directly
    and benefit from the combined allocation and construction.
    However, using the constructor of `std::shared_ptr` with a custom allocator gives you more flexibility
    if you need to perform additional steps during the allocation or construction process.

# Deleters.

    Deleters in `std::shared_ptr` provide a way to customize the cleanup behaviour when the reference count of the `std::shared_ptr` reaches zero. 
    They are used to specify how the resources managed by the `std::shared_ptr` should be released.

    One common use case for custom deleters is when you need to release resources that are not managed by the default destructor of the object. 
    For example, if you have a resource that needs to be explicitly released, such as a file handle or a network connection, 
    you can provide a custom deleter to ensure proper cleanup.

## Example:

```cpp
#include <iostream>
#include <memory>
#include <fstream>

struct FileDeleter
{
    void operator()(std::ofstream* file)
    {
        std::cout << "Custom deleter called\n";
        if (file->is_open())
        {
            file->close();
        }
        delete file;
    }
};

int main()
{
    std::shared_ptr<std::ofstream> filePtr(new std::ofstream("example.txt"), FileDeleter());

    if (filePtr->is_open())
    {
        *filePtr << "Hello, World!";
    }

    return 0;
}
```

    In this example, a custom deleter called `FileDeleter` is defined. 
    It provides a custom `operator()` that is called when the reference count of the `std::shared_ptr` reaches zero.

    When creating a `std::shared_ptr`, you can pass an instance of `FileDeleter` as an argument to the constructor. 
    This tells the `std::shared_ptr` to use the custom deleter for releasing the resources.

    The output of the above example would be:

    ```
    Hello, World!
    Custom deleter called
    ```

    As you can see, the custom deleter is called when the `std::shared_ptr` goes out of scope and the reference count reaches zero. 
    This allows you to perform any necessary cleanup, such as closing a file or releasing other resources.

## Summary:
    Custom allocators and deleters provide additional customization and flexibility when working with `std::shared_ptr`. 
    Allocators allow you to customize the memory allocation strategy, while deleters allow you to specify custom cleanup behaviour. 
    They are useful in scenarios where you have specific memory or resource management requirements that cannot be accomplished with the default constructor or destructor.

