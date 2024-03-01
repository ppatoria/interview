# Move Semantics:
## Question: Move constructors and move operators should be noexcept ?
### Answer: Yes.
    Move constructors and move operators should be noexcept.
    This is generally a good practice because it allows certain optimizations by the compiler and the standard library, and it provides a strong exception safety guarantee.

```cpp
#include <vector>
#include <iostream>

class MyClass {
public:
    int* data;

    // Regular constructor
    MyClass(size_t size) : data(new int[size]) { }

    // Move constructor
    MyClass(MyClass&& other) noexcept : data(other.data) {
        other.data = nullptr;
    }

    // Destructor
    ~MyClass() {
        delete[] data;
    }

    // Deleted copy constructor and copy assignment operator
    MyClass(const MyClass&) = delete;
    MyClass& operator=(const MyClass&) = delete;
};

int main() {
    std::vector<MyClass> vec;
    vec.reserve(10);  // Reserve space for 10 elements

    try {
        for (int i = 0; i < 10; ++i) {
            vec.push_back(MyClass(1000000));  // Might throw std::bad_alloc
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << '\n';
    }

    return 0;
}
```

In this example, `MyClass` has a move constructor that is `noexcept`.
This allows `std::vector` to use the move constructor when resizing, which is faster than making copies.
If an exception (like `std::bad_alloc`) is thrown during the `push_back` calls, the strong exception safety guarantee ensures that
    - `vector` remains in a valid state.
    - No memory is leaked.
The exception is then caught and handled in the `catch` block.
If an exception was thrown after the strong exception safety guarantee, it would be caught in the same way.

1. **Optimizations**:
   When a move constructor or move assignment operator is declared `noexcept`, it tells the compiler that these operations will not throw an exception.
   This is beneficial because it allows the compiler and the standard library to make certain optimizations.
   For example, when resizing a `std::vector`, if the move constructor is `noexcept`, the standard library can simply move the objects to the new memory location.
   If it's not `noexcept`, the library has to create copies to ensure strong exception safety, which is slower.

2. **Strong Exception Safety Guarantee**:
   This guarantee means that if an exception is thrown during the operation, the program state remains unchanged.
   - **No resources are leaked.**
   - **Any changes made during the operation are rolled back.**
   This is beneficial because it makes your code safer and more robust, as it prevents data corruption and resource leaks in the face of exceptions.

3. **Exception Occurs even after noexcept Guarantee**:
   In C++, if a function is declared `noexcept` and it throws an exception, the C++ runtime will call `std::terminate()` to stop the program.
   This is because throwing an exception from a `noexcept` function is considered a serious programming error.

    The `noexcept` specification is a promise that the function will not throw an exception.
    If it does, it means that this promise has been broken.
    The C++ runtime takes this very seriously and decides that it's better to stop the program than to continue running with broken promises.

```cpp
void myFunction() noexcept {
    throw std::runtime_error("An error occurred");  // This will call std::terminate()
}

int main() {
    try {
        myFunction();
    } catch (...) {
        std::cout << "Caught an exception" << std::endl;  // This code will never be reached
    }

    return 0;
}
```

In this code, `myFunction` is declared `noexcept`, but it throws an exception.
When this happens, the C++ runtime calls `std::terminate()` and the program stops.
The `catch` block in `main` will never be reached.
This is why it's important to ensure that `noexcept` functions really don't throw exceptions.
If there's a chance they might, it's better not to declare them `noexcept`.



