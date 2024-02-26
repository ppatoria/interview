# Operator overloading
## Question: Should prefer making operators like == and < non-member functions?
### Answer: Yes
This can improve code clarity and enable more flexible type conversions.

When you make operators like `==` and `<` non-member functions, you allow for more flexible type conversions.
This is because non-member functions do not have a `this` pointer and therefore do not have a preferred argument type.
This means that both arguments of the operator are treated equally, and conversions can be applied to either argument to best match the function signature.

Let's consider a real-world scenario.
Suppose you have a `Fraction` class and you want to compare a `Fraction` object with an `int`.
If you implement `==` as a member function, the left-hand side of the operator must be a `Fraction` object, and the right-hand side must match the type of the argument to the member function.
This means you can write `fraction == integer`, but not `integer == fraction`.

However, if you implement `==` as a non-member function, you can provide two versions:
    - one that takes a `Fraction` and an `int`.
    - Another that takes an `int` and a `Fraction`.
This allows you to write both `fraction == integer` and `integer == fraction`.

#### Examples:

1. Franction:

```cpp
#include <iostream>

class Fraction {
    int numerator;
    int denominator;
public:
    Fraction(int num, int denom) : numerator(num), denominator(denom) {}
    friend bool operator==(const Fraction& f, int value);
    friend bool operator==(int value, const Fraction& f);
};

bool operator==(const Fraction& f, int value ) { return f.numerator == value * f.denominator; }
bool operator==(int value, const Fraction& f ) { return value == f.numerator / f.denominator; }

int main() {
    Fraction fraction(2, 1);
    if (fraction == 2) {
        std::cout << "fraction equals 2" << std::endl;
    }
    if (2 == fraction) {
        std::cout << "2 equals fraction" << std::endl;
    }
    return 0;
}
```

In this example, both `fraction == 2` and `2 == fraction` are valid and produce the expected output.
This wouldn't be possible if `operator==` were implemented as a member function of `Fraction`.
This is why making operators like `==` and `<` non-member functions can provide more flexibility.
It allows for more intuitive usage of the class and can make the code easier to read and understand.

2. Custome Double class:

Let's consider a `CustomDouble` class that could be used in a trading system.
This class will encapsulate a `double` value and provide conversions to and from `int` and `std::string`.
It will also provide `operator==` and `operator<` for comparison.

In this example, `Double` objects can be compared using `==` and `<`, and can be converted to `int` and `std::string`. This allows for flexible usage of `Double` objects in a trading system, where different types of comparisons and conversions might be needed. The `main` function demonstrates these capabilities. This wouldn't be possible if `operator==` and `operator<` were implemented as member functions of `Double`. This is why making operators like `==` and `<` non-member functions can provide more flexibility. It allows for more intuitive usage of the class and can make the code easier to read and understand.


```cpp

#include <iostream>
#include <sstream>

class Double {
    double value;
public:
    Double (double val) : value(val)   {}

    operator int         () const { return static_cast<int>(value); }
    operator std::string () const { std::ostringstream strs; strs << value; return strs.str(); }

    bool operator== (const Double& other) const { return value == other.value; }
    bool operator<  (const Double& other) const { return value <  other.value; }

    friend bool operator== (const Double& a , const Double& b );
    friend bool operator<  (const Double& a , const Double& b );

    friend bool operator== (const Double& a , double b        );
    friend bool operator== (double a        , const Double& b );

    friend bool operator<  (const Double& a , double b        );
    friend bool operator<  (double a        , const Double& b );

    friend bool operator== (const Double& a , int b           );
    friend bool operator== (int a           , const Double& b );

    friend bool operator<  (const Double& a , int b           );
    friend bool operator<  (int a           , const Double& b );
};

bool operator== (const Double& a , const Double& b) { return a.value  == b.value; }
bool operator<  (const Double& a , const Double& b) { return a.value  <  b.value; }

bool operator== (const Double& a , double b)        { return a.value  == b;       }
bool operator== (double a        , const Double& b) { return a        == b.value; }

bool operator<  (const Double& a , double b)        { return a.value  < b;        }
bool operator<  (double a        , const Double& b) { return a        < b.value;  }

bool operator== (const Double& a , int b)           { return a.value  == b;       }
bool operator== (int a           , const Double& b) { return a        == b.value; }

bool operator<  (const Double& a , int b)           { return a.value  < b;        }
bool operator<  (int a           , const Double& b) { return a        < b.value;  }

int main() {
    Double a(2.5);
    double b = 3.5;
    int c = 2;

    if (a == b) {
        std::cout << "a equals b" << std::endl;
    } else {
        std::cout << "a does not equal b" << std::endl;
    }

    if (a < b) {
        std::cout << "a is less than b" << std::endl;
    } else {
        std::cout << "a is not less than b" << std::endl;
    }

    if (a == c) {
        std::cout << "a equals c" << std::endl;
    } else {
        std::cout << "a does not equal c" << std::endl;
    }

    if (a < c) {
        std::cout << "a is less than c" << std::endl;
    } else {
        std::cout << "a is not less than c" << std::endl;
    }

    return 0;
}
```

In this example, `Double` objects can be compared with `double` and `int` using `==` and `<`. This allows for flexible usage of `Double` objects in a trading system, where different types of comparisons might be needed. The `main` function demonstrates these capabilities. This wouldn't be possible if `operator==` and `operator<` were implemented as member functions of `Double`. This is why making operators like `==` and `<` non-member functions can provide more flexibility. It allows for more intuitive usage of the class and can make the code easier to read and understand.




