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

body {
  margin-left: 220px;
}
</style>

# Virtual over Generic

## Question: Which of the following are common reasons to prefer virtual dispatch over templates:
Pick ONE OR MORE options.  
1. **Wanting to share the implementation code in header files with users of the interface.**  
2. **Wanting to hide the implementation code for faster compilation times.**  
3. **Better code optimisation opportunities for the compiler.**  
4. **Better compilation error messages (pre C++20)**  
5. **Ability to selectively include functions in the API based on the types involved.**

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

# Related questions that can be asked during this question in the interview.

## Various techniques and patters for compile time polymorphism equivalent to virtual dispatch.
A simple example where we have different types of shapes and we want to calculate their area.
Here's how you might implement this using different techniques:

**1. Virtual Dispatch (Runtime Polymorphism)**

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

**2. CRTP (Compile-time Polymorphism)**
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
1. **CRTP and Heterogeneous Container**: Yes, the code you've shown using CRTP and `std::variant`/`std::visit` can achieve a similar effect as runtime polymorphism with virtual dispatch. However, it's important to note that while `std::variant`/`std::visit` does provide a form of polymorphism, it's not the same as runtime polymorphism with virtual dispatch. The `std::visit` call does involve a form of dynamic dispatch, but it's implemented differently from virtual dispatch and has different trade-offs.

2. **How variant/visit works**: `std::variant` is a type-safe union, which can hold a value of any of the specified types. When you call `std::visit` on a `std::variant`, it checks the current active type of the variant and then calls the appropriate function (or "visitor") for that type. This is done at runtime, but the set of possible types is fixed at compile time. This is why `std::variant`/`std::visit` can provide a form of polymorphism.

3. **Advantages and Disadvantages of Virtual Dispatch vs Variant/Visit**:
   - **Virtual Dispatch**: The main advantage of virtual dispatch is that it allows for true runtime polymorphism, where the set of possible types can be extended by deriving new classes, and the decision of which function to call is made at runtime based on the dynamic type of the object. The main disadvantage is that it involves a level of indirection (the vtable), which can have a performance cost. It also requires heap allocation if you want to store derived objects in a container of base class pointers.
   - **Variant/Visit**: The main advantage of `std::variant`/`std::visit` is that it provides a form of polymorphism while avoiding the need for heap allocation or the indirection of a vtable. It can be more efficient than virtual dispatch in some cases. The main disadvantage is that the set of possible types is fixed at compile time, so you can't extend it by deriving new classes. It also involves a form of dynamic dispatch, as `std::visit` needs to check the active type of the variant at runtime.

4. **Advantage of CRTP + variant/visit vs runtime polymorphism using virtual function and virtual dispatch**: The combination of CRTP and `std::variant`/`std::visit` can provide a form of polymorphism that is more efficient than virtual dispatch in some cases, as it avoids the need for heap allocation and the indirection of a vtable. It can also take advantage of compile-time polymorphism for some operations, thanks to CRTP. However, it doesn't support true runtime polymorphism, as the set of possible types is fixed at compile time. So the choice between these techniques depends on the specific requirements of your program.

**3. Tag Dispatching**
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

**4. Expression Templates**

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
**5. Compile-time type-based dispatch**

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

**6. Overloading**

```cpp
struct Circle { double radius; };
struct Square { double side; };

double area(const Circle& circle) { return 3.14159 * circle.radius * circle.radius; }
double area(const Square& square) { return square.side * square.side; }
```

**7. SFINAE**

```cpp
template <typename T, typename = std::enable_if_t<std::is_same_v<T, Circle>>>
double area(const T& circle) { return 3.14159 * circle.radius * circle.radius; }

template <typename T, typename = std::enable_if_t<std::is_same_v<T, Square>>>
double area(const T& square) { return square.side * square.side; }
```

**8. constexpr If (C++17)**

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
**9. Concepts (C++20)**

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
In this example,
the printArea function template uses the Shape concept to constrain its template parameter,
ensuring that it only accepts types that satisfy the Shape concept.

### Here's a brief overview of when you might want to use each technique and their advantages:

1. **Virtual Dispatch (Runtime Polymorphism)**:
   Use when you have a hierarchy of related types and you want to write code that can work with any type in the hierarchy.
   The advantage is that it allows for dynamic behavior based on the runtime type of the object.

2. **CRTP (Curiously Recurring Template Pattern)**:
   Use when you want to achieve static polymorphism (i.e., behavior that depends on the type, but is resolved at compile time).
   The advantage is that it can provide performance benefits over virtual dispatch by avoiding virtual function calls.

3. **Tag Dispatching**:
   Use when you have several overloaded functions and you want to select one based on the type traits of the arguments.
   The advantage is that it can make your code more modular and easier to extend.

4. **Expression Templates**:
   Use when you're implementing mathematical expressions or similar constructs and you want to avoid unnecessary temporary objects.
   The advantage is that it can provide significant performance benefits.

5. **Compile-time type-based dispatch**:
   Use when you want to select different implementations based on the properties of types at compile time.
   The advantage is that it can make your code more flexible and easier to maintain.

6. **Overloading**:
   Use when you have several functions that do similar things but for different types.
   The advantage is that it can make your code easier to read and write.

7. **SFINAE (Substitution Failure Is Not An Error)**:
   Use when you want to enable or disable certain template instantiations based on the properties of the template arguments.
   The advantage is that it can make your templates more flexible and robust.

8. **constexpr If (C++17)**:
   Use when you want to conditionally compile code based on a constant expression.
   The advantage is that it can make your code more efficient by eliminating unnecessary code at compile time.

9. **Concepts (C++20)**:
   Use when you want to specify the requirements on template arguments.
   The advantage is that it can make your templates easier to use and understand,
   and it can provide better error messages when the requirements are not met.

### There are certain scenarios where virtual dispatch can achieve things that CRTP cannot, or can do better than CRTP:

1. **Runtime Polymorphism**:
   The primary advantage of virtual dispatch is that it enables runtime polymorphism.
   This means you can decide which function to call based on the dynamic type of an object at runtime.
   CRTP, on the other hand, resolves the function call at compile time, so it doesn't support this kind of dynamic behavior.

2. **Ease of Use with Collections**:
   If you have a collection of objects of different types derived from a common base class,
   you can easily iterate over the collection and call a virtual function on each object,
   and the correct function will be called based on the dynamic type of the object.

   With CRTP, this is not straightforward because objects of different instantiations of the CRTP template are of different types.

3. **Code Size**:
   Virtual dispatch can potentially result in smaller code size compared to CRTP.
   With CRTP, the compiler generates a separate version of the function for each type that it's used with, which can increase the code size.
   With virtual dispatch, there's only one version of the function in the base class.

4. **Simplicity**:
   Virtual dispatch can be simpler to use and understand, especially for less experienced programmers.
   CRTP involves more complex C++ features like templates and can be harder to get right.


#### A real-world scenario where runtime polymorphism (virtual dispatch) is beneficial and cannot be replaced by CRTP or other compile-time polymorphism techniques.

Imagine you're developing a graphics rendering engine.
In this engine, you have a variety of different shapes that can be drawn, such as circles, rectangles, triangles, etc.
Each of these shapes is represented by a different class (Circle, Rectangle, Triangle, etc.), and all of these classes inherit from a common base class, Shape.

Now, you want to maintain a collection of these shapes in your engine,
and you want to be able to iterate over this collection and call a `draw()` function on each shape to draw it on the screen.
The implementation of the `draw()` function is different for each shape
(i.e., the way you draw a circle is different from the way you draw a rectangle or a triangle).

Here's where runtime polymorphism comes into play.
You can declare a virtual `draw()` function in the base Shape class and override this function in each of the derived classes (Circle, Rectangle, Triangle, etc.).
Then, you can maintain a collection of Shape pointers, where each pointer points to an object of one of the derived classes.
When you iterate over this collection and call the `draw()` function on each Shape pointer,
the correct `draw()` function will be called based on the dynamic type of the object that the pointer points to.

This is something that cannot be achieved with CRTP or other compile-time polymorphism techniques.
With CRTP, the `draw()` function call would be resolved at compile time,
so you wouldn't be able to use a single collection of Shape objects to hold objects of different types and call the correct `draw()` function on each one at runtime.
This is a clear advantage of virtual dispatch over CRTP in this scenario.

#### Here are a few scenarios in an Investment Bank Trading System where runtime polymorphism (virtual dispatch) might be beneficial:

1. **Different Types of Financial Instruments**:
   An investment bank deals with various types of financial instruments like stocks, bonds, derivatives, etc.
   Each of these can be represented by a different class (Stock, Bond, Derivative, etc.), all inheriting from a common base class, FinancialInstrument.
   Each instrument type might have a different method to calculate its price, risk, or other metrics.
   Using virtual dispatch, you can maintain a collection of FinancialInstrument pointers, and call the appropriate method on each one at runtime.

2. **Order Execution Strategies**:
   Different strategies might be used to execute orders based on various factors like order size, market volatility, etc.
   These strategies can be represented by different classes inheriting from a common base class, OrderExecutionStrategy.
   Using virtual dispatch, the appropriate strategy can be chosen at runtime based on the current market conditions.

3. **Risk Models**:
   Different risk models might be used to calculate the risk associated with a portfolio.
   These models can be represented by different classes inheriting from a common base class, RiskModel.
   Using virtual dispatch, the appropriate model can be chosen at runtime based on the portfolio's characteristics.

4. **Pricing Models**:
   Different pricing models might be used to price derivatives based on their characteristics.
   These models can be represented by different classes inheriting from a common base class, PricingModel.
   Using virtual dispatch, the appropriate model can be chosen at runtime.

In all these scenarios, the decision about which method to call is made at runtime based on the dynamic type of the objects.
This is something that cannot be achieved with compile-time polymorphism techniques like CRTP.
The ability to make these decisions at runtime is a clear advantage of virtual dispatch in these scenarios.
