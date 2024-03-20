# `std::string_view`.

`std::string_view` is a lightweight, non-owning reference to a string, introduced in C++17.
It's intended to be used as a parameter type for functions that need to take a read-only string as input.

Here's how it works:

```cpp
std::string str = "Hello, world!";
std::string_view sv = str;
```

In this example, `sv` is a view into the string `str`.
It doesn't own the string data; it's just a reference to the existing string.

## Here are some scenarios where `std::string_view` can be useful:

### Performance:
    If you're **passing a string to a function** that doesn't need to modify it, using `std::string_view` can avoid potentially expensive copies.

### Substrings:
    `std::string_view` can refer to a substring of a `std::string` without having to create a new string.

###  Compatibility:
    `std::string_view` can be constructed from both `std::string` and C-style strings,
    making it a versatile choice for functions that need to accept both types of strings.

## However, there are also scenarios where `std::string_view` should be used with caution:

### Lifetime:
    Because `std::string_view` doesn't own its data, you need to be careful about the lifetime of the underlying string.
    If the original string is destroyed, the `std::string_view` will be left dangling.

### Non-null terminated:
    `std::string_view` is not guaranteed to be null-terminated.
    If you need a null-terminated string, you should use `std::string`.

### Mutability:
    `std::string_view` is non-mutating.
    If you need to modify the string, you should use `std::string`.

In general, `std::string_view` is a powerful tool for optimizing string handling in C++, but it needs to be used with care to avoid dangling references and other potential issues.


## **When to return `std::string_view`:

### Performance:
    If the string data already exists and won't be modified or destroyed while the `std::string_view` is in use,
    returning a `std::string_view` can avoid the cost of copying the string.

### Substrings:
    If you're returning a substring of an existing string and the original string will outlive the `std::string_view`,
    `std::string_view` can be a good choice because it avoids the need to create a new string.

### Read-Only:
    If the function is only meant to provide read-only access to a string, `std::string_view` can make this contract explicit.

## When not to return `std::string_view`:

### Ownership:
    If the function needs to return a new string that it creates,
    or if the original string might be destroyed while the returned value is still in use,
    you should return a `std::string` to ensure the data remains valid.

### Mutability:
    If the caller might need to modify the string,
    you should return a `std::string`,
    since `std::string_view` is non-modifying.

### API Consistency:
    If your function is part of an API where other functions return `std::string`,
    it might be more consistent and less confusing to users of your API to also return `std::string`.

Remember, the key difference between `std::string` and `std::string_view` is ownership.
`std::string` owns its data and ensures it remains valid for the lifetime of the string,
while `std::string_view` is a non-owning reference to a string.
The choice between them depends on the specific use case and the lifetime of the string data.
