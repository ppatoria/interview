# Virtual over Generic

## Question: Which of the following are common reasons to prefer virtual dispatch over templates:
```
Pick ONE OR MORE options.
1. Wanting to share the implementation code in header files with users of the interface.
2. Wanting to hide the implementation code for faster compilation times.
3. Better code optimisation opportunities for the compiler.
4. Better compilation error messages (pre C++20)
5. Ability to selectively include functions in the API based on the types involved.
```
### Elaboration:

* **Templates:** in C++ allow for compile-time polymorphism.
  This means you can have different implementations of a function for different types,
  and the appropriate implementation is chosen at compile time based on the type of the arguments.
  This is known as function template specialization.
  It allows you to selectively include functions in the API based on the types involved.

* **Virtual dispatch:** is a form of runtime polymorphism and doesn’t provide this ability. With virtual dispatch,
  you define a set of functions in a base class and these can be overridden in derived classes.
  However, the functions that are part of the interface are fixed at compile time and can’t be selectively included based on the types involved at runtime.

## Answer: Reasons to prefer virtual dispatch over templates:

### Correct:
Here are the common reasons to prefer virtual dispatch over templates:

2. **Wanting to hide the implementation code for faster compilation times:**
   Templates require the implementation to be available at compile time, which can lead to longer compilation times and larger binary sizes.
   Virtual dispatch allows you to hide the implementation details in a source file, potentially leading to faster compilation times.

4. **Better compilation error messages (pre C++20):**
   Templates can sometimes produce complex and difficult-to-understand error messages, especially when template metaprogramming is involved.
   Virtual dispatch can often lead to clearer error messages.

### Incorrect:
1. **Wanting to share the implementation code in header files with users of the interface.**
   Option is not a correct reason because it's actually a characteristic of templates, not virtual dispatch.

3. **Better code optimisation opportunities for the compiler.**
   Templates in C++ are resolved at compile time, allowing the compiler to generate specific code for each type that is used as a template argument.
   This can lead to more efficient code because the compiler can optimize the code for each specific type.

5. **Ability to selectively include functions in the API based on the types involved.**
   The ability to selectively include functions in the API based on the types involved can be a reason to prefer templates over virtual dispatch in C++ (not the opposite).

    Templates in C++ allow for compile-time polymorphism, which means the compiler generates specific code for each type that is used as a template argument.
    This allows you to write generic code that works with any data type, and you can selectively include or exclude certain functions based on the types involved.

    For example, you could define a template class with a function that only exists for certain types.
    If a type does not meet the requirements for that function, the compiler simply won't generate the function for that type.
    This can be particularly useful in API design where you might want to expose different functionalities for different types.

    ```c++

   #include <iostream>
   #include <concepts>

    // Define a concept for printable types
    template<typename T>
    concept Printable = requires(T t) {
        { std::cout << t } -> std::same_as<std::ostream&>;
    };

    // Our Printer class
    template <Printable T>
    class Printer {
    public:
        void print(const T& data) {
            std::cout << data << std::endl;
        }
    };

    int main() {
        Printer<int> intPrinter;
        intPrinter.print(123);  // This will work

        /** This will not compile 
        class test{};
        test t;
        Printer<test> testPrinter;  
        testPrinter.print(t);
        */
        return 0;
    }

    ```
    In this example, the print function is included in the Printer class for any type T that can be printed.
    If T cannot be printed (for example, if T is a type of object that doesn’t support the << operator with std::cout),
    then trying to use the print function with a Printer<T> object will result in a compile error.
    This is a simple way to selectively include functions in the API based on the types involved.


When you use templates in C++, the implementation of the template must be available to the compiler when it's instantiated.
This is typically achieved by placing the implementation in the header file.
Therefore, using templates often implies sharing the implementation code in header files with users of the interface.

On the other hand, virtual dispatch in C++ is a feature of polymorphism and is typically used with classes and inheritance.
The implementation of a virtual function can be provided in a source (.cpp) file, separate from the header file.
This allows for hiding the implementation details from the users of the interface,
which aligns with the principle of encapsulation in object-oriented programming.

So, if you want to share the implementation code in header files with users of the interface,
you would typically use templates, not virtual dispatch.
Hence, this option  is not a correct reason to prefer virtual dispatch over templates.
