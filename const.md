# const keyword does not gurantee all class invariants:
The const keyword can help maintain some invariants, but it doesn’t guarantee all class invariants.
Class invariants depend on the logic of your code.

(1) **Class invariants** are a set of conditions that must be true for all instances of a class.
    They are used to constrain objects of a class and ensure that the state stored in the object meets predefined conditions.
    Methods of the class should preserve the invariant, and the invariant is established during construction and constantly maintained between calls to public methods.

(2) Let's consider an example of a `Rectangle` class with two private data members `width` and `height`.
    The class invariant for this class could be that `width` and `height` must always be greater than zero.
    Here's an implementation of the `Rectangle` class in C++:

```c++
#include <iostream>

class Rectangle {
public:
    Rectangle(int w, int h) : width(w), height(h) {}
    int getArea() const { return width * height; }
private:
    const int width;
    const int height;
};

int main() {
    Rectangle r(3, 4);
    std::cout << "Area of rectangle with width " << r.getWidth() << " and height " << r.getHeight() << " is " << r.getArea() << std::endl;
    return 0;
}
```

In this example, the class invariant is that `width` and `height` must always be greater than zero.
The `const` keyword is used to ensure that these values cannot be changed after construction.
However, the `const` keyword does not guarantee that the class invariant is maintained.
For example, if we add a method to the `Rectangle` class that sets the `width` or `height` to a negative value, the invariant would be violated.
Here's an example of such a method:

```c++
void Rectangle::setWidth(int w) {
    width = w;
}
```

To maintain the class invariant, we could add a check to this method to ensure that `w` is greater than zero:

```c++
void Rectangle::setWidth(int w) {
    if (w > 0) {
        width = w;
    }
}
```

This ensures that the class invariant is maintained, even if the `const` keyword is not sufficient to guarantee it.

The const keyword in C++ doesn't guarantee all class invariants.
While it offers valuable benefits, it has limitations that require careful consideration when defining class behavior:
## Guarantee can be broken in following scenarious:
### Modifying Non-Const Members:

Even in a const member function, non-const member variables can still be modified.
This can lead to unexpected behavior if the modification violates the class's intended invariants.

```c++
class Rectangle {
public:
    Rectangle(int w, int h) : width(w), height(h) {}
    int getArea() const { return width * height; }
    void setWidth(int w) const { width = w; } // This method violates the class invariant
private:
    mutable int width;
    const int height;
};

int main() {
    const Rectangle r(3, 4);
    std::cout << "Area of rectangle with width " << r.getWidth() << " and height " << r.getHeight() << " is " << r.getArea() << std::endl;
    r.setWidth(-1); // This call violates the class invariant
    std::cout << "Area of rectangle with width " << r.getWidth() << " and height " << r.getHeight() << " is " << r.getArea() << std::endl;
    return 0;
}
```

In this example, the `setWidth` method modifies the `width` member variable, even though it is declared as `const`. This violates the class invariant that `width` must always be greater than zero. The `mutable` keyword is used to allow the `width` variable to be modified in a `const` method, but it does not guarantee that the class invariant is maintained.

### External Modifications:
The const keyword only applies to the member functions and data members within the class itself.
It doesn't prevent external code from modifying the object through non-const member functions or public accessors, potentially breaking the invariants.

```c++
class Rectangle {
public:
    Rectangle(int w, int h) : width(w), height(h) {}
    int getArea() const { return width * height; }
private:
    const int width;
    const int height;
};

void modifyRectangle(Rectangle& r) {
    r.setWidth(-1); // This call violates the class invariant
}

int main() {
    const Rectangle r(3, 4);
    std::cout << "Area of rectangle with width " << r.getWidth() << " and height " << r.getHeight() << " is " << r.getArea() << std::endl;
    modifyRectangle(r); // This call violates the class invariant
    std::cout << "Area of rectangle with width " << r.getWidth() << " and height " << r.getHeight() << " is " << r.getArea() << std::endl;
    return 0;
}
```

In this example, the `modifyRectangle` function modifies the `width` member variable of the `Rectangle` object passed to it, even though it is declared as `const`.
This violates the class invariant that `width` must always be greater than zero.


### Derived Classes:
A derived class can inherit a const member function from its base class, but it can still override it with a non-const version.
This can potentially allow the derived class to violate the base class's invariants.

```c++
class Shape {
public:
    virtual int getArea() const = 0;
};

class Rectangle : public Shape {
public:
    Rectangle(int w, int h) : width(w), height(h) {}
    int getArea() const override { return width * height; }
private:
    const int width;
    const int height;
};

class Square : public Rectangle {
public:
    Square(int s) : Rectangle(s, s) {}
    int getArea() override { return side * side; } // This method violates the base class's invariant
private:
    const int side;
};

int main() {
    const Shape& s = Square(3);
    std::cout << "Area of square with side " << s.getSide() << " is " << s.getArea() << std::endl;
    return 0;
}
```

In this example, the `Square` class inherits the `getArea` method from its base class `Rectangle`, but it overrides it with a non-`const` version that calculates the area using the `side` member variable instead of the `width` and `height` variables.
This violates the base class's invariant that `width` and `height` must always be greater than zero.

## To address these limitations, consider these strategies:
### Enforce const-correctness: Make member functions and data members const whenever possible to prevent unintended modifications.
### Use private members: Make critical data members private to restrict direct access from external code.
### Consider immutability: If the class truly shouldn't be modified after creation, explore making it immutable by using const constructors and disallowing non-const member functions.



















Certainly! Here are some examples:

(1) **Modifying Non-Const Members**: Consider the following example:


**External Modifications**: Consider the following example:


**Derived Classes**: Consider the following example:


(2) **Enforce const-correctness**: This strategy involves making member functions and data members `const` whenever possible to prevent unintended modifications. For example, we could modify the `Rectangle` class from the previous examples to use `const` member functions:

```c++
class Rectangle {
public:
    Rectangle(int w, int h) : width(w), height(h) {}
    int getArea() const { return width * height; }
    void setWidth(int w) { if (w > 0) width = w; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
private:
    int width;
    const int height;
};
```

This ensures that the `width` variable can only be modified through the `setWidth` method, which checks that the new value is greater than zero.

**Use private members**: This strategy involves making critical data members private to restrict direct access from external code. For example, we could modify the `Rectangle` class from the previous examples to make the `width` and `height` variables private:

```c

Source: Conversation with Bing, 2/29/2024
(1) Const keyword in C++ - GeeksforGeeks. https://www.geeksforgeeks.org/const-keyword-in-cpp/.
(2) Understanding how to correctly treat c++ class constants. https://stackoverflow.com/questions/5620256/understanding-how-to-correctly-treat-c-class-constants.
(3) Override const function to modify a class variable in C++?. https://stackoverflow.com/questions/20522234/override-const-function-to-modify-a-class-variable-in-c.
(4) C++ Constants - Programiz. https://www.programiz.com/cpp-programming/constants.
(5) c++ - Using 'const' in class's functions - Stack Overflow. https://stackoverflow.com/questions/2157458/using-const-in-classs-functions.
(6) How to Use Const Keyword in C++? - GeeksforGeeks. https://www.geeksforgeeks.org/how-to-use-const-keyword-in-cpp/.
(7) const type qualifier - cppreference.com. https://en.cppreference.com/w/c/language/const.
