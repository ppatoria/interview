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


# Runtime and compile-time polymorphism.

## Virtual Dispatch (Runtime Polymorphism)

```cpp
class Shape {
public:
    virtual double area() const = 0;  // Pure virtual function
};

class Circle : public Shape {
    double radius;
public:
    Circle(double r) : radius(r) {}
    double area() const override { return 3.14159 * radius * radius; }
};

class Square : public Shape {
    double side;
public:
    Square(double s) : side(s) {}
    double area() const override { return side * side; }
};
```

## CRTP (Compile-time Polymorphism)
    CRTP stands for “Curiously Recurring Template Pattern”.
    It is a design pattern in C++ where a class X derives from a class template instantiation using X itself as template argument.

```cpp
template <typename Derived>
class Shape {
public:
    double area() const { return static_cast<const Derived*>(this)->area(); }
};

class Circle : public Shape<Circle> {
    double radius;
public:
    Circle(double r) : radius(r) {}
    double area() const { return 3.14159 * radius * radius; }
};

class Square : public Shape<Square> {
    double side;
public:
    Square(double s) : side(s) {}
    double area() const { return side * side; }
};

int main() {
    using ShapeTypes = std::variant<Circle, Square>;
    std::vector<ShapeTypes> shapes;

    shapes.push_back(Circle(10));
    shapes.push_back(Square(10));

    for(const auto& shape : shapes){
        std::visit( [&](auto& s) { std::cout << s.area() << std::endl; }, shape);
    }

    return 0;
}
```


## Concepts (C++20)

```cpp
template<typename T>
concept Shape = requires(T t) {
    { t.area() } -> std::same_as<double>;
};

class Circle {
    double radius;
public:
    Circle(double r) : radius(r) {}
    double area() const { return 3.14159 * radius * radius; }
};

class Square {
    double side;
public:
    Square(double s) : side(s) {}
    double area() const { return side * side; }
};

int main() {
    using ShapeTypes = std::variant<Circle, Square>;
    std::vector<ShapeTypes> shapes;

    shapes.push_back(Circle(10));
    shapes.push_back(Square(10));

    for(const auto& shape : shapes){
        std::visit( [&](auto& s) { std::cout << s.area() << std::endl; }, shape);
    }

    return 0;
}

```

## Tag Dispatching
    SFINAE stands for “Substitution Failure Is Not An Error”.
    It is a rule in C++ template instantiation, which says that
    *"if a substitution of template parameters fails, the instantiation is removed from the overload resolution set instead of causing a compile error.".*

```cpp
struct Circle { double radius; };
struct Square { double side; };

template <typename Shape>
double area(const Shape& shape);

template <>
double area<Circle>(const Circle& circle) { return 3.14159 * circle.radius * circle.radius; }

template <>
double area<Square>(const Square& square) { return square.side * square.side; }
```

## Expression Templates

Expression templates are a bit more complex and are typically used for building and evaluating mathematical expressions at compile time.
They might be overkill for this simple example.

Here is a simple example of expression templates.
This technique is often used in scientific computing to optimize performance.

```cpp

template<typename E>
class VecExpression {
public:
    double operator const { return static_cast<E const&>(*this)[i]; }
    // ...
};

template<typename E1, typename E2>
class VecSum : public VecExpression<VecSum<E1, E2>> {
    E1 const& _u;
    E2 const& _v;
public:
    VecSum(E1 const& u, E2 const& v) : _u(u), _v(v) {}
    double operator const { return _u[i] + _v[i]; }
    // ...
};

template<typename E1, typename E2>
VecSum<E1,E2> operator+(E1 const& u, E2 const& v) { return VecSum<E1,E2>(u,v); }

```

## Compile-time type-based dispatch

This technique involves using templates and type traits, and it's more advanced than the others.
It might be overkill for this simple example.

Here is a simple example of compile-time type-based dispatch.
This technique uses templates and type traits to dispatch a function to a templated member variable if that variable derives from a given class.


```cpp
template<bool B, class T = void>
struct enable_if {};

template<class T>
struct enable_if<true, T> { typedef T type; };

template<class Base, class T>
struct Derived_from {
    static void constraints(T* p) { Base* pb = p; }
    Derived_from() { void(*p)(T*) = constraints; }
};

template<class T>
struct Container : Derived_from<Container_base, T> {
    // Container-specific code
};

template<class T>
typename enable_if<Is_container<T>::value, void>::type
use(T& x)
{
    // use x as a container
}

template<class T>
typename enable_if<!Is_container<T>::value, void>::type
use(T& x)
{
    // use x as a scalar
}
```

## Overloading

```cpp
struct Circle { double radius; };
struct Square { double side; };

double area(const Circle& circle) { return 3.14159 * circle.radius * circle.radius; }
double area(const Square& square) { return square.side * square.side; }
```

## SFINAE

```cpp
template <typename T, typename = std::enable_if_t<std::is_same_v<T, Circle>>>
double area(const T& circle) { return 3.14159 * circle.radius * circle.radius; }

template <typename T, typename = std::enable_if_t<std::is_same_v<T, Square>>>
double area(const T& square) { return square.side * square.side; }
```

## constexpr If (C++17)

```cpp
template <typename Shape>
double area(const Shape& shape) {
    if constexpr (std::is_same_v<Shape, Circle>) {
        return 3.14159 * shape.radius * shape.radius;
    } else if constexpr (std::is_same_v<Shape, Square>) {
        return shape.side * shape.side;
    }
}
```

## A brief overview of when you might want to use each technique and their advantages:

1. **Virtual Dispatch (Runtime Polymorphism)**:
   Use when you have a hierarchy of related types and you want to write code that can work with any type in the hierarchy.
   The advantage is that it allows for dynamic behavior based on the runtime type of the object.

2. **Concepts (C++20)**:
   Use when you want to specify the requirements on template arguments.
   The advantage is that it can make your templates easier to use and understand,
   and it can provide better error messages when the requirements are not met.

3. **CRTP (Curiously Recurring Template Pattern)**:
   Use when you want to achieve static polymorphism (i.e., behavior that depends on the type, but is resolved at compile time).
   The advantage is that it can provide performance benefits over virtual dispatch by avoiding virtual function calls.

4. **Tag Dispatching**:
   Use when you have several overloaded functions and you want to select one based on the type traits of the arguments.
   The advantage is that it can make your code more modular and easier to extend.

5. **Expression Templates**:
   Use when you're implementing mathematical expressions or similar constructs and you want to avoid unnecessary temporary objects.
   The advantage is that it can provide significant performance benefits.

6. **Compile-time type-based dispatch**:
   Use when you want to select different implementations based on the properties of types at compile time.
   The advantage is that it can make your code more flexible and easier to maintain.

7. **Overloading**:
   Use when you have several functions that do similar things but for different types.
   The advantage is that it can make your code easier to read and write.

8. **SFINAE (Substitution Failure Is Not An Error)**:
   Use when you want to enable or disable certain template instantiations based on the properties of the template arguments.
   The advantage is that it can make your templates more flexible and robust.

9. **constexpr If (C++17)**:
   Use when you want to conditionally compile code based on a constant expression.
   The advantage is that it can make your code more efficient by eliminating unnecessary code at compile time.

## Virtual Dispatch vs CRTP
* **CRTP and Heterogeneous Container**:
   The code using CRTP and `std::variant`/`std::visit` can achieve a similar effect as runtime polymorphism with virtual dispatch.
   However, it's important to note that while `std::variant`/`std::visit` does provide a form of polymorphism, it's not the same as runtime polymorphism with virtual dispatch.
   The `std::visit` call does involve a form of dynamic dispatch, but it's implemented differently from virtual dispatch and has different trade-offs.

* **How variant/visit works**:
   `std::variant` is a type-safe union, which can hold a value of any of the specified types.
   When you call `std::visit` on a `std::variant`, it checks the current active type of the variant and then calls the appropriate function (or "visitor") for that type.
   This is done at runtime, but the set of possible types is fixed at compile time.
   This is why `std::variant`/`std::visit` can provide a form of polymorphism.

* **Advantages and Disadvantages of Virtual Dispatch vs Variant/Visit**:
   - **Virtual Dispatch**:
     The main advantage of virtual dispatch is that it allows for true runtime polymorphism,
     where the set of possible types can be extended by deriving new classes,
     and the decision of which function to call is made at runtime based on the dynamic type of the object.
     The main disadvantage is that it involves a level of indirection (the vtable), which can have a performance cost.
     It also requires heap allocation if you want to store derived objects in a container of base class pointers.

   - **Variant/Visit**:
     The main advantage of `std::variant`/`std::visit` is that it provides a form of polymorphism while avoiding the need for heap allocation or the indirection of a vtable.
     It can be more efficient than virtual dispatch in some cases.
     The main disadvantage is that the set of possible types is fixed at compile time, so you can't extend it by deriving new classes.
     It also involves a form of dynamic dispatch, as `std::visit` needs to check the active type of the variant at runtime.

* **Advantage of CRTP + variant/visit vs runtime polymorphism using virtual function and virtual dispatch**:
   The combination of CRTP and `std::variant`/`std::visit` can provide a form of polymorphism that is more efficient than virtual dispatch in some cases,
   as it avoids the need for heap allocation and the indirection of a vtable.
   It can also take advantage of compile-time polymorphism for some operations, thanks to CRTP.
   However, it doesn't support true runtime polymorphism, as the set of possible types is fixed at compile time.
   So the choice between these techniques depends on the specific requirements of your program.

* **Code Size**:
   Virtual dispatch can potentially result in smaller code size compared to CRTP.
   With CRTP, the compiler generates a separate version of the function for each type that it's used with, which can increase the code size.
   With virtual dispatch, there's only one version of the function in the base class.
