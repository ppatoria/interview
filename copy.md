# POD, Trivial Type and Trivially Copyable Type:

## **POD (Plain Old Data)**:
    A type is considered POD if it is both trivial and standard-layout.
    This means it can be safely copied with `memcpy` or serialized to and from byte arrays.
    Examples include basic types like `int`, `char`, `double`, etc., as well as arrays and structs that only contain POD types.
    A POD type in C++ is a data type that has no "magic" going on in the structure, such as hidden pointers to vtables, offsets that get applied to the address when it is cast to other types, constructors, or destructors.
    They can be copied into an array of `char` or `unsigned char`, and safely copied back into a variable of the POD type.
    Examples of POD types include:
        - All fundamental types (like `int`, `char`, `double`, etc.)
        - Enumerations
        - Pointers
        - Arrays of POD types
        - Classes and structs that only contain POD types as members, have no user-defined destructor, no user-defined copy assignment operator, and no non-static members of pointer-to-member type.
    Please note that the rules surrounding POD were relaxed in C++11, allowing more types to qualify as POD.
    For example, in C++11 and later, a class with a user-defined constructor can still be a POD type if the constructor is trivial.
    You can use `std::is_pod<T>()` in C++11 and later to check if a type `T` is a POD¹.
    However, `std::is_pod` is deprecated in C++20 and removed in C++23.

   Here are some characteristics of POD types:
### POD are trivial types, meaning they have a
   1. **Trivial Default Constructor**:
      A default constructor is trivial if it is not user-provided, meaning it is implicitly-defined or defaulted.
      It performs no action and all data types compatible with the C language (POD types) are trivially default-constructible.
   2. **Trivial Copy Constructor**:
      A copy constructor is trivial if it is not user-provided, meaning it is implicitly-defined or defaulted.
      It creates a bytewise copy of the object representation of the argument, and performs no other action.
   3. **Trivial Copy Assignment Operator**:
      A copy assignment operator is trivial if it is not user-provided, meaning it is implicitly-defined or defaulted.
      It copies the given class object to a new class object.
   4. **Trivial Destructor**:
      A destructor is trivial if it is not user-provided, meaning it is implicitly-defined or defaulted.
      It performs no action.

### Standard-layout Types:
    POD are standard-layout types, meaning they have a well-defined memory layout.
    A type is considered a standard-layout type if it does not use certain C++ features unavailable in C, and all non-static data members have the same access control.
    Such a type is memcopy-able, and its layout is defined so that the same type can be used in a C program.
    Note: In C++, a class can be considered a standard-layout type even if it has private or protected non-static data members.
    The key is that all non-static data members must have the same access control (i.e., all public, all protected, or all private).

    Here are some examples of standard-layout types:

    ```cpp
    // Example 1: A struct with public data members
    struct S1 {
        int x;
        double y;
    };

    // Example 2: A class with public data members
    class C1 {
    public:
        int x;
        double y;
    };

    // Example 3: A class with private data members and public static data member
    class C2 {
    public:
        static int x;
    private:
        double y;
    };
    ```

    In these examples, `S1` and `C1` are standard-layout types because they only contain non-static data members with the same access control (public).
    `C2` is also a standard-layout type because the static data member does not affect the layout of the non-static data members.
    However, if `C2` had another non-static data member that was public, it would not be a standard-layout type.

    Remember, a type is a standard-layout type if it meets all of the following criteria:
        - All non-static data members have the same access control (public, private, or protected).
        - The class or struct has no virtual functions or virtual base classes.
        - The class or struct has no non-static data members of reference type.
        - All non-static data members and base classes are themselves standard-layout types.
        - The order of declaration for non-static data members is the same in both the class definition and the constructor initializer list.
        - The class or struct has no base classes of the same type as the first non-static data member.

### Well-defined Memory Layout:
        This refers to how the members of an object of class, struct, or union type are arranged in memory.
        In some cases, the layout is well-defined by the language specification.
        But when a class or struct contains certain C++ language features such as virtual base classes, virtual functions, members with different access control, then the compiler is free to choose a layout.
        That layout may vary depending on what optimizations are being performed and in many cases, the object might not even occupy a contiguous area of memory.


## **Trivial Type**:
   A type is considered trivial if it has a trivial default constructor, copy constructor, copy assignment operator, and destructor.
   This means that the type can be statically initialized, safely copied with `memcpy`, and does not require any custom logic when it is created, copied, assigned, or destroyed.

## **Trivially Copyable Type**:
   A type is considered trivially copyable if it can be safely copied with `memcpy.
   This includes all trivial types, but also includes some types that are not trivial.
   For example, a type with a deleted default constructor is not trivial, but it is still trivially copyable if it has a trivial copy constructor and trivial destructor.

In summary, all PODs are trivial and trivially copyable, all trivial types are trivially copyable, but not all trivially copyable types are trivial or POD.
The requirements for being trivial or a POD are stricter than for being trivially copyable.


# Trivialble and Non-Triviable:

## Trivial and Non-Trivial Data Structure
### Trivial Data Structure:
      A data structure is considered trivial if it only contains primitive types (like `int`, `char`, `float`, etc.) and does not contain any pointers, references, or compound data structures (like arrays, structures, or classes).
      For example, an `int` variable is a trivial data structure.

    ```cpp
    int x = 10;
    ```

### Non-Trivial Data Structure:
    A data structure is considered non-trivial if it contains compound data types like arrays, structures, or classes, or if it contains pointers or references.
    For example, a `std::string` or `std::vector<int>` is a non-trivial data structure.

    ```cpp
    std::string str = "Hello, World!";
    std::vector<int> vec = {1, 2, 3, 4, 5};
    ```

## Trivially Copyable
    - A type is trivially copyable if it can be copied bit-by-bit (using `memcpy` or similar), and the resulting value is a valid, independent object.
    This is typically true for simple types, like `int` or `char`, and structures/classes that only contain such types and have trivial or deleted copy/move constructors and assignment operators, and a trivial or deleted destructor.

    ```cpp
    struct TriviallyCopyable {
        int x;
        char y;
    };
    ```

## **Non-Trivial Copy Semantics**
    - Non-trivial copy semantics refer to situations where simply copying the bits from one object to another is not sufficient to correctly copy an object.
      This is often the case when a class manages resources such as dynamic memory or file handles.
      In these cases, a class will provide a copy constructor that correctly copies the object.
      For example, `std::string` has non-trivial copy semantics because it manages a dynamically-allocated buffer of characters.

    ```cpp
    std::string original = "Hello, World!";
    std::string copy = original;  // Invokes std::string's copy constructor
    ```
    Here, simply copying the bits from `original` to `copy` would result in two `std::string` objects that think they own the same buffer of characters, leading to errors when one of them is destroyed and deletes the buffer.


# Question: memset() your large objects to 0 bytes to avoid uninitialized memory reads ?
## Answer: Incorrect.
    In C++, using memset() on non-trivial objects can lead to undefined behavior.
    It’s better to initialize your objects properly using constructors.

### Setting large objects memory with 0 to avoid uninitialized memory reads:
    In C++, for trivial types (like built-in types and PODs), you can use `memset()` to initialize memory to zero. Here's an example:

    ```cpp
    #include <cstring>

    int main() {
        int arr[100];
        std::memset(arr, 0, sizeof(arr));  // Initialize all elements to 0
        // Now arr is fully initialized and can be used safely.
    }
    ```

    However, for non-trivial types (like classes with constructors, destructors, or virtual functions), using `memset()` can lead to undefined behavior.

#### Using memset() on non-trivial objects leads to undefined behavior:

    Here's an example of how using `memset()` on non-trivial objects can lead to undefined behavior:

    ```cpp
    #include <cstring>

    class NonTrivial {
    public:
        NonTrivial() : num(42) {}
        virtual void doSomething() {}  // Virtual function makes this class non-trivial
    private:
        int num;
    };

    int main() {
        NonTrivial obj;
        std::memset(&obj, 0, sizeof(obj));  // Undefined behavior!
    }
    ```

    In this case, `memset()` overwrites the vtable pointer of `obj`, which leads to undefined behavior when we try to call `doSomething()`.

#### Safe solution with constructor:
    The safe way to initialize non-trivial objects is to use their constructors.
    If you want to reset the object to some initial state, you can provide a `reset()` member function:

    ```cpp
    class NonTrivial {
    public:
        NonTrivial() : num(42) {}
        virtual void doSomething() {}
        void reset() { num = 42; }  // Reset function to reinitialize the object
    private:
        int num;
    };

    int main() {
        NonTrivial obj;
        obj.reset();  // Safe way to "reset" the object
    }
    ```

    In this case, `reset()` safely reinitializes `obj` without causing undefined behavior.
    This is the preferred way to handle initialization and resetting of non-trivial objects in C++.

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
