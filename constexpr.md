# Constexpr always ensures something is evaluated at compile time ?
This is not always true.
A constexpr function or variable may be evaluated at compile time, but it can also be evaluated at runtime if its value is not known at compile time.

**Example***:

```cpp
constexpr int square(int num) {
    return num * num;
}

int main() {
    constexpr int sq = square(5); // This will be evaluated at compile time

    constexpr int x = 5;
    int sq = square(x);           // This will be evaluated at compile time

    constexpr int x = 5;
    constexpr int sq = square(x); // This will be evaluated at compile time

    const int x = 5;
    const int sq = square(x);     // This will be evaluated at compile time

    const int x = 5;
    int sq = square(x);           // This can be evaluated at compile time or runtime

    const int x = 5;
    constexpr int sq = square(x); // This will be evaluated at compile time

    int x = 5;
    int sq = square(x);           // This will be evaluated at runtime

    return 0;
**
```

In this code, the `square` function is declared as `constexpr`, which means it can be evaluated at compile time.
However, whether it's actually evaluated at compile time or runtime depends on the context.

## {c++ constexpr int sq = square(5);} : is evaluated at compile time.
  This is because `5` is a literal constant, and its value is known at compile time.
  Therefore, the compiler can compute `square(5)` at compile time and replace `sq` with the result, `25`.

## {constexpr int x=5; int sq = square(x);} : is evaluated at runtime.
  While it's true that in this specific code snippet the value of `x` is known at compile time, the compiler treats `x` as a runtime variable because in general, variables can be modified at runtime.
  This is because `x` is a variable, and its value could potentially change at runtime.
  Therefore, the compiler cannot compute `square(x)` at compile time, and the computation must be done at runtime.
  The `constexpr` specifier in C++ instructs the compiler to evaluate the expression at compile time if possible.
  However, it doesn't force the compiler to do so.
  If the expression includes runtime variables, the compiler will defer the evaluation to runtime.
  In the case of `int x = 5; int sq = square(x);`, even though `x` is initialized with a compile-time constant, it's still a non-const variable that could potentially be modified at runtime.
  Therefore, the compiler treats `square(x)` as a runtime expression.

## {constexpr int x = 5; constexpr int sq = square(x);} : is evaluated at compiletime.
  To ensure that an expression is evaluated at compile time, one should use `constexpr` variables.
  In this case, `x` is a `constexpr` variable, which means its value is fixed at compile time and cannot be changed at runtime.
  Therefore, `square(x)` can be evaluated at compile time as well.

## {const int x = 5; int sq_compile_time = square(x);} : is evaluated at compile time or runtime
  In this case, x is a const variable, which means its value is fixed at compile time and cannot be changed at runtime.
  Therefore, square(x) can be evaluated at compile time as well.
  However, the result of square(x) is stored in a non-constexpr variable `sq`,
  so the compiler doesn’t have to evaluate square(x) at compile time, and it might be evaluated at runtime depending on the compiler and its optimization settings.

## {const int x = 5; constexpr int sq = square(x);} : is evaluated at compile time.
  To ensure that square(x) is evaluated at compile time, one should store the result in a constexpr variable.
  In this case, sq is a constexpr variable, so its value must be known at compile time.
  Therefore, square(x) must be evaluated at compile time.

## {constexpr int x = 5; int sq = square(x);} : is evaluated at compile time.
  In this case, even though `sq` is not declared as constexpr, the expression square(x) will still be evaluated at compile time.
  This is because x is a constexpr, so its value is known at compile time, and square is a constexpr function, which means it can be evaluated at compile time when given compile-time constants as arguments.
  In this code, square(x) is evaluated at compile time, and its result (25) is assigned to `sq`.
  The compiler can do this optimization because it knows at compile time that the value of x is 5 and that the square function simply returns the square of its argument.
  However, please note that while modern compilers are often able to do this optimization, it’s not guaranteed by the C++ language standard.
  The standard only guarantees compile-time evaluation for constexpr variables.

## {const int x = 5; const int sq = square(x);} : is evaluated at compile time.
  This code will work as expected.
  Here, x is a const integer initialized with the value 5, and sq is another const integer that is initialized with the result of the square(x) function call.
  Because x is a const, its value is known at compile time, so the square(x) can also be evaluated at compile time.
  In this code, square(x) is evaluated at compile time, and its result (25) is assigned to sq_time.
  The compiler can do this optimization because it knows at compile time that the value of x is 5 and that the square function simply returns the square of its argument.
  However, please note that while modern compilers are often able to do this optimization, it’s not guaranteed by the C++ language standard.
  The standard only guarantees compile-time evaluation for constexpr variables.
  If you want to ensure that square(x) is evaluated at compile time, you should declare sq as constexpr.


Under the hood, when the compiler sees a `constexpr` function or variable, it tries to evaluate it at compile time.
If it can't (because the value isn't known until runtime), it generates code to compute the value at runtime, just like a regular function or variable.

The `constexpr` keyword is a hint to the compiler that the function or variable can be computed at compile time.
It doesn't guarantee that the computation will be done at compile time.
The actual behavior depends on the specific context and the capabilities of the compiler.
