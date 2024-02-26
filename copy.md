# Trivialble and Non-Triviable:

1. **Trivial and Non-Trivial Data Structures**
    - **Trivial Data Structure**:
      A data structure is considered trivial if it only contains primitive types (like `int`, `char`, `float`, etc.) and does not contain any pointers, references, or compound data structures (like arrays, structures, or classes).
      For example, an `int` variable is a trivial data structure.

    ```cpp
    int x = 10;
    ```
    - **Non-Trivial Data Structure**:
    A data structure is considered non-trivial if it contains compound data types like arrays, structures, or classes, or if it contains pointers or references.
    For example, a `std::string` or `std::vector<int>` is a non-trivial data structure.

    ```cpp
    std::string str = "Hello, World!";
    std::vector<int> vec = {1, 2, 3, 4, 5};
    ```

2. **Trivially Copyable**
    - A type is trivially copyable if it can be copied bit-by-bit (using `memcpy` or similar), and the resulting value is a valid, independent object.
    This is typically true for simple types, like `int` or `char`, and structures/classes that only contain such types and have trivial or deleted copy/move constructors and assignment operators, and a trivial or deleted destructor.

    ```cpp
    struct TriviallyCopyable {
        int x;
        char y;
    };
    ```

3. **Non-Trivial Copy Semantics**
    - Non-trivial copy semantics refer to situations where simply copying the bits from one object to another is not sufficient to correctly copy an object.
    This is often the case when a class manages resources such as dynamic memory or file handles.
    In these cases, a class will provide a copy constructor that correctly copies the object.
    For example, `std::string` has non-trivial copy semantics because it manages a dynamically-allocated buffer of characters.

    ```cpp
    std::string original = "Hello, World!";
    std::string copy = original;  // Invokes std::string's copy constructor
    ```
    Here, simply copying the bits from `original` to `copy` would result in two `std::string` objects that think they own the same buffer of characters, leading to errors when one of them is destroyed and deletes the buffer.


# Deep Copy

## Question: To help support deep-copying of a runtime polymorphic object, provide a “virtual size_t size() const” function?
### Answer: No.
    This is not a common or recommended practice for supporting deep copying of polymorphic objects.
    Instead, a common practice is to provide a virtual clone() function.

It's important to note that using a `virtual size_t size() const` function to support deep copying of a runtime polymorphic object is not feasible or recommended.
The `size()` function would only provide the size of the object in memory, but it wouldn't provide any information about the object's actual data or structure, which are crucial for creating a deep copy.

    You could theoretically use the `size()` function to allocate the correct amount of memory for a new object, and then use a byte-by-byte copy to duplicate the original object.
But this approach has serious limitations and risks, such as:
    - It would only work correctly if all the data members of the class are trivially copyable (i.e., they can be safely copied byte-by-byte).
      This is not the case for many types of data, including strings, vectors, and any other type that manages its own memory.
    - It would not correctly handle inheritance hierarchies, because the `size()` function would only return the size of the base class part of the object, not any derived class parts.
    - It would not invoke any copy constructors, which could lead to incorrect behavior if the class has any non-trivial copy semantics.

Here's an example of how you might try to use a `size()` function to create a copy of an object, but it is not recommended or safe in general:

```cpp
#include <cstring> // for std::memcpy

class Base {
public:
    virtual ~Base() {}
    virtual size_t size() const = 0;
};

class Derived : public Base {
private:
    int data[10]; // Just for example
public:
    size_t size() const override { return sizeof(Derived); }
};

int main() {
    Derived d;
    Base* b = &d;
    void* copy = operator new(b->size()); // Allocate memory for the copy
    std::memcpy(copy, b, b->size()); // Copy the object byte-by-byte
    // WARNING: 'copy' is now a copy of 'b', but it's not a valid 'Derived' object!
    // We can't safely use it as a 'Derived' or 'Base' object.
    operator delete(copy); // Deallocate the memory
    return 0;
}
```

This is not a safe or recommended way to copy objects in C++.
The correct way to support deep copying of polymorphic objects is to provide a virtual `clone()` function in the base class.
This function is then overridden in each derived class to return a copy of the derived object. Here's an example:

```cpp
class Base {
public:
    virtual ~Base() {}
    virtual Base* clone() const = 0; // Pure virtual function
};

class Derived : public Base {
public:
    Derived* clone() const override { return new Derived(*this); } // Copy constructor
};

int main() {
    Derived d;
    Base* b = d.clone(); // b now points to a copy of d
    delete b; // Don't forget to delete!
    return 0;
}
```

In this example, `Derived::clone()` returns a new `Derived` object that is a copy of the original.
This allows us to make a deep copy of a `Derived` object even when we only have a `Base` pointer.
This is why it's a common practice for supporting deep copying of polymorphic objects.
It's more flexible and applicable in more scenarios compared to a `size_t size() const` function.



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
