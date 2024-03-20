# The `std::format` feature introduced in C++20 and how to use it to replace the `operator<<` and c-stype printf.

The `std::format` function is a new feature in C++20 that provides a safe and extensible way to format text.

The `operator<<` for output in C++ is not inherently unsafe, but it can lead to issues in certain scenarios, which are addressed by `std::format`:

## Type Safety:
    With `operator<<`, there's no check to ensure that the type of the argument matches the type expected by the format specifier.
    This can lead to runtime errors.
    On the other hand, `std::format` is type-safe.
    It checks at compile-time that the type of each argument matches the type expected by the corresponding format specifier.
    The `operator<<` is type-safe in the sense that it will only compile if the type can be sent to the stream.
    but the type safety issue referred above is more about the mismatch between the format specifier and the argument type,
    which is a problem in C-style `printf` functions, not `operator<<`.

    In C-style `printf` functions, you could have a situation like this:

    ```c
    int main() {
        int i = 10;
        printf("%s\n", i);  // This will compile but cause undefined behavior at runtime
    }
    ```

    In this case, the format specifier `%s` expects a string, but an integer is provided. 
    This will compile, but it will cause undefined behavior at runtime.

    In contrast, both `operator<<` and `std::format` are type-safe and would catch such a mismatch at compile time.
    For example, the equivalent code using `std::format` would look like this:

    ```cpp
    int main() {
        int i = 10;
        std::cout << std::format("{:s}\n", i);  // This will not compile
    }
    ```

    In this case, the `{:s}` format specifier expects a string, but an integer `i` is provided.
    This will cause a compile-time error, preventing the undefined behavior that could occur with `printf`.

## Readability and Maintainability:
    When formatting complex strings with `operator<<`, the code can become difficult to read and maintain because the format specifiers are interspersed with the `<<` operators.
    `std::format` improves readability by keeping the format string separate from the arguments.
### Example:
    let's consider an example where we want to print a complex string that includes several variables.

    Here's how you might do it with `operator<<`:

    ```cpp
    std::string name = "Alice";
    int age = 25;
    double height = 1.68;  // in meters

    std::cout << "Name: " << name << ", Age: " << age << ", Height: " << height << " meters\n";
    ```

    As you can see, the format specifiers (the strings like `"Name: "`, `", Age: "`, etc.) are interspersed with the `<<` operators and the variables.
    This can become difficult to read and maintain, especially for more complex strings.

    Now, let's see how we can do the same thing with `std::format`:

    ```cpp
    std::string name = "Alice";
    int age = 25;
    double height = 1.68;  // in meters

    std::cout << std::format("Name: {}, Age: {}, Height: {} meters\n", name, age, height);
    ```

    With `std::format`, the format string is separate from the arguments, which makes the code easier to read and maintain.
    The placeholders `{}` in the format string are replaced by the arguments in the order they are provided.
    This is especially helpful when dealing with complex format strings and multiple variables.
    It also makes it easier to change the format string without having to modify the rest of the code.

## Localization:
    `std::format` supports localization, which is the process of translating your program's user interface into different languages.
    This is not directly supported by `operator<<`.

## Extensibility:
    `std::format` is extensible. You can define how your own types are formatted by specializing the `std::formatter` template.
    This is not possible with `operator<<` without overloading the operator for your type.
    ```c++
    #include <format>
    #include <iostream>

    struct Point {
        int x;
        int y;
    };

    template<>
    struct std::formatter<Point> {
        auto parse(format_parse_context& ctx) {
            return ctx.begin();
        }

        auto format(const Point& p, format_context& ctx) {
            return format_to(ctx.out(), "({}, {})", p.x, p.y);
        }
    };

    int main() {
        Point p {10, 20};
        std::cout << std::format("p = {}\n", p);
    }
    ```
    So, while `operator<<` is not unsafe per se, `std::format` provides several improvements that make it a safer and more flexible choice for text formatting in C++.
    It's a powerful tool that can be used to replace the `printf` family of functions⁶ and is based on Python’s string formatting specification⁷.
