# Copyable and Non-Copyable classes
## Question: Base classes with virtual functions should not be copyable?
### Answer: Yes.
    Base classes with virtual functions should not be copyable.
    This is a good rule of thumb to prevent slicing, where an object of a derived class is copied into an object of the base class, losing the derived class data.

1. **Slicing**:
   Slicing is a phenomenon in C++ where an object of a derived class is copied into an object of the base class, causing the derived class data to be lost.
   This happens because the base class knows nothing about the extra data in the derived class.
   When a base class object is created from a derived class object, only the base class part of the derived class object is copied.
   This is known as slicing.

Here's an example to illustrate this:


```cpp
#include <iostream>
#include <memory>

class Base {
public:
    virtual void print() const {
        std::cout << "Base\n";
    }
    virtual ~Base() = default;  // Don't forget the virtual destructor!
};

class Derived : public Base {
private:
    int data;
public:
    Derived(int value) : data(value) { }
    void print() const override {
        std::cout << "Derived: " << data << '\n';
    }
};

/* Slicing to call print function of Base class. This is design flaw.*/
void slicing() {
    Derived d(42);
    Base b = d;  // Slicing occurs here
    b.print();   // Prints "Base", not "Derived: 42"
}
/* Better (not perfect) way to call print function of Base class without slicing.*/
int better_approach() {
    std::unique<Base> b = std::make_unique<Derived>(42);
    b->Base::print();   // Prints "Base", not "Derived: 42"
}

int main(){
    slicing();
    better_approach();
}
```

In this code, `Derived` is a subclass of `Base` and overrides the `print` function.
In `main`, a `Derived` object `d` is created and then copied into a `Base` object `b`.
This is where slicing occurs: the `data` member of `d` is not copied to `b`, because `Base` knows nothing about `data`.
When `b.print()` is called, it prints "Base", not "Derived: 42", because `b` is a `Base` object, not a `Derived` object.

**To prevent slicing, make the base class non-copyable by:**
    - Deleting the copy constructor and the copy assignment operator.

```cpp
class Base {
public:
    Base(const Base&) = delete;
    Base& operator=(const Base&) = delete;

    virtual void print() const {
        std::cout << "Base\n";
    }
};
```

Now, if you try to copy a `Derived` object into a `Base` object, the compiler will give an error, preventing slicing.
This is why it's a good rule of thumb to make base classes with virtual functions non-copyable.
It helps prevent slicing and makes your code safer.

1. **When is slicing required?**
   Slicing is not typically "required" and is generally something to be avoided because it can lead to unexpected behavior.
   However, there might be some scenarios where you intentionally want to use only the base part of a derived object.
   For example, if you have a collection of different types of objects (all derived from the same base class) and you want to perform a common operation that is defined in the base class, you might slice the objects down to the base class.

2. [Is vtable only created when objects are created on heap?](./virtual.md)
