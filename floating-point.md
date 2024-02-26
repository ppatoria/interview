# How floating point numbers are stored in memory?
    Floating point numbers are stored in memory using the IEEE 754 standard.
    This standard represents a number in three parts: the sign, the exponent, and the fraction (also known as the significand or the mantissa).
    The layout of a single-precision floating point number is as follows:
    - Sign bit: 1  bit
    - Exponent: 8  bits
    - Fraction: 23 bits

# Floating-point representation:
    Computers represent real numbers as floating-point numbers, which have a finite precision.
    This means that some real numbers can't be represented exactly, leading to small rounding errors.
    For example, the number 0.1 can't be represented exactly as a binary floating-point number.

# Single vs double precision and their datatypes in C++:
    In C++, `float` and `double` are used to represent floating point numbers.
    `float` is a single-precision floating point type that uses 32 bits of memory,
    while `double` is a double-precision floating point type that uses 64 bits of memory.
    Double precision means higher range and more precision.

# Epsilon and its uses:
    Epsilon refers to the difference between 1 and the least value greater than 1 that is representable.
    For floating point numbers in C++, this value is provided by `std::numeric_limits<float>::epsilon()` for `float` and `std::numeric_limits<double>::epsilon()` for `double`.
    It's often used to compare floating point numbers for equality with a certain amount of error.
## std::numeric_limits<double>::epsilon:
    This is a function in C++ that returns the smallest positive `double` value that can be added to 1.0 to produce a value different from 1.0.
    This value is very small, close to zero.
    It's often used to compare two floating-point numbers for equality.

# Comparing floating-point numbers:
    Due to these rounding errors, it's usually not a good idea to compare floating-point numbers for exact equality.
    Instead, we often consider two floating-point numbers equal if their difference is less than some small number, like `std::numeric_limits<double>::epsilon`.

# Things to take care with floating point calculation and comparison:
    - Be aware of precision errors.
      Due to the way they're stored, some floating point numbers might not have an exact representation.
    - Comparing two floating point numbers for equality directly is usually not a good idea.
      Instead, check if the absolute difference is within a small tolerance.
    - Be careful with operations that can cause overflow or underflow.

## Class "Double" which is a wrapper over double type:
   Here's a simple implementation of a class that wraps a `double`:

  ```cpp
  #include <string>
  #include <cmath>

  class Double {
  private:
      double value;
      double epsilon = std::numeric_limits<double>::epsilon();

  public:
      Double(double val) : value(val) {}

      // Set precision
      void setPrecision(int precision) {
          value = std::round(value * std::pow(10, precision)) / std::pow(10, precision);
      }

      // Safe comparison
      bool isEqual(Double &other) {
          return std::abs(value - other.value) <= epsilon;
      }

      // Safe calculation
      Double add(Double &other) {
          return Double(value + other.value);
      }

      // Convert to int
      int toInt() {
          return static_cast<int>(value);
      }

      // Convert to string
      std::string toString() {
          return std::to_string(value);
      }
  };
  ```
  This class provides methods for setting precision, comparing two `Double` objects, adding two `Double` objects, and converting a `Double` to an `int` or a `std::string`.
  Note that this is a very basic implementation and might not cover all edge cases or requirements.

### Setting Precision:
    The `setPrecision` function is designed to round the `value` of the `Double` object to a specified number of decimal places. Here's how it works:

    - `std::pow(10, precision)` calculates 10 raised to the power of `precision`. This effectively moves the decimal point in `value` to the right by `precision` places.
    - `value * std::pow(10, precision)` multiplies `value` by this power of 10, moving the decimal point as described.
    - `std::round(value * std::pow(10, precision))` rounds this result to the nearest whole number. This has the effect of rounding `value` to `precision` decimal places.
    - Dividing by `std::pow(10, precision)` then moves the decimal point back to its original position, leaving us with `value` rounded to `precision` decimal places.

    Let's walk through an example.
    Suppose we have a `Double` object with a `value` of `123.456789` and we want to set the precision to `3` decimal places.

    Here's how the `setPrecision` function would work:

    - Calculate `std::pow(10, precision)`: This is `10` raised to the power of `3`, which equals `1000`.
    - Multiply `value` by this power of `10`: `123.456789 * 1000` equals `123456.789`.
    - Round this result to the nearest whole number: `std::round(123456.789)` equals `123457`.
    - Divide by `std::pow(10, precision)`: `123457 / 1000` equals `123.457`.

    So, the `value` of the `Double` object is now `123.457`, rounded to `3` decimal places.

#### std::setprecision:
    As for `std::setprecision`, it's a manipulator used with output streams (like `std::cout`) to control the number of digits displayed when outputting floating-point numbers.
    It doesn't actually change the value of the number itself, just how it's displayed.
    In contrast, the `setPrecision` function in the `Double` class is designed to actually change the `value` stored in the object.
    That's why `std::setprecision` wasn't used in this case.

# Question: When comparing doubles, consider any two values within std::numeric_limits<double>::epsilon of each other to be equal ?
## Answer: Correct.
    Due to the nature of floating-point representation, it’s often a good idea to consider two floating-point numbers equal if they are close enough, rather than expecting them to be exactly equal.

Here's an example of how to use `std::numeric_limits<double>::epsilon` to compare two `double` values:

```cpp
#include <iostream>
#include <cmath>
#include <limits>

bool areEqual(double a, double b, double epsilon = std::numeric_limits<double>::epsilon()) {
    return std::abs(a - b) <= epsilon * std::abs(a);
}

int main() {
    double a = 0.1 + 0.2;
    double b = 0.3;

    if (areEqual(a, b)) {
        std::cout << "a and b are equal.\n";
    } else {
        std::cout << "a and b are not equal.\n";
    }

    return 0;
}
```
Let's break down the `areEqual` function and how it works with the values of `a` and `b` provided in `main`.

The `areEqual` function is designed to compare two `double` values for equality, taking into account the precision issues that can arise with floating point numbers.
It does this by checking if the absolute difference between the two numbers is less than or equal to `epsilon` times the absolute value of the first number.

Here's how it works with the given values of `a` and `b`:

1. In `main`, `a` is assigned the value of `0.1 + 0.2`, which due to precision issues with floating point arithmetic, is not exactly `0.3`.

2. `b` is assigned the value `0.3`.

3. When `areEqual(a, b)` is called, it calculates `std::abs(a - b)`, which is the absolute difference between `a` and `b`. Due to the aforementioned precision issues, this is not exactly `0`.

4. It then calculates `epsilon * std::abs(a)`. `epsilon` is the smallest value such that `1.0 + epsilon != 1.0`. Multiplying it by `std::abs(a)` scales the allowable difference based on the magnitude of `a`.

5. If the absolute difference between `a` and `b` is less than or equal to this scaled epsilon, the function returns `true`, indicating that `a` and `b` are considered equal within the precision limits of `double`. Otherwise, it returns `false`.

In this specific case, even though `0.1 + 0.2` is not exactly equal to `0.3` due to floating point precision issues, the `areEqual` function will likely consider them equal because the difference is within the allowable tolerance.

This is a common way to compare floating-point numbers for equality.
