# Error handling using error codes, error conditions and error categories.
## Purpose and Design Perspective
The `std::error_category` is designed to encapsulate the concept of an error domain.
This is useful when you want to represent errors without throwing exceptions, which is especially important in library design or in scenarios where exception safety is a concern.
It allows for a more controlled error handling strategy and can be used to create cross-platform error reporting mechanisms.

Instead of throwing a custom exception, you might use `std::error_category` and `std::error_code` when:
- You need to report errors from destructors or noexcept functions where throwing exceptions is not possible.
- You want to avoid the overhead of exception handling.
- You're interfacing with C libraries that use error codes.
- You require more control over the error handling process than what exceptions provide.

In summary, `std::error_category` and related classes provide a robust framework for error handling that complements the exception mechanism in C++.
It's particularly useful for library authors and applications that need precise control over error representation and handling.

## Error codes:
The `std::error_code` class in C++ is used to represent error conditions in a system-independent manner.
  - The `std::error_code` class, along with other error handling utilities like `std::error_condition` and `std::error_category`, was introduced in C++11 as part of the `<system_error>` header.
    This provided a standardized way to represent and handle system-level errors in C++.


Here are the return types and explanations for the member functions you've listed:

  - `assign(int, const error_category&)` : **void**                       - This function assigns an error value and a category to the `error_code`.
  - `clear()`                            : **void**                       - Resets the `error_code` to a state that indicates success (no error).
  - `value()`                            : **int**                        - Returns the error value as an integer.
  - `category()`                         : **const std::error_category&** - Returns a reference to the `error_category` associated with the error code.
  - `message()`                          : **std::string**                - Provides a human-readable description of the error.
  - `default_error_condition()`          : **std::error_condition**       - Converts an `error_code` to a more generic `error_condition` that can be compared across system boundaries.

## Error categories:
are classes derived from `std::error_category` and are used to categorize errors.
They provide context to the error codes.
For example, `std::system_category()` is used for system-related errors, while `std::iostream_category()` is used for stream-related errors.
To set a human-readable message, you typically override the `message()` function in a custom error category class.
The `std::error_category` class in C++ is a part of the error handling mechanism provided by the `<system_error>` header.
It serves as a base class for error category types and is used to define error codes and corresponding error messages in a platform-independent way.
A detailed look at its members and usage:

### Members of `std::error_category`
- **name()**                                             : Returns a string that identifies the error category. It's a pure virtual function, so each derived class must provide its own implementation.
- **message(int)**                                       : Returns a string with a message describing the error condition for the given error code. It's also a pure virtual function.
- **default_error_condition(int) const**                 : Makes a `std::error_condition` object that is equivalent to the error code. This is used for comparing error codes from different categories.
- **equivalent(int, const std::error_condition&) const** : Checks whether a given error code is equivalent to a specific error condition.
- **equivalent(const std::error_code&, int) const**      : Checks whether a given error code is equivalent to another error code.

### Usage and Examples
```cpp
#include <system_error>
#include <iostream>

// Custom error category
class MyErrorCategory : public std::error_category {
public:
  const char* name() const noexcept override {
    return "MyCustomError";
  }

  std::string message(int ev) const override {
    switch (ev) {
      case 1: return "First error";
      case 2: return "Second error";
      default: return "Unknown error";
    }
  }
};

const MyErrorCategory myErrorCategory{};

// Usage
std::error_code ec(1, myErrorCategory);
std::cout << ec.message() << std::endl; // Outputs: First error
```

## Error conditions:
`std::error_condition` are a higher-level, generic representation of errors that can be used to compare errors across different categories.
They are useful for portable error handling.

**Default and generic error conditions** refer to a standardized set of error conditions that can be used across different platforms.
The `default_error_condition()` function maps an error code to one of these standard conditions.
The `std::generic_category()` represents the category for these generic conditions, which are often based on POSIX `errno` values.

An example of using `default_error_condition()`:

```cpp
std::error_code ec(ENOMEM, std::system_category());
std::error_condition ecnd = ec.default_error_condition();
if (ecnd == std::errc::not_enough_memory) {
  // Handle not enough memory error
}
```

## Difference and Relationship between `std::error_condition` and `std::error_category`:
`std::error_condition` and `std::error_category` are both part of the error handling facilities provided by the C++ standard library,
specifically for representing and managing error conditions in a platform-independent way.

### Difference between `std::error_condition` and `std::error_category`:
- **`std::error_condition`**:
  - It is a class that represents a portable error condition, which is intended to be used for comparing errors across different systems or libraries.
  - It is associated with `std::error_category` but is not tied to any specific system or library error codes.
  - It holds an integer value and a reference to an `std::error_category` object, which together represent an error condition.

- **`std::error_category`**:
  - It is an abstract base class that defines a category of errors.
    It is used to create specific error categories, such as `std::system_category` or `std::iostream_category`.
  - Derived classes from `std::error_category` must implement the `name()` and `message()` virtual functions to provide a string that identifies the category and
    to return a text message for a given error code, respectively.
  - It also provides the `default_error_condition()` method, which is used to map an `std::error_code` to a `std::error_condition` that can be compared across different systems.

### Relationship between `std::error_condition` and `std::error_category`:
- An `std::error_condition` is created with a specific `std::error_category`, and it uses this category to interpret its integer value as a portable error condition.
- The `std::error_category` provides context and meaning to the integer value held by an `std::error_condition`.

### Examples:

```cpp
#include <system_error>
#include <iostream>

int main() {
  // Create an error_code with a specific value and category
  std::error_code ec = make_error_code(std::errc::not_supported);

  // Create an error_condition from the error_code
  std::error_condition ecnd = ec.default_error_condition();

  // Check if the error_condition represents a specific error
  if (ecnd == std::errc::not_supported) {
    std::cout << "Operation not supported." << std::endl;
  }

  return 0;
}
```

In this example, `make_error_code` creates an `std::error_code` object with a specific error condition (`std::errc::not_supported`) and the generic category.
The `default_error_condition()` method is then used to create an `std::error_condition` that can be compared against other `std::error_condition` objects, even if they come from different error categories.

In summary, `std::error_category` is used to define categories of errors and provide context for error codes,
while `std::error_condition` is used to represent portable error conditions that can be compared across different systems or libraries.
They work together to allow for robust and portable error handling in C++ applications.

# Comparison of `std::optional`, `std::exception`, and `boost::outcome` in terms of their purpose, design, usage, and appropriate contexts for use.

### `std::optional`
- **Purpose**         : Provides a way to represent 'optional' values; a value that may or may not be present.
- **Design**          : A wrapper that contains either an instance of a type `T` or nothing.
- **Usage**           : Used when a function might not return a value or when a class member is not always required.
- **When to Use**     : When you need to indicate the absence of a value without using pointers or special markers.
- **When Not to Use** : When you always expect a value to be returned or when using it would complicate the code unnecessarily.

### `std::exception`
- **Purpose**         : The base class for all exceptions thrown by the standard library, and a type that user-defined exceptions can inherit from.
- **Design**          : Contains a virtual function `what()` that returns a C-style string describing the error.
- **Usage**           : Thrown to signal the occurrence of an error and caught to handle these errors.
- **When to Use**     : When you need to signal and handle error conditions, especially those that cannot be resolved locally.
- **When Not to Use** : When error conditions can be handled without disrupting the normal flow of the program, or when exceptions would introduce performance overhead.

### `std::expected` or `boost::outcome`
- **Purpose**         : Designed to report the outcome of functions that may return a result, an error, or an exception.
- **Design**          : Provides `result<T>` and `outcome<T>` types that can contain a value, an error, or an exception pointer.
- **Usage**           : Used in scenarios where you need to handle multiple potential outcomes of a function call.
- **When to Use**     : When you want to avoid exceptions but still need to handle complex error states that may include exceptions.
- **When Not to Use** : When the overhead of handling multiple outcomes is unnecessary, or when a simple `std::optional` or error code would suffice.

In summary,
`std::optional` is for situations where a value might not be present,
`std::exception` is for error handling via exceptions, and
`boost::outcome` is a more complex tool that handles both error codes and exceptions, providing more control over function outcomes.
Choose the one that best fits the error handling strategy and performance requirements of your application.

# `std::expected` vs `boost::outcome`:
## Comparison of `std::expected` and `boost::outcome`:
### `std::expected<T, E>`:
    is a type that may contain a value of type `T` or an error of type `E`.
    It's similar to `std::optional` but with the added ability to hold an error.
    It was standardized in C++23.
### `boost::outcome`:
    `boost::outcome` provides `result<T, E>` and `outcome<T, E, P>` types.
    `result` is used for functions that may return a value or an error, while
    `outcome` is for functions that may additionally return an exception pointer.
    It's more flexible and can be configured to behave like `std::expected` if desired.

#### `boost::outcome` and Exception Pointers:
`boost::outcome` can handle scenarios where a function might not only fail but also throw an exception that is not caught within the function itself.
The `outcome::outcome<T, E, P>` type can store an exception pointer (`std::exception_ptr`) in addition to a value or an error code.
This is particularly useful in asynchronous code where exceptions need to be transported across threads or task boundaries.

A simplified example of how `boost::outcome` can be used with an exception pointer:

```cpp
#include <boost/outcome.hpp>

namespace outcome = BOOST_OUTCOME_V2_NAMESPACE;

outcome::outcome<int> FunctionThatMightThrow() {
  try {
    // Code that may throw an exception
    throw std::runtime_error("An error occurred");
  } catch (...) {
    return std::current_exception(); // Capture the exception pointer
  }
}

// Usage
auto result = FunctionThatMightThrow();
if (!result) {
  if (result.has_exception()) {
    try {
      std::rethrow_exception(result.exception()); // Handle the exception
    } catch (const std::exception& e) {
      std::cerr << "Caught exception: " << e.what() << std::endl;
    }
  }
}
```

In this example, `FunctionThatMightThrow` attempts to execute some code that may throw an exception.
If an exception is thrown, it is caught and stored as an `std::exception_ptr` in the `outcome`.
The caller can then check if the `outcome` contains an exception and handle it appropriately.

## Design Under the Hood:
   - `std::expected<T, E>` is designed as a constrained variant type, preferring the successful type `T` and providing an 'unexpected' `E` value otherwise.
     It's efficient in terms of runtime overhead and code generation bloat, but its variant storage can impact build times.
   - `boost::outcome` uses a struct-based storage for `result` and `outcome` types, which is C-compatible if `T` and `E` are C-compatible.
     It only uses union storage when both `T` and `E` are trivially copyable or move bitcopying.
     Otherwise, struct storage is used, making `boost::outcome` types larger than `std::expected`'s.

## Usage of `std::error_code`:
`std::error_code` can be used with both `std::expected` and `boost::outcome`.
In `std::expected<T, E>`, `E` can be `std::error_code`, allowing you to return either a value of type `T` or an error code.
Similarly, `boost::outcome`'s `result<T, E>` can use `std::error_code` as the error type, enabling functions to return a value or an error code.

Example to use `std::error_code` with `std::expected`:

```cpp
#include <system_error>
#include <expected> // Assuming the expected header is available

std::expected<int, std::error_code> FunctionThatMightFail() {
  if (/* success condition */) {
    return 42; // Return a value
  } else {
    return std::make_error_code(std::errc::operation_not_permitted); // Return an error code
  }
}
```

And with `boost::outcome`:

```cpp
#include <boost/outcome.hpp>

namespace outcome = BOOST_OUTCOME_V2_NAMESPACE;

outcome::result<int> FunctionThatMightFail() {
  if (/* success condition */) {
    return 42; // Return a value
  } else {
    return std::make_error_code(std::errc::operation_not_permitted); // Return an error code
  }
}
```

In both cases, `std::error_code` is used to represent the error state, providing a standardized way to handle errors without exceptions.

