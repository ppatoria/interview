# Demystifying Polymorphism in C++: A Comprehensive Guide

Introduction
Polymorphism is a fundamental concept in object-oriented programming (OOP) that allows objects of different types to be treated uniformly. In C++, polymorphism can be achieved through various techniques, each offering unique advantages and use cases. This article aims to demystify polymorphism in C++ by providing a comprehensive guide to its different forms, including virtual dispatch, CRTP, concepts (introduced in C++20), tag dispatching, expression templates, compile-time type-based dispatching, overloading, SFINAE, and constexpr if (introduced in C++17). By understanding the strengths and trade-offs of each technique, developers can make informed decisions when designing and implementing polymorphic behavior in their C++ programs.

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

## CRTP (Curiously Recurring Template Pattern)
CRTP stands for “Curiously Recurring Template Pattern”. It is a design pattern in C++ where a class X derives from a class template instantiation using X itself as a template argument.

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
        std::visit( [&](const auto& s) { std::cout << s.area() << std::endl; }, shape);
    }

    return 0;
}

```


## Tag Dispatching
1. Example:
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
2. Example: Let’s consider a scenario where we have a collection of different geometric shapes and we want to perform different operations on them based on their type.
For instance, we might want to calculate the area of a circle, the perimeter of a square, and the volume of a cube.

In this case, we can use tag dispatching to select the appropriate function for each shape. Here’s an example:
```c++
#include <iostream>
#include <vector>

struct CircleTag {};
struct SquareTag {};
struct CubeTag {};

class Shape {
public:
    virtual void calculate() const = 0;
};

template <typename Tag>
class ShapeImpl : public Shape {
public:
    void calculate() const override {
        calculateImpl(Tag());
    }
private:
    void calculateImpl(CircleTag) const {
        std::cout << "Calculating area of circle\n";
    }
    void calculateImpl(SquareTag) const {
        std::cout << "Calculating perimeter of square\n";
    }
    void calculateImpl(CubeTag) const {
        std::cout << "Calculating volume of cube\n";
    }
};

using Circle = ShapeImpl<CircleTag>;
using Square = ShapeImpl<SquareTag>;
using Cube = ShapeImpl<CubeTag>;

int main() {
    std::vector<Shape*> shapes;
    shapes.push_back(new Circle);
    shapes.push_back(new Square);
    shapes.push_back(new Cube);

    for (const auto& shape : shapes) {
        shape->calculate();
    }

    for (const auto& shape : shapes) {
        delete shape;
    }

    return 0;
}

```
In this code, ShapeImpl is a template class that inherits from Shape and takes a Tag as a template parameter.
The calculate function in ShapeImpl calls calculateImpl with an instance of Tag, which dispatches to the correct calculateImpl overload based on the type of Tag.
This allows us to perform different calculations for each shape type without having to modify the Shape interface or the shape classes themselves.

This is just one example of how tag dispatching can be used.
The technique is very flexible and can be adapted to many different scenarios.
It’s particularly useful in generic programming, where you often need to customize behavior based on type traits.

    - **Advantages over Virtual Dispatch**:
    The primary advantage of tag dispatching over virtual dispatch is that it allows you to customize behavior based on type traits without modifying the classes themselves.
    This can be particularly useful in generic programming, where you often need to customize behavior based on type traits.
    With virtual dispatch, you would need to add a new virtual function to the base class for each new operation, which can lead to a bloated interface and violate the Open-Closed Principle (classes should be open for extension but closed for modification).
    Additionally, tag dispatching can be more efficient than virtual dispatch because it avoids the overhead of a virtual function call (vtable lookup and indirection).
    However, the objects still need to be created on the heap and the dispatch is still dynamic.

    - **Advantages over CRTP**:
    The Curiously Recurring Template Pattern (CRTP) is a powerful technique for static polymorphism, where the derived class is passed as a template parameter to the base class.
    This allows the base class to call methods on the derived class at compile time, avoiding the overhead of virtual function calls.
    However, CRTP has some limitations.
    For example, it doesn't work well with `std::variant` or `std::visit`, because these require objects of the same type, whereas each instantiation of a CRTP base class is a different type.
    Tag dispatching doesn't have this limitation, because the tag types are separate from the class types.
    This allows you to use tag dispatching with `std::variant` and `std::visit`, as shown in the example.
    Also, with CRTP, each class needs to implement all the methods, whereas with tag dispatching, you can have default implementations in the base class.

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
    SFINAE stands for “Substitution Failure Is Not An Error”.
    It is a rule in C++ template instantiation, which says that
    *"if a substitution of template parameters fails, the instantiation is removed from the overload resolution set instead of causing a compile error.".*

```cpp
template <typename T, typename = std::enable_if_t<std::is_same_v<T, Circle>>>
double area(const T& circle) { return 3.14159 * circle.radius * circle.radius; }

template <typename T, typename = std::enable_if_t<std::is_same_v<T, Square>>>
double area(const T& square) { return square.side * square.side; }
```
or

```c++
#include <iostream>
#include <type_traits>
#include <vector>
#include <variant>

class Circle;
class Square;

template <typename T>
class Shape {
public:
    template <typename U = T>
    typename std::enable_if<std::is_same<U, Circle>::value, double>::type
    area() const { return static_cast<const T*>(this)->area(); }

    template <typename U = T>
    typename std::enable_if<std::is_same<U, Square>::value, double>::type
    area() const { return static_cast<const T*>(this)->area(); }
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
        std::visit( [&] (const auto& s){ std::cout << s.area() << std::endl; }, shape);
    }

    return 0;
}
```
3. Example: SFINAE is a principle in C++ template metaprogramming that is used to control the overload resolution among different function templates.
   A common use case for SFINAE is to create a function template that behaves differently based on the type of its arguments.
   For example, let’s say we want to create a print function that prints the elements of a container if the argument is a container, and simply prints the argument itself otherwise.
```c++
#include <iostream>
#include <type_traits>
#include <vector>

// Check if T is a container
template <typename T>
struct is_container {
    template <typename U>
    static std::true_type check(typename U::const_iterator*);

    template <typename U>
    static std::false_type check(...);

    static constexpr bool value = decltype(check<T>(nullptr))::value;
};

// Function to print if T is a container
template <typename T>
typename std::enable_if<is_container<T>::value>::type
print(const T& t) {
    for (const auto& element : t) {
        std::cout << element << ' ';
    }
    std::cout << '\n';
}

// Function to print if T is not a container
template <typename T>
typename std::enable_if<!is_container<T>::value>::type
print(const T& t) {
    std::cout << t << '\n';
}

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    print(v);  // Prints: 1 2 3 4 5 

    int i = 42;
    print(i);  // Prints: 42

    return 0;
}
```
in the example, SFINAE is used to enable or disable certain print function overloads depending on whether the argument type is a container.
This allows the print function to behave differently for container types and non-container types.
This kind of functionality is hard to achieve without SFINAE or a similar technique prior to constexpr(c++17) and concept(c++20)

## constexpr If (C++17)

```cpp

#include <iostream>
#include <type_traits>
#include <vector>

// Check if T is a container
template <typename T, typename = void>
struct is_container : std::false_type {};

template <typename T>
struct is_container<T, std::void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end())>> : std::true_type {};

// Function to print
template <typename T>
void print(const T& t) {
    if constexpr (is_container<T>::value) {
        for (const auto& element : t) {
            std::cout << element << ' ';
        }
        std::cout << '\n';
    } else {
        std::cout << t << '\n';
    }
}

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    print(v);  // Prints: 1 2 3 4 5 

    int i = 42;
    print(i);  // Prints: 42

    return 0;
}

```
In this version, the print function uses if constexpr to check at compile time whether the argument type is a container.
If it is, it prints the elements of the container; otherwise, it prints the argument itself.
This eliminates the need for separate function overloads and makes the code simpler and easier to understand.

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
## Concepts (C++20)
With the introduction of Concepts in C++20, we can simplify the code even further.
Concepts allow us to express the intent in the type system and make the code more readable and self-explanatory.

Here’s how you might implement the same functionality using Concepts:

```cpp
#include <iostream>
#include <vector>
#include <concepts>

// Define a concept "Container" that checks if T is a container
template<typename T>
concept Container = requires(T t) {
    { t.begin() } -> std::same_as<typename T::iterator>;
    { t.end() } -> std::same_as<typename T::iterator>;
};

// Function to print if T satisfies the Container concept
template<Container T>
void print(const T& t) {
    for (const auto& element : t) {
        std::cout << element << ' ';
    }
    std::cout << '\n';
}

// Function to print if T does not satisfy the Container concept
template<typename T>
requires (!Container<T>)
void print(const T& t) {
    std::cout << t << '\n';
}

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    print(v);  // Prints: 1 2 3 4 5 

    int i = 42;
    print(i);  // Prints: 42

    return 0;
}

```
In this version, we define a Container concept that checks if a type has begin and end member functions that return an iterator.
We then use this concept to constrain the print function templates.
This makes the code more expressive and easier to understand.

2.
 we further simpify it using constexpr to create a compile-time function that checks if a type is a container, and then use that function in a single print function.
```c++
#include <iostream>
#include <vector>
#include <concepts>

// Define a concept "Container" that checks if T is a container
template<typename T>
concept Container = requires(T t) {
    { t.begin() } -> std::same_as<typename T::iterator>;
    { t.end() } -> std::same_as<typename T::iterator>;
};

// Compile-time function to check if T is a container
template<typename T>
constexpr bool is_container = Container<T>;

// Single print function
template<typename T>
void print(const T& t) {
    if constexpr (is_container<T>) {
        for (const auto& element : t) {
            std::cout << element << ' ';
        }
    } else {
        std::cout << t;
    }
    std::cout << '\n';
}

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    print(v);  // Prints: 1 2 3 4 5 

    int i = 42;
    print(i);  // Prints: 42

    return 0;
}

```
In this code, is_container is a constexpr variable template that checks if a type is a container at compile-time.
Then, in the print function, we use if constexpr to check if T is a container.
If it is, we print each element; if it’s not, we print the value directly.
This way, we avoid the need for function overloading.
This makes the code more readable and maintainable.

3. Example:

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
4.
We can use C++20 concepts and constexpr to simplify the code written using tag dispatch.

```c++

#include <iostream>
#include <vector>

template<typename T>
concept ShapeConcept = requires(T a) {
    { a.calculate() } -> std::same_as<void>;
};

struct Circle {
    constexpr void calculate() const {
        std::cout << "Calculating area of circle\n";
    }
};

struct Square {
    constexpr void calculate() const {
        std::cout << "Calculating perimeter of square\n";
    }
};

struct Cube {
    constexpr void calculate() const {
        std::cout << "Calculating volume of cube\n";
    }
};

int main() {
    std::vector<ShapeConcept auto*> shapes;
    shapes.push_back(new Circle);
    shapes.push_back(new Square);
    shapes.push_back(new Cube);

    for (const auto& shape : shapes) {
        shape->calculate();
    }

    for (const auto& shape : shapes) {
        delete shape;
    }

    return 0;
}
```

In this code, we define a ShapeConcept that requires a calculate method.
Then we define Circle, Square, and Cube structs that satisfy this concept.
In the main function, we create a vector of pointers to objects that satisfy the ShapeConcept.
This allows us to call the calculate method on each shape in the vector.
The constexpr keyword is used to indicate that the calculate method can be evaluated at compile time.
This can potentially improve performance, but in this case, it’s mostly used for clarity.
Note that the auto keyword is used in the vector declaration to allow for different types of shapes.
This is a feature of C++20.
The delete operation is still necessary to free the memory allocated with new.
Please note that this code uses features from C++20 and might not compile with older compilers.
Also, it’s always a good practice to use smart pointers (like std::unique_ptr or std::shared_ptr) instead of raw pointers to avoid memory leaks.
But for the sake of simplicity, raw pointers used here.

* **Here are some advantages and disadvantages of using concepts and constexpr over tag dispatching:**

**Advantages:**
1. **Readability and Maintainability**:
   Concepts make the code more readable and maintainable by clearly specifying the requirements for a type to be used in a particular context.
   This can make it easier to understand how the code is supposed to work and to catch errors.

2. **Compile-time Checks**:
   Concepts provide compile-time checks, ensuring that the types used meet the necessary requirements.
   This can help catch errors earlier in the development process.

3. **Performance**:
   `constexpr` functions are evaluated at compile time when possible, which can potentially lead to performance improvements.

**Disadvantages:**
1. **Compatibility**:
   Concepts and constexpr are features of C++20.
   Therefore, this code will not compile with older compilers that do not support C++20.
2. **Complexity**:
   While concepts can make the code more readable for those familiar with them, they can also add complexity for those who are not.
   This could potentially make the code harder to understand for some developers.
3. **Memory Management**:
   The use of raw pointers in this example (for simplicity) can lead to memory leaks if not handled properly.
   It's generally recommended to use smart pointers instead.

In contrast, tag dispatching is a technique that can be used in older versions of C++, and while it may not provide the same level of type safety and readability as concepts, it can still be a useful technique for controlling the behavior of templates based on type traits.
However, tag dispatching can lead to more verbose code and doesn't provide the same compile-time checks that concepts do.

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
* **CRTP and Heterogeneous Container:**
  The code using CRTP and std::variant/std::visit can achieve a similar effect as runtime polymorphism with virtual dispatch.
  However, it’s important to note that while std::variant/std::visit does provide a form of polymorphism, it’s not the same as runtime polymorphism with virtual dispatch.
  The std::visit call does involve a form of dynamic dispatch, but it’s implemented differently from virtual dispatch and has different trade-offs.

    - **How variant/visit works**:
      `std::variant` is a type-safe union, which can hold a value of any of the specified types.
      When you call `std::visit` on a `std::variant`, it checks the current active type of the variant and then calls the appropriate function (or "visitor") for that type.
      This is done at runtime, but the set of possible types is fixed at compile time.
      This is why `std::variant`/`std::visit` can provide a form of polymorphism.

* **Advantages and Disadvantages of Virtual Dispatch vs Variant/Visit:**

    - **Virtual Dispatch:**
    The main advantage of virtual dispatch is that it allows for true runtime polymorphism,
    where the set of possible types can be extended by deriving new classes,
    and the decision of which function to call is made at runtime based on the dynamic type of the object.
    The main disadvantage is that it involves a level of indirection (the vtable), which can have a performance cost.
    It also requires heap allocation if you want to store derived objects in a container of base class pointers.

    - **Variant/Visit:**
    The main advantage of std::variant/std::visit is that it provides a form of polymorphism while avoiding the need for heap allocation or the indirection of a vtable.
    It can be more efficient than virtual dispatch in some cases.
    The main disadvantage is that the set of possible types is fixed at compile time, so you can’t extend it by deriving new classes.
    It also involves a form of dynamic dispatch, as std::visit needs to check the active type of the variant at runtime.

* **Advantage of CRTP + variant/visit vs runtime polymorphism using virtual function and virtual dispatch:**
  The combination of CRTP and std::variant/std::visit can provide a form of polymorphism that is more efficient than virtual dispatch in some cases, as it avoids the need for heap allocation and the indirection of a vtable.
  It can also take advantage of compile-time polymorphism for some operations, thanks to CRTP.
  However, it doesn’t support true runtime polymorphism, as the set of possible types is fixed at compile time.
  So the choice between these techniques depends on the specific requirements of your program.

    - **Code Size:** Virtual dispatch can potentially result in smaller code size compared to CRTP.
      With CRTP, the compiler generates a separate version of the function for each type that it’s used with, which can increase the code size.
      With virtual dispatch, there’s only one version of the function in the base class.
