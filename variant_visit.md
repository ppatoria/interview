# type-safe Union:
A type-safe union is a data structure that can hold values of different types
but ensures that the type of the value currently held by the union
is always known and enforced at compile-time.

This means that you cannot accidentally interpret the value of one type as another,
which can prevent many types of programming errors.

In contrast, a type-unsafe union (often just called a union) is a data structure that can also hold values of different types,
but does not keep track of what type of value it currently holds.
This means that it's up to the programmer to ensure that they are using the correct type when accessing the union's value,
which can lead to errors if not done carefully.

An example of a type-unsafe union in C:

```c
union Data {
   int i;
   float f;
   char str[20];
};

int main() {
   union Data data;

   data.i = 10;
   printf("%d\n", data.i); // Correct usage

   data.f = 220.5;
   printf("%f\n", data.f); // Correct usage

   // Incorrect usage: this will compile but the result is undefined
   // because we're accessing the union through an incorrect type
   printf("%d\n", data.i);

   return 0;
}
```

In the above example, the union `Data` can hold an integer, a float, or a string.
However, it's not type-safe because it's possible to write code that compiles but causes undefined behavior at runtime due to incorrect type usage.

A type-safe union would be something like `std::variant` in C++17 and later, which keeps track of the type stored and provides type-safe access:

```cpp
#include <variant>
#include <iostream>
#include <string>

int main() {
   std::variant<int, float, std::string> data;

   data = 10;
   std::cout << std::get<int>(data) << std::endl; // Correct usage

   data = 220.5f;
   std::cout << std::get<float>(data) << std::endl; // Correct usage

   // Incorrect usage: this will not compile because it's not type-safe
   // std::cout << std::get<int>(data) << std::endl;

   return 0;
}
```

In this example, `std::variant` is used to create a type-safe union.
Attempting to access the value with the wrong type will result in a compile-time error, preventing the kind of undefined behavior seen with traditional unions.
This is what makes `std::variant` type-safe.

# Type punning in `union` and `undefined behavior`. How its prevented in `std::variant`:
Type punning refers to a programming technique where the same piece of memory is treated as if it were a different data type.
This can be done for various reasons,
  - such as to interpret the underlying binary representation of a data type or
  - to bypass type system restrictions for certain operations.

## Example of Type Punning:
In C and C++, one common way to perform type punning is by using unions.
An example:

```c
union Data {
    float f;
    int i;
};

Data data;
data.f = 1.23f; // set the float field
int intRepresentation = data.i; // read the bits as an integer
```

In this example, we write to the `float` field of the union and then read from the `int` field.
This allows us to access the raw binary representation of the floating-point number as an integer.

## Scenarios Where Type Punning is Used:
- Interpreting the bit patterns of different types, such as reading the binary representation of a float as an integer.
- Performing bitwise operations on types that are not normally subject to such operations.
- Optimizing code by avoiding costly type conversions or operations.

## How `std::variant` Prevents Undefined Behavior:
`std::variant` is a type-safe alternative to unions that was introduced in C++17.
It prevents the undefined behavior associated with type punning in the following ways:

- **Type Safety**        : `std::variant` keeps track of the active type and only allows access to that type, preventing accidental access to the wrong type.
- **Visitation**         : Accessing the value in a `std::variant` is done through visitation, which is a type-safe way of applying an operation to the contained value.
                           The visitor must handle all possible types that the variant could hold.
- **No Aliasing Issues** : Since `std::variant` enforces type safety and does not allow direct memory reinterpretation,
                           it avoids the strict aliasing rule violations that can occur with unions.

An example of using `std::variant`:

```cpp
#include <variant>
#include <iostream>

std::variant<int, float> data;

data = 1.23f; // set the variant to hold a float
// Visit the variant and perform an action based on the contained type
std::visit([](auto&& arg) {
    using T = std::decay_t<decltype(arg)>;
    if constexpr (std::is_same_v<T, float>) {
        std::cout << "Float: " << arg << '\n';
    } else if constexpr (std::is_same_v<T, int>) {
        std::cout << "Int: " << arg << '\n';
    }
}, data);
```

In this example, `std::visit` is used to safely access the value in the `std::variant`.
The visitor handles each type that the variant could hold, ensuring that the correct type is accessed and processed.
This eliminates the risk of undefined behavior that comes with type punning in traditional unions.

# Variant:
`std::variant` is a feature introduced in C++17 that represents a type-safe union known as a variant.
Here's a detailed explanation:

## Design and Purpose:
- **Design**  : `std::variant` holds a value of one of its specified types or no value (in case of an error).
                It's a template class that can be instantiated with only one of its specified types at any given time.
- **Purpose** : It's designed to safely store values of different types and to be a better alternative to C unions, which are not type-safe.

## Implementation Under the Hood:
- **Storage**     : Internally, `std::variant` uses enough storage to hold the largest of its alternative types
                    and typically also stores a small amount of additional data to keep track of which type is currently held.
- **Type-Safety** : It ensures type-safety by keeping track of the type currently held and only allowing access to that type,
                    preventing undefined behavior associated with type punning in unions.
                   **Storage:** Similar to a C-style union, std::variant also allocates memory equal to the size of its largest member.
                   However, it holds additional information about the active type.
                   **Type Safety:** std::variant provides type-safe access by keeping track of which type is currently active, using an index or type tag.
- **Visitation**  : Accessing the stored value is done through visitation, where a visitor function or lambda is applied to the contained value.

### Efficiency:
- **Runtime Overhead** : `std::variant` has minimal runtime overhead compared to other polymorphic types since it doesn't use dynamic allocation or virtual functions.
- **Memory Usage**     : The size of a `std::variant` is the size of its largest type plus any additional storage required for type management,
                         which can lead to wasted space if the size difference between the types is significant.

### When to Use:
- **Multiple Types**   : Use `std::variant` when a variable may hold values of different types but only one at a time.
- **Type-Safe Unions** : It's a good choice when you need the functionality of a union but with type safety.
- **State Management** : It's useful for managing state in a type-safe manner, such as in finite state machines.

### When Not to Use (Alternatives):
- **Single Type**   : If a variable will only ever hold one type, use that type directly instead of a variant.
- **Inheritance**   : If you need runtime polymorphism with a fixed set of types that share a common interface,
                      traditional inheritance with virtual functions might be more appropriate.
- **Boost.Variant** : Before C++17 or if `std::variant` doesn't meet your needs, consider using `boost::variant`, which offers similar functionality.
- **std::any**      : If you need to store any type but don't need to know the specific type at compile-time, `std::any` might be a better fit.
- **std::optional** : For a single value that may or may not be present, `std::optional` is more suitable.

# Strict Aliasing and Vairant
Aliasing occurs when two different references point to the same memory location.
In programming, this can happen when you have multiple pointers or references to the same variable
or when you use a union to interpret a memory location in multiple ways.

Strict aliasing is a rule in C and C++ that restricts how you can access a memory location through pointers or references of different types.
The rule states that an object in memory can only be accessed by a pointer or reference of a compatible type,
except for certain exceptions like `char*` or `std::byte*`, which are allowed to alias any type.

## How Unions Do Not Follow Strict Aliasing:
A union in C or C++ allows you to store different data types in the same memory location and access that memory as any of its member types.
This can violate the strict aliasing rule because you might write to the union using one type and read from it using another, which is not generally allowed under the rule.

### How `std::variant` Prevents Type Punning:
`std::variant` is a type-safe union that prevents type punning by maintaining active type information.
It ensures that you can only access the stored value as the type it was set with.
If you try to access the value as a different type, `std::variant` will throw a `std::bad_variant_access` exception,
enforcing type safety and adhering to the strict aliasing rule.

Here's an example of type punning with a union:

```c
union Data {
    int i;
    float f;
};

Data data;
data.i = 42;
float floatView = data.f; // Type punning: interpreting int memory as float
```

And here's how `std::variant` prevents it:

```cpp
#include <variant>
#include <iostream>

std::variant<int, float> data;

data = 42; // data holds an int
try {
    float floatView = std::get<float>(data); // This will throw because data holds an int, not a float
} catch (const std::bad_variant_access&) {
    std::cout << "Type mismatch error!" << std::endl;
}
```

In the `std::variant` example, attempting to access the `int` as a `float` results in an exception,
thus preventing type punning and ensuring compliance with the strict aliasing rule.
This makes `std::variant` a safer alternative to unions for type punning.



# Runtimepolymorphism but dispatch table is created at compile time
Great observation! The variant/visitor pattern indeed creates a dispatch table at compile time, which is an essential aspect of how it works efficiently. However, it's still considered runtime polymorphism because the actual type stored in the variant is determined at runtime, and the corresponding function to handle that type is invoked based on the runtime type.

Here's a deeper dive into why this is still considered runtime polymorphism:

### Compile-Time and Runtime Aspects

1. **Compile-Time Preparation**:
   - The dispatch table (mapping of types to functions) is created at compile time using template metaprogramming. This ensures that the appropriate functions for all possible types are generated and available.

2. **Runtime Type Resolution**:
   - At runtime, the `std::variant` holds a specific type, which is determined dynamically when the program executes.
   - `std::visit` checks the active type stored in the variant at runtime and uses the pre-generated dispatch table to call the appropriate function.

### Key Points of Runtime Polymorphism:

- **Dynamic Behavior**: The actual type contained in the `std::variant` and the corresponding visitor function that gets called are determined at runtime, hence the behavior can change dynamically based on the actual data.
- **Type Safety**: Despite the dynamic nature, the type safety is ensured at compile time, preventing invalid type access.
- **No Virtual Tables or Pointers**: By avoiding inheritance and virtual functions, you eliminate the overhead associated with virtual tables and pointers while still achieving dynamic behavior.

### Why It’s Still Runtime Polymorphism:

- **Flexibility**: The program can handle different types stored in the variant dynamically, which is a hallmark of runtime polymorphism.
- **Efficiency**: Using a dispatch table created at compile time means the program runs efficiently, but the decision of which function to call is made based on the type stored in the variant at runtime.

### Simple English Analogy:

Think of a `std::variant` as a box that can hold any one of several types of objects. The specific type of object in the box isn't known until you look inside (at runtime). The visitor is like a set of labeled tools designed for each type of object. The `std::visit` function opens the box, sees what's inside, and picks the right tool to handle it based on the labels prepared in advance (at compile time).

This combination allows the flexibility and dynamic nature of runtime polymorphism without the overhead of virtual function calls.



# Psedo implementation of Variant/Visit :
### Complete Pseudo Code:

1. **Define Shape Types**:
    ```
    struct Circle {
        double radius;
    };

    struct Square {
        double side;
    };

    struct Triangle {
        double base;
        double height;
    };
    ```

2. **Define Variant to Hold Shapes**:
    ```
    Variant Shape {
        Circle, Square, Triangle
    }
    ```

3. **Define Visitor Struct**:
    ```
    struct AreaVisitor {
        function visit(Circle c) {
            return 3.14 * c.radius * c.radius;
        }

        function visit(Square s) {
            return s.side * s.side;
        }

        function visit(Triangle t) {
            return 0.5 * t.base * t.height;
        }
    }
    ```

4. **Union Declaration**:
    - Define a union that can store any of the shape types.
    ```
    union ShapeUnion {
        Circle circle;
        Square square;
        Triangle triangle;
    }
    ```

5. **Variant Class**:
    - Define the variant class that uses the union to store the value and an index to keep track of the active type.
    ```
    class Variant {
        int index;      // Type index (e.g., 0 for Circle, 1 for Square, etc.)
        ShapeUnion value; // Union to hold different shapes

        function set(Circle c) {
            value.circle = c;
            index = 0;
        }

        function set(Square s) {
            value.square = s;
            index = 1;
        }

        function set(Triangle t) {
            value.triangle = t;
            index = 2;
        }

        function getIndex() {
            return index;
        }

        function getValue() {
            return value;
        }
    }
    ```

6. **Create Dispatch Table**:
    - At compile-time, create a table that maps the variant type index to the corresponding visitor function.
    ```
    DispatchTable = {
        [0] = AreaVisitor.visit(Circle),    // Maps index 0 to Circle visitor function
        [1] = AreaVisitor.visit(Square),    // Maps index 1 to Square visitor function
        [2] = AreaVisitor.visit(Triangle)   // Maps index 2 to Triangle visitor function
    }
    ```

7. **Use `std::visit` to Apply Visitor**:
    - Determine the type at runtime and call the corresponding visitor function from the dispatch table.
    ```
    function std_visit(visitor, variant) {
        index = variant.getIndex();             // Get the active type index
        value = variant.getValue();             // Get the stored value
        dispatchFunction = DispatchTable[index]; // Retrieve the visitor function from dispatch table
        return dispatchFunction(visitor, value); // Call the visitor function with the value
    }
    ```

8. **Calculate Area Using Visitor**:
    - Create instances of shapes and calculate the area using the visitor.
    ```
    Shape shape1 = Variant();
    shape1.set(Circle(10.0));

    Shape shape2 = Variant();
    shape2.set(Square(5.0));

    Shape shape3 = Variant();
    shape3.set(Triangle(6.0, 4.0));

    AreaVisitor visitor;

    area1 = std_visit(visitor, shape1);
    area2 = std_visit(visitor, shape2);
    area3 = std_visit(visitor, shape3);

    print("Area of Circle: ", area1);
    print("Area of Square: ", area2);
    print("Area of Triangle: ", area3);
    ```

### Explanation of Type Safety:

1. **Union to Store Values**:
    - The union (`ShapeUnion`) can hold any of the shape types (Circle, Square, Triangle). Only one value can be active at any given time.

2. **Index to Track Type**:
    - The variant class uses an `index` to keep track of the currently active type in the union. This ensures type safety, as the `index` value indicates which member of the union is currently valid.

3. **Set Functions**:
    - The `set` functions update the union with a specific shape type and set the corresponding index. This ensures that only the correct type is stored and accessed.

4. **Dispatch Table**:
    - The dispatch table is created at compile time, mapping each type index to the corresponding visitor function. This table ensures that the correct function is called based on the active type stored in the union.

By following this approach, we achieve efficient runtime polymorphism with type safety, avoiding the overhead of virtual tables and pointers.
