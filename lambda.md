# The C++ Standard requires compilers to ensure lambdas never capture dangling references
This is not directly stated in the C++ Standard.
However, it’s the programmer’s responsibility to ensure that lambdas do not capture dangling references, as this would lead to undefined behavior.

An example of a lambda capturing a dangling reference:

```cpp
#include <iostream>
#include <functional>

std::function<int()> danglingReference() {
    int x = 5;
    auto lambda = [&x]() { return x; }; // lambda captures reference to x
    return lambda;                      // x goes out of scope here, so lambda now has a dangling reference
}

int main() {
    auto func = danglingReference();
    std::cout << func() << std::endl;   // Undefined behavior: accessing a dangling reference
    return 0;
}
```

In this code, the function `danglingReference` returns a lambda that captures a reference to a local variable `x`.
However, `x` goes out of scope when `danglingReference` returns, so the lambda ends up holding a reference to a destroyed object.
This is a dangling reference, and accessing it (as in `func()` in `main`) leads to undefined behavior.

To avoid this issue, you should ensure that any objects captured by reference in a lambda outlive the lambda itself.
If you can't guarantee this, you should capture the objects by value instead.
Here's a corrected version of the above code:

```cpp
#include <iostream>
#include <functional>

std::function<int()> noDanglingReference() {
    int x = 5;
    auto lambda = [x]() { return x; }; // lambda captures x by value
    return lambda;                     // x goes out of scope here, but lambda has its own copy
}

int main() {
    auto func = noDanglingReference();
    std::cout << func() << std::endl;  // No undefined behavior: lambda has its own copy of x
    return 0;
}
```

In this corrected code, the lambda captures `x` by value, so it has its own copy of `x` that isn't affected when the original `x` goes out of scope.
Therefore, there are no dangling references, and the code doesn't have undefined behavior.
