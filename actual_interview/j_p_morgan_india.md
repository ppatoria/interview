# Questions:
## What is Rule of 5?
```cpp
#include <iostream>
#include <string>

class Data {
private:
    int value;       // Simple int
    int* ptr;        // Dynamically allocated int
    std::string text; // Standard string

public:
    // Constructor
    Data(int v, int p, const std::string& t) 
        : value(v), ptr(new int(p)), text(t) {}

    // Destructor
    ~Data() {
        delete ptr; // Free dynamically allocated memory
    }

    // Copy Constructor (Deep Copy)
    Data(const Data& other) 
        : value(other.value), ptr(new int(*other.ptr)), text(other.text) {}

    // Copy Assignment Operator (Deep Copy)
    Data& operator=(const Data& other) {
        if (this == &other) return *this; // Self-assignment check

        delete ptr; // Free existing resource

        // Perform deep copy
        value = other.value;
        ptr = new int(*other.ptr);
        text = other.text;

        return *this;
    }

    // Move Constructor (Steal Resources)
    Data(Data&& other) noexcept 
        : value(other.value), ptr(other.ptr), text(std::move(other.text)) {
        other.ptr = nullptr; // Leave the moved-from object in a valid state
    }

    // Move Assignment Operator (Steal Resources)
    Data& operator=(Data&& other) noexcept {
        if (this == &other) return *this; // Self-assignment check

        delete ptr; // Free existing resource

        // Move data
        value = other.value;
        ptr = other.ptr;
        text = std::move(other.text);

        other.ptr = nullptr; // Leave the moved-from object in a valid state

        return *this;
    }

    // Print function to display data
    void print() const {
        std::cout << "Value: " << value 
                  << ", Ptr: " << (ptr ? std::to_string(*ptr) : "null")
                  << ", Text: " << text << std::endl;
    }
};

int main() {
    Data d1(10, 20, "Hello");
    Data d2 = d1;   // Copy Constructor
    Data d3(0, 0, "Empty");
    d3 = d1;        // Copy Assignment

    Data d4 = std::move(d1); // Move Constructor
    Data d5(0, 0, "Temp");
    d5 = std::move(d2);      // Move Assignment

    std::cout << "d3: "; d3.print();
    std::cout << "d4: "; d4.print();
    std::cout << "d5: "; d5.print();

    return 0;
}
```

## What will be the default values of the member variable in the above class ?
If you define a **default constructor** (`Data()`) and do not explicitly initialize the member variables, their values will be **indeterminate** (garbage values for built-in types) or **default-constructed** (for `std::string`).

---

### **Default Values of Member Variables in `Data`**
```cpp
class Data {
private:
    int value;       // Uninitialized (Garbage value)
    int* ptr;        // Uninitialized (Garbage value, dangling pointer)
    std::string text; // Default-initialized (Empty string "")

public:
    Data() {}  // Default constructor (doesn't initialize anything)
    
    void print() const {
        std::cout << "Value: " << value 
                  << ", Ptr: " << ptr  // Could be a garbage address
                  << ", Text: " << text << std::endl;
    }
};
```
### **What happens if we create an object like this?**
```cpp
int main() {
    Data d; // No initialization
    d.print();  // Undefined behavior for value and ptr
    return 0;
}
```
#### **Output (Undefined Behavior)**
```
Value: -13245, Ptr: 0x7ffdb7b26ad0, Text: 
```
- `value`: **Garbage value** (since `int` is not initialized).
- `ptr`: **Garbage value** (dangling pointer, could cause segmentation fault if dereferenced).
- `text`: **Empty string ("")** because `std::string` is default-constructed.

---

### **Fix: Initialize in Default Constructor**
To avoid undefined behavior, explicitly initialize member variables in the default constructor:
```cpp
Data() : value(0), ptr(nullptr), text("") {}
```
Now, if we create a `Data` object, it will have predictable values:
```cpp
int main() {
    Data d;
    d.print();
}
```
#### **Safe Output**
```
Value: 0, Ptr: 0, Text: 
```

---

### **Key Takeaways**
- **Primitive types (`int`, `int*`)** are **not initialized** by default and contain **garbage values**.
- **Pointer (`int*`)** remains **dangling** and can cause crashes if accessed.
- **`std::string`** gets **default-constructed** (i.e., an empty string).
- **Always initialize variables in the default constructor** to prevent undefined behavior.



(2) Constructor 
(3) Copy Constructor 
(4) Move Constructor
(5) make_unique
(6) vector
