# Question: Bike ride hydration
A cyclist rides a bike on a long, straight bicycle path.
He has one water bottle- initially full.
With a full bottle, he can ride at most "range" kilometres before passing out from dehydration.
Please implement the C++20 function...
```c++
int min_refills(int start_pos, int finish_pos, int range, const
std::vector<int>& tap_positions)
...
```

such that it efficiently (in a big-O sense) calculates and returns the minimum number of times he'd need to refill his water bottle when riding from the "start" kilometre mark to the "finish" kilometre mark.
The kilometre positions of each tap where he can refill his bottle are provided in the "taps" argument.
If it's not possible to reach the intended finish mark, min_refills should return -1.
1. For example, a call to...
```c++
min_refills(10, 20, 4, {13, 17, 18, 22, 7 30})
8
9
10
11
...
```
models a desire to ride from the 10km mark and reach the 20km mark, an ability to ride 4 kms between refis, and taps at each of the 13, 17, 18, 22, and 30 kilometre marks.
Starting at the 10km mark, after refilling twice - at the 13km and 17km marks - he'll have enough water to reach his finish point at the 20km mark, so min_refills should return

2. Another examples:
min_refills(10, 25, 5, {13, 17, 18, 21, 22, 30}) should refill at 13, 17 or 18, 21 or 22kms => result 3.
NOTES
- all kilometre marks - start_pos, finish_pos and tap_positions (if any) - are in the range [0..1,000,000] inclusive - start <= finish, and tap_positions are distinct values in increasing order - range is in the range [1..1,000,000] km .
Please implement  the below min_refills function  in c++20 and it should be as efficient at it could be.
```c++
#include <bits/stdc++.h>

using namespace std;
int min_refills(int start_pos, int finish_pos, int range, const vector<int>& tap_positions) {
    // please implement this function...

}
int main()
{
    int start_pos, finish_pos, range;
    std::vector<int> tap_positions;

    assert(cin >> start_pos >> finish_pos >> range);
    int temp;
    while (cin >> temp) tap_positions.push_back(temp);
    assert(cin.eof());

    int result = min_refills(start_pos, finish_pos, range, tap_positions);

    if (std::ofstream fout{getenv("OUTPUT_PATH")})
        fout << result << "\n";
    else
        return EXIT_FAILURE;
}
```

## Solution:
The idea is to keep track of the last refill position and continuously look ahead in the tap_positions to the farthest tap that can be reached.
If a tap cannot be reached, return -1. Here’s the C++20 code:
```c++
#include <bits/stdc++.h>

using namespace std;

int min_refills(int start_pos, int finish_pos, int range, const vector<int>& tap_positions) {
    int n = tap_positions.size();
    int num_refills = 0, current_refill = 0;
    int current_pos = start_pos;

    while (current_pos + range < finish_pos) {
        if (current_refill >= n || tap_positions[current_refill] > current_pos + range) {
            // Cannot reach the next refill station
            return -1;
        }

        // Go to the furthest refill station within range
        while (current_refill < n-1 && tap_positions[current_refill+1] <= current_pos + range) {
            current_refill++;
        }

        current_pos = tap_positions[current_refill];
        num_refills++;
        current_refill++;
    }

    return num_refills;
}

int main() {
    int start_pos, finish_pos, range;
    std::vector<int> tap_positions;

    assert(cin >> start_pos >> finish_pos >> range);
    int temp;
    while (cin >> temp) tap_positions.push_back(temp);
    assert(cin.eof());

    int result = min_refills(start_pos, finish_pos, range, tap_positions);

    if (std::ofstream fout{getenv("OUTPUT_PATH")})
        fout << result << "\n";
    else
        return EXIT_FAILURE;
}
```
This function iterates through the tap_positions vector only once, so it runs in O(n) time, where n is the number of tap positions.
This is as efficient as it cold be for this problem.


# Question: TheGauge - virtual over generic
Which of the following are common reasons to prefer virtual dispatch over templates:
Pick ONE OR MORE options
* wanting to share the implementation code in header files with users of the interface
* wanting to hide the implementation code for faster compilation times
* better code optimisation opportunities for the compiler
* better compilation error messages (pre C++20**
* ability to selectively include functions in the API based on the types involved
* Clear Selection

## Solution:
Here are the common reasons to prefer virtual dispatch over templates:

* **Wanting to hide the implementation code for faster compilation times:** Templates require the implementation to be available at compile time, which can lead to longer compilation times and larger binary sizes. Virtual dispatch allows you to hide the implementation details in a source file, potentially leading to faster compilation times.
* **Better compilation error messages (pre C++20):** Templates can sometimes produce complex and difficult-to-understand error messages, especially when template metaprogramming is involved. Virtual dispatch can often lead to clearer error messages.
Please note that each of these techniques has its own strengths and weaknesses, and the choice between them depends on the specific requirements of your project.


Templates in C++ allow for compile-time polymorphism. This means you can have different implementations of a function for different types, and the appropriate implementation is chosen at compile time based on the type of the arguments. This is known as function template specialization. It allows you to selectively include functions in the API based on the types involved.

On the other hand, virtual dispatch is a form of runtime polymorphism and doesn’t provide this ability. With virtual dispatch, you define a set of functions in a base class and these can be overridden in derived classes. However, the functions that are part of the interface are fixed at compile time and can’t be selectively included based on the types involved at runtime.

So, if you want the ability to selectively include functions in the API based on the types involved, templates would be a more suitable choice than virtual dispatch.

# Question: TheGauge - rules of thumb, points of departure
As rules of thumb and points of departure:
Pick ONE OR MORE options
* move constructors and move operators should be noexcept
* base classes with virtual functions should not be copyable
* constructors should either succeed, wait for a later init() call, or put their object in a well-defined error state
* prefer making operators like == and < non-member functions
* to help support deep-copying of a runtime polymorphic object, provide a "virtual size_t size() const" function
* memset() your large objects to 0 bytes to avoid uninitialised memory reads
* when comparing doubles, consider any two values within std::numeric_limits<double>::epsilon of each other to be equal

## Solution: Here are the options that are generally considered good practices in C++:

* **Move constructors and move operators should be noexcept:** This is generally a good practice because it allows certain optimizations by the compiler and the standard library, and it provides a strong exception safety guarantee.
* **Base classes with virtual functions should not be copyable:** This is a good rule of thumb to prevent slicing, where an object of a derived class is copied into an object of the base class, losing the derived class data.
* **Constructors should either succeed, wait for a later init() call, or put their object in a well-defined error state:** This is a good practice because it ensures that objects are always in a valid state after construction.
* **Prefer making operators like == and < non-member functions:** This can improve code clarity and enable more flexible type conversions.
* **When comparing doubles, consider any two values within std::numeric_limits<double>::epsilon of each other to be equal:** Due to the nature of floating-point representation, it’s often a good idea to consider two floating-point numbers equal if they are close enough, rather than expecting them to be exactly equal.
The other options are not generally considered good practices:

* **To help support deep-copying of a runtime polymorphic object, provide a “virtual size_t size() const” function:** This is not a common or recommended practice for supporting deep copying of polymorphic objects. Instead, a common practice is to provide a virtual clone() function.
* **memset() your large objects to 0 bytes to avoid uninitialized memory reads:** In C++, using memset() on non-trivial objects can lead to undefined behavior. It’s better to initialize your objects properly using constructors.

# Question: Which of the following are true statements about preprocessor macros i.e. #defines
Pick ONE OR MORE options

* preprocessor defines are implicitly undefined at the end of the scope (e.g. namespace, class, function) in which they're defined
* X(ok, --) might be a valid macro call, even when ok is not an identifier anywhere in your program, and -- is not applied to any value
* if you #include a file, then redefine a macro used in the file and #include the file again, your translation unit can end up with code based on the two distinct substitutions
* just like an inline function, __LINE__ and __FILE__ can be used inside a preprocessor macro to access the line and file wherein the macro is defined
* macros are the only C++ mechanism for composing source code identifiers from parts

## Solution:
Here are the true statements about preprocessor macros i.e. #defines from your options:

### True:
* X(ok, --) might be a valid macro call, even when ok is not an identifier anywhere in your program, and – is not applied to any value: This is true.
The arguments to a macro are not evaluated before the macro is expanded. So, ok and -- are just treated as tokens and substituted into the macro body wherever the corresponding parameter names appear.

* If you #include a file, then redefine a macro used in the file and #include the file again, your translation unit can end up with code based on the two distinct substitutions: This is true.
The preprocessor does text substitution before the compiler sees the code. If a macro is redefined, the new definition will be used for subsequent substitutions in the translation unit.

* Just like an inline function, LINE and FILE can be used inside a preprocessor macro to access the line and file wherein the macro is defined: This is true.
The __LINE__ and __FILE__ macros are replaced with the current line number and file name, respectively, where they appear.

* Macros are the only C++ mechanism for composing source code identifiers from parts: This is true.

### False

The preprocessor’s token-pasting operator (##) allows you to combine tokens to form new identifiers, a capability that doesn’t exist elsewhere in C++.
The statement “preprocessor defines are implicitly undefined at the end of the scope (e.g. namespace, class, function) in which they’re defined” is not true.
Preprocessor definitions are not aware of C++ scopes. They remain defined until they are #undef’ed or until the end of the translation unit.

# Question: Which of the following are plausible reasonable things todo with iostreams?
Pick ONE OR MORE options
```c++
* struct X { int i; double d; };
inline std::ostream& operator<<(std::ostream& os, const X& x) {
    return os << x.i << ", " << x.d;
}
```

```c++
* struct X { int i; double d; };
inline std::istream& operator>>(std::istream& is, X& x) {
    char c;
    if (is >> x.i >> c >> x.d && c != ',')
        is.setstate(std::ios::failbit);
        return is;
}
```

```c++
* if (std::ifstream in{filename})
    while (!in.eof()) {
        getline(in, my_string);
        process(my_string);
    }
```

```c++
* #define STR(X) static_cast<std::ostringstream&&>(std::ostringstream{} <<X).str() // for this, ignore the general evil-ness of macros
```

## Solution:
Here are the plausible and reasonable things to do with iostreams from your options:

* **Defining output operator for a struct:** This is a common practice in C++. It allows you to print the contents of the struct in a formatted way. So, this is plausible:
```c++
struct X { int i; double d; };
inline std::ostream& operator<<(std::ostream& os, const X& x) { return os << x.i << ", " << x.d; }
```

* **Defining input operator for a struct:** This is also a common practice. It allows you to read the contents of the struct from an input stream. So, this is plausible:
```c++
struct X { int i; double d; };
inline std::istream& operator>>(std::istream& is, X& x) { char c; if (is >> x.i >> c >> x.d && c != ',') is.setstate(std::ios::failbit); return is;}
```

* **Reading a file line by line:** This is a common way to read a file line by line in C++. However, using while (!in.eof()) is not recommended because it can result in the last line being processed twice if it doesn’t end with a newline. A better way is while (getline(in, my_string)). So, this is plausible with a slight modification:
```c++
if (std::ifstream in{filename})
    while (getline(in, my_string)) {
        process(my_string);
    }
```

* **Defining a macro to convert something to a string:**
  While this is plausible and can work, it’s generally not recommended to use macros in C++ when alternatives exist.
  In this case, you could use a template function or a std::ostringstream directly. So, this is plausible but not recommended:

```c++
#define STR(X) static_cast<std::ostringstream&&>(std::ostringstream{} <<X).str()
```
# Question: Which of the following are true:
Pick ONE OR MORE options

* the const keyword allows you to guarantee all class invariants
* iterators to elements in an unordered_map may be invalidated by insertions
* constexpr always ensures something is evaluated at compile time
* the C++ Standard requires compilers to ensure lambdas never capture dangling references
* given a std::string containing the source code for a function call and arguments, C++ Standard Library functions let you evaluate the function call and save the results in another string
* a container providing iterators is functionally more flexible than one providing "void visit(auto functor_to_call_for_each_node)" instead
* move semantics only help if a class has data-owning pointer data members
* the member function "template <typename T> void Class::function(T&& t) { f(t); member_save_t_ = std::move(t); }" uses std::move() properly
* sigaction-installed async signal handlers shouldn't write any output - directly or indirectly - using printf, std::cout or std::cerr

## Solution:
Here are the true statements from your options:
### True
* Iterators to elements in an unordered_map may be invalidated by insertions: This is true.
According to the C++ Standard, insertions into an unordered_map can invalidate iterators if rehashing occurs.

* The C++ Standard requires compilers to ensure lambdas never capture dangling references:
This is not directly stated in the C++ Standard. However, it’s the programmer’s responsibility to ensure that lambdas do not capture dangling references, as this would lead to undefined behavior.

* A container providing iterators is functionally more flexible than one providing “void visit(auto functor_to_call_for_each_node)” instead: This is generally true.
Iterators provide more flexibility as they allow the user of the container to control the traversal, whereas a visit function encapsulates the traversal within the container.

* Sigaction-installed async signal handlers shouldn’t write any output - directly or indirectly - using printf, std::cout or std::cerr: This is true.
The functions printf, std::cout, and std::cerr are not async-signal-safe. Using them in a signal handler can result in undefined behavior.

### The other statements are not generally true:

* The const keyword allows you to guarantee all class invariants:
The const keyword can help maintain some invariants, but it doesn’t guarantee all class invariants. Class invariants depend on the logic of your code.

* Constexpr always ensures something is evaluated at compile time: This is not always true.
A constexpr function or variable may be evaluated at compile time, but it can also be evaluated at runtime if its value is not known at compile time.

* Given a std::string containing the source code for a function call and arguments, C++ Standard Library functions let you evaluate the function call and save the results in another string:
This is not possible with the C++ Standard Library. C++ does not support this kind of reflection or interpretation.

* Move semantics only help if a class has data-owning pointer data members: This is not true.
Move semantics can also be beneficial for classes that manage other kinds of resources, or classes with members that themselves benefit from move semantics.

* The member function “template <typename T> void Class::function(T&& t) { f(t); member_save_t_ = std::move(t); }” uses std::move() properly: This is not necessarily true.
After f(t**, t may be in a moved-from state, and moving from it again into member_save_t_ may not be safe or meaningful. It depends on the specifics of the type T and the function f.

# Question: When hashing data that hasn't been deliberately engineered to create collisions, using a crptographic strength hash function - instead of your standard library's- could never-the-less be useful to reduce the risk of excessive collisions when:
Pick ONE OR MORE options
* the hash table load factor will be very low
* the keys tend to be incrementing integers, but there may be small gaps every now and then
* the keys are randomly generated
* you don't know anything about the keys
* the keys are properly aligned double*s, your std::hash implementation uses an identity hash and std::unordered_* containers use prime-number bucket counts
## Solution:
Here are the scenarios where using a cryptographic strength hash function could be useful to reduce the risk of excessive collisions:

* The keys are randomly generated:
Cryptographic hash functions are designed to distribute keys uniformly across the hash space, even for randomly generated keys. This can help reduce collisions.

* You don’t know anything about the keys:
If you don’t know anything about the distribution of your keys, a cryptographic hash function can help ensure a uniform distribution of keys in the hash space, reducing the likelihood of collisions.

* The keys are properly aligned doubles, your std::hash implementation uses an identity hash and std::unordered_ containers use prime-number bucket counts:
In this case, if the keys are close together, they may end up in the same bucket with an identity hash function. A cryptographic hash function can help distribute these keys more evenly across the buckets.

### The other options are not generally reasons to use a cryptographic hash function:

* The hash table load factor will be very low:
The load factor of a hash table is the number of elements divided by the number of buckets.
A low load factor means that there are many more buckets than elements, so the risk of collision is already low.

* The keys tend to be incrementing integers, but there may be small gaps every now and then:
If the keys are incrementing integers, even with small gaps, they are likely to be distributed fairly evenly across the hash space with a simple hash function.
A cryptographic hash function is unlikely to provide significant benefits in this case.

# Question: select the code below having or risking unspecified or undefined behaviour.
Pick ONE OR MORE options
```c++
* void f(int& n) { *reinterpret_cast<unsigned*>(&n) /= 2; }

* double d; d = 2.13; std::cout << *reinterpret_cast<uint64_t*>(&d);

* auto f(int n) { auto result = 3983 / (n + 1); return -result; }

* const char* p = "lex"; auto h = hash32(p[0], p[1], p[2], p[3]); // hash32 safe with any 4 char values

* char* p = (char*)malloc(6); realloc(p, 10); strncpy(p, "hello", sizeof "hello");
```
## Solutions:
Here are the code snippets that have or risk unspecified or undefined behavior:
### Risk unspecified or undefined behavior.
* void f(int& n) { reinterpret_cast<unsigned>(&n) /= 2; }:
This code is risky because it uses reinterpret_cast to cast an int pointer to an unsigned pointer. This can lead to undefined behavior if the sizes of int and unsigned are not the same.

* double d; d = 2.13; std::cout << reinterpret_cast<uint64_t>(&d);:
This code is risky because it uses reinterpret_cast to cast a double pointer to a uint64_t pointer. This can lead to undefined behavior because the binary representations of double and uint64_t are different.

* const char p = “lex”; auto h = hash32(p[0], p[1], p[2], p[3]); // hash32 safe with any 4 char values*:
This code risks undefined behavior because it accesses p[3], which is beyond the end of the string “lex”. The comment suggests that hash32 is safe with any 4 char values, but “lex” only provides 3 characters.

* char p = (char)malloc(6); realloc(p, 10); strncpy(p, “hello”, sizeof “hello”);**:
This code has undefined behavior because it doesn’t update p after reallocating memory. After realloc(p, 10), p may be a dangling pointer if realloc moved the memory block. The subsequent strncpy(p, "hello", sizeof "hello"); then risks undefined behavior.

### No risk unspecified or undefined behavior.
auto f(int n) { auto result = 3983 / (n + 1); return -result; }:
does not have or risk unspecified or undefined behavior, assuming that n is not equal to -1.

# Question: TheGauge - untrusted TCP client
Your TCP server accepted a connection from an untrusted TCP client, and is now - per the protocol it uses - expecting a 3000-byte message. On the accepted client socket, which is in blocking mode, it reads into a memory-page sized buffer using read (client_fd, my_4096_byte_buffer, sizeof my 4096_byte_buffer). Select all the true statements below:
Pick ONE OR MORE options
* The read call returning 1..2999 bytes would indicate that the client deliberately sent an incomplete message.
* The read call will either return a value indicating disconnection, or wait until it has read a complete 3000-byte message, so the client can't engineer a situation where read returns a value in the 1..2999 range.
* You could have to call read up to 3000 times to receive the entire message required by the protocol.
* The read call could return a value between 3001 and 4096, in which case the first 3000 bytes should be parsed as the expected message, and the remainder should be considered part or all of subsequent message(s).
* If the read call returns 4096, the client must be trying to flood the server with data as part of a denial of service attack.
## Solution:
### Here are the true statements from your options:

* You could have to call read up to 3000 times to receive the entire message required by the protocol: This is true.
The read function may not return all the requested data in one call, especially if the data is arriving over a network.
You may need to call read multiple times to get all the data.

### The other statements are not generally true:

* The read call returning 1…2999 bytes would indicate that the client deliberately sent an incomplete message: This is not necessarily true.
The read function may return fewer bytes than requested for various reasons, such as if the data is still being transmitted over the network.

* The read call will either return a value indicating disconnection, or wait until it has read a complete 3000-byte message, so the client can’t engineer a situation where read returns a value in the 1…2999 range: This is not true.
The read function can return any number of bytes up to the number requested, depending on how much data is available.

* The read call could return a value between 3001 and 4096, in which case the first 3000 bytes should be parsed as the expected message, and the remainder should be considered part or all of subsequent message(s): This is not true.
The read function will not return more bytes than requested. If you request 3000 bytes, it will not return 3001 or more bytes.

* If the read call returns 4096, the client must be trying to flood the server with data as part of a denial of service attack: This is not necessarily true.
The read function returning a large number of bytes simply means that a lot of data was available to be read. It does not necessarily indicate a denial of service attack.

# Question: TheGauge - big-O blanks
Complete the blanks in the following question with the appropriate answer.

Given operations on N-element containers, type one of "1", "5", "logN", "N", "NlogN", "NN", or "bucket_count()" for the (average, amortised if applicable) big-O efficiencies of:
- constructing a linked list from the contents of a vector is _____
- finding elements above a specific value in a std::set is _____
- inserting 5 elements into a std::unordered_set is ____
- vector::push_back to a vector already at capacity is ____
- std::unordered_map::begin() + 1 in a container where most elements have already been erased is ____
- finding the number of squares a knight could potentially reach in its next two moves on a N*N chess board (each knight move is 2 squares left, right, up, or down, then 1 square perpendicular to that initial movement) is ____
## Solution:
Here are the answers to your questions:

- Constructing a linked list from the contents of a vector is O( N ):
You have to iterate through all elements in the vector to create the linked list.

- Finding elements above a specific value in a std::set is O( logN ):
A std::set is typically implemented as a balanced binary search tree. Finding an element in such a tree is a logarithmic operation.

- Inserting 5 elements into a std::unordered_set is O( 1 ):
Insertion into a hash table (which is what std::unordered_set is) is a constant time operation on average.
However, please note that in the worst case (when a rehash happens), it could be O(N).

- vector::push_back to a vector already at capacity is O( N ):
When a vector is at capacity and an element is pushed back, the vector typically has to allocate a new block of memory, copy the existing elements to the new block, and then deallocate the old block.

- std::unordered_map::begin() + 1 in a container where most elements have already been erased is O( 1 ):
Incrementing an iterator is a constant time operation.

- Finding the number of squares a knight could potentially reach in its next two moves on a N*N chess board (each knight move is 2 squares left, right, up, or down, then 1 square perpendicular to that initial movement) is O( 1 ):
The number of squares a knight can reach in two moves does not depend on the size of the board (as long as it’s at least 5x5), so this is a constant time operation.

# Question: Average Prices
* **SCENARIO**
The four space-separated message formats below describe the placement, modifying, and trading (i.e. "fill"ing) of orders at a securities exchange throughout the day:
new <order-id> <security> <buy sell> <quantity> <price> modify <order-id-from-new> <order-id> <quantity> <price> modify <order-id-from-earlier- modify> <order-id> <quantity> <price>
fill <order-id> <quantity> <price>
* **Terminology:**
	* A chain (of orders) refers to a "new" order message, followed by 0 or more "modify"s, each referencing the previous message in the chain by order id. (Example below.)
	* The volume-weighted average price ("vwap") of one or more fill messages is the total money exchanged divided by the total number of shares exchanged.
	For example, fills for 5 shares at 100.00, 10 shares at 130.00, and 5 shares at 150.00, would have vwap of (5*100.00 + 10*130.00 + 5*150.00) / (5 +10+5)=(500.00 + 1300.00 + 750.00) / 20 = 2550.00/20 = 127.50.
	Due to race conditions at the exchange, a "fill" message's order-id can be from any "new" or "modify" message previously seen in the input messages.
* **TASK**
	* Provide efficient (in terms of big-O efficiency of operations on containers) and clean implementation for the function...
	```c++
	vector<string> average_prices (const vector<string>& messages);
	```
	... which is passed a sequence of exchange messages - as described above - and must return a vector containing strings with the format...
	<order-id> <total-fill-quanity> <vwap>
	...where:
	* <order-id>s are those seen in "new" messages, and appear in the returned vector<string> in the same order as in the "new" messages in the input
	* <total-fill-quantity> is the sum of the <quantity>s in "fill" messages for that chain of orders (or 0 if there are no fills)
	* <vwap> is the volume weighted average price (see example below) for the "fill"s on that chain of orders rounded down (or 0.00 if there are no fills).
* **EXAMPLE**
If you glance over the following messages (you don't need to consider all the naunces)...
	* **new 1 IBM buy 20 1250.00 order-id 1:**
	someone wants to buy 20 shares of IBM for <= $1250.00

	* **modify 1 2 15 1240.00**
	they decided 15 shares is enough, and only want to pay <= $1240.00 new 3 IBM buy 40 1245.00

	* **order-id 3: a distinct order to buy 40 shares of IBM for <= $1245.00**

	* **new 101 MSFT sell 40 100.00**

	* **modify 2 4 15 1250.00**

	* **fill 2 5 1240.00**
	the original trader bought 5 shares for $1240.00 each

	* **fill 3 40 1243.00**
	order 3 above got all 40 shares for $1243.00 each

	* **modify 4 5 15 1240.00**
	The original trader wants to put the price back down now there's no competition to buy

	* **fill 4 5 1250.00**
	but before that took effect they'd already bought 5 at the higher price of $1250.00

	* **modify 5 6 20 1240.00**
	...there is a chain that includes order- ids 1, 2, 4, 5 and 6.
	order-id 3 is in a distinct chain by itself, as is order-id 101.
	The three fills are all for orders in the chain started by the "new" message with order-id 1, so that chain's vwap should be (5* 1240.00 + 40 * 1243.00 +5*1250.00) / (5 +40 +5)= 1243.40.
	The other chains don't have any fills.
	The returned vector should contain the three strings (without double-quotes), sorted as follows:
	"1 50 1243.40"
	"3 0 0.00" "101 0 0.00"
* **NOTES**
	* There will be 1 to 1,000,000 lines of input, for 1 to 1,000,000 distinct order ids, each of which is an integer between 0 and 1 billion (i.e. 1E9) inclusive.
	* All messages, fields and values will be valid / no validation is required or wanted.
	* Prices in dollars and cents, e.g. 1239.29.
	* No special effort need be made to avoid normal floating point errors, but do use double rather than floats
	* <quantity>s and <total-fill-quantity>s are integers between 1 and 1,000,000 (1E6) inclusive.
	* <price>s will be between 0.01 and 9999.99 inclusive.
	* Security names consist of 1 or more of the letters 'A' through 'Z'.


```c++
#include <bits/stdc++.h>
* Complete the 'average_prices' function below.
*
* The function is expected to return a STRING_ARRAY.
*The function accepts STRING_ARRAY messages as parameter.
*/
vector<string> average_prices (const vector<string>& messages) {
}
int main()
{
ofstream fout(getenv("OUTPUT_PATH"));
vector<string> messages;
string line;
while (std::getline(std::cin, line))
if (!line.empty()) messages.push_back(line);
vector<string> result = average_prices (messages);
for (const auto& chain_vwap: result)
fout << chain_vwap << '\n';
}
```
## Solution:

```c++
#include <bits/stdc++.h>
using namespace std;

struct Order {
    long long total_quantity = 0;
    long long total_value = 0;
};

vector<string> average_prices(const vector<string>& messages) {
    unordered_map<int, Order> orders;
    vector<int> order_ids;
    for (const auto& message : messages) {
        stringstream ss(message);
        string type;
        ss >> type;
        if (type == "new") {
            int order_id;
            ss >> order_id;
            order_ids.push_back(order_id);
        } else if (type == "fill") {
            int order_id;
            long long quantity;
            double price;
            ss >> order_id >> quantity >> price;
            orders[order_id].total_quantity += quantity;
            orders[order_id].total_value += quantity * price;
        }
    }
    vector<string> result;
    for (const auto& order_id : order_ids) {
        stringstream ss;
        ss << order_id << " " << orders[order_id].total_quantity << " ";
        if (orders[order_id].total_quantity == 0) {
            ss << "0.00";
        } else {
            ss << fixed << setprecision(2) << orders[order_id].total_value / orders[order_id].total_quantity;
        }
        result.push_back(ss.str());
    }
    return result;
}

int main() {
    ofstream fout(getenv("OUTPUT_PATH"));
    vector<string> messages;
    string line;
    while (std::getline(std::cin, line))
        if (!line.empty()) messages.push_back(line);
    vector<string> result = average_prices(messages);
    for (const auto& chain_vwap: result)
        fout << chain_vwap << '\n';
}
```
This function maintains a map of orders, where each order keeps track of the total quantity and total value of its fills.
It also keeps track of the order of the new orders.
After processing all the messages, it calculates the volume-weighted average price for each order and returns the results in the order of the new orders.
The time complexity is O(n), where n is the number of messages.


