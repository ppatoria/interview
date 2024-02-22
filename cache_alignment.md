One of the low-level features of C++ is the ability to specify the precise
alignment of objects in memory to take maximum advantage of a specific hardware
architecture.
By default, the compiler aligns class and struct members on their size value:
    - 1-byte boundaries : bool and char
    - 2-byte boundaries : short
    - 4-byte boundaries : int, long, and float
    - 8-byte boundaries : long long, double, and long double

In most scenarios, you never have to be concerned with alignment because the
default alignment is already optimal. In some cases, however, you can achieve
significant performance improvements, or memory savings, by specifying a custom
alignment for your data structures.

The C++ standard doesn't specify packing behavior for
alignment on boundaries smaller than the compiler default for the target
platform, so you still need to use the Microsoft #pragma pack in that case.

Use the aligned_storage class for memory allocation of data structures with
custom alignments. The aligned_union class is for specifying alignment for
unions with nontrivial constructors or destructors.

Alignment and memory addresses Alignment is a property of a memory address,
expressed as the numeric address modulo a power of 2.
**For example**
The address 0x0001103F modulo 4 is 3.
That address is said to be aligned to 4n+3, where 4 indicates the chosen power of 2.
The alignment of an address depends on the chosen power of 2.
The same address modulo 8 is 7. An address is said to be aligned to X if its alignment is Xn+0.

Alignment is essentially about how addresses in memory are situated relative to
a certain size boundary, which is typically a power of two. This is important
for performance reasons on most hardware architectures, as they are designed to
access data more efficiently when it is aligned.

Aligned to 4n+0 (or simply aligned to 4-byte boundaries**: This means that an
address is evenly divisible by 4. For an address to be 4-byte aligned, the last
two bits of the address must be zero, because 4 is 2^2, and the two least
significant bits determine the multiples of 4.

**Example**:
```
Address: 0x00011040 is 4-byte aligned because 0x00011040 % 4 = 0.
```

Aligned to 8n+0 (or aligned to 8-byte boundaries**: This means that the address
is evenly divisible by 8. For an address to be 8-byte aligned, the last three
bits of the address must be zero, because 8 is 2^3.

**Example**:
```
Address: 0x00011048 is 8-byte aligned because 0x00011048 % 8 = 0.
``**

CPUs execute instructions that operate on data stored in memory. The data are
identified by their addresses in memory. A single datum also has a size. We call
a datum naturally aligned if its address is aligned to its size. It's called
misaligned otherwise. 
**Example**: an 8-byte floating-point datum is naturally
aligned if the address used to identify it has an 8-byte alignment.

Compiler handling of data alignment Compilers attempt to make data allocations
in a way that prevents data misalignment.

For simple data types, the compiler assigns addresses that are multiples of the
size in bytes of the data type. For example, the compiler assigns addresses to
variables of type long that are multiples of 4, setting the bottom 2 bits of the
address to zero.

The compiler also pads structures in a way that naturally aligns each element of
the structure. Consider the structure struct x_ in the following code example:

```C++

struct x_
{
    char a;  // 1 byte
    int b;   // 4 bytes
    short c; // 2 bytes
    char d;  // 1 byte
}
bar[3];
```
The compiler pads this structure to enforce alignment naturally.
The following code example shows how the compiler places the padded structure in
memory:

C++

Copy // Shows the actual memory layout struct x_ { char a; // 1 byte char
_pad0[3]; // padding to put 'b' on 4-byte boundary int b; // 4 bytes short c; //
2 bytes char d; // 1 byte char _pad1[1]; // padding to make sizeof(x_) multiple
of 4 } bar[3]; Both declarations return sizeof(struct x_) as 12 bytes.

The second declaration includes two padding elements:

char _pad0[3] to align the int b member on a 4-byte boundary. char _pad1[1] to
align the array elements of the structure struct _x bar[3]; on a 4-byte
boundary. The padding aligns the elements of bar[3] in a way that allows natural
access.

The following code example shows the bar[3] array layout:

Output

Copy adr offset element ------ ------- 0x0000 char a; // bar[0] 0x0001 char
pad0[3]; 0x0004 int b; 0x0008 short c; 0x000a char d; 0x000b char _pad1[1];

0x000c char a; // bar[1] 0x000d char _pad0[3]; 0x0010 int b; 0x0014 short c;
0x0016 char d; 0x0017 char _pad1[1];

0x0018 char a; // bar[2] 0x0019 char _pad0[3]; 0x001c int b; 0x0020 short c;
0x0022 char d; 0x0023 char _pad1[1]; alignof and alignas The alignas specifier
is a portable, C++ standard way to specify custom alignment of variables and
user defined types. The alignof operator is likewise a standard, portable way to
obtain the alignment of a specified type or variable.

Example You can use alignas on a class, struct or union, or on individual
members. When multiple alignas specifiers are encountered, the compiler chooses
the one with the largest value.

C++

Copy // alignas_alignof.cpp // compile with: cl /EHsc alignas_alignof.cpp
#include <iostream>

struct alignas(16) Bar { int i; // 4 bytes int n; // 4 bytes alignas(4) char
arr[3]; short s; // 2 bytes };

int main() { std::cout << alignof(Bar) << std::endl; // output: 16 }











`alignas` is a C++ keyword used to specify the alignment requirements for a
variable or a user-defined type. It allows you to control the memory alignment
of objects in memory.

The purpose of `alignas` is to ensure that objects are stored in memory at
specific byte boundaries. Memory alignment is important in some scenarios,
especially when dealing with hardware-specific requirements, such as cache lines
or SIMD (Single Instruction, Multiple Data) instructions.

The syntax of `alignas` is as follows:

```cpp
alignas(expression)
```

The expression can be an integral constant expression that specifies the
alignment requirements in bytes. It can also be a type, in which case the
alignment is based on the alignment requirements of that type.

Here's an example to illustrate how `alignas` works:

```cpp
#include <iostream>

struct alignas(16) MyStruct {
    int value;
    double data;
};

int main() {
    std::cout << "Size of MyStruct: " << sizeof(MyStruct) << std::endl;

    MyStruct obj;

    std::cout << "Address of obj: "       << &obj       << std::endl;
    std::cout << "Address of obj.value: " << &obj.value << std::endl;
    std::cout << "Address of obj.data: "  << &obj.data  << std::endl;

    return 0;
}
```

In this example, we define a structure `MyStruct` with the `alignas(16)`
specifier. This means that objects of `MyStruct` should be aligned on a 16-byte
boundary.

Inside the `main` function, we create an object `obj` of type `MyStruct`. We
then print the size of `MyStruct` using the `sizeof` operator, which gives us
the size in bytes. In this case, it will be the sum of the sizes of `int` and
`double`.

Next, we print the addresses of `obj`, `obj.value`, and `obj.data`. Since `obj`
is aligned on a 16-byte boundary, the addresses will be multiples of 16.

The `alignas` specifier ensures that the object `obj` is aligned according to
the specified alignment requirement. This can be useful in scenarios where you
want to optimize memory access patterns or work with hardware-specific
requirements.

It's important to note that `alignas` does not change the size of the object or
the representation of the data. It only affects the alignment of the object in
memory.

Here's a text-based diagram representing the memory allocation for the
`MyStruct` example with `alignas(16)`:


Certainly! Let's break down the memory representation and provide the size of
each member and padding.

```
  0            4            8            12           16           20           24           28           32           36           40           44           48           52           56           60
  +------------+------------+------------+------------+------------+------------+------------+------------+------------+------------+------------+------------+------------+------------+------------+------------+
  |   value    |   padding  |      padding  |    data                 |   padding  |   padding  |   padding  |   padding  |   padding  |   padding  |
  +------------+------------+------------+------------+------------+------------+------------+------------+------------+------------+------------+------------+------------+------------+------------+------------+

    In this memory representation, we have labeled the size of each member and padding. 
    Please note that the sizes might vary depending on the specific implementation and compiler settings.

    - value member     : Size = 4 bytes (assuming an int takes 4 bytes on your system).
    - First padding    : Size = 4 bytes (to align data on a 8-byte boundary).
    - data member      : Size = 8 bytes (assuming a double takes 8 bytes on your system).
    - Remaining padding: Size = 44 bytes (to align the overall struct on a 64-byte boundary).
The padding bytes are added to ensure that the data member is properly aligned according to the alignas(16) specifier. The size of the struct is the sum of the sizes of its members and padding, which in this case is 60 bytes.

I hope this clarifies the sizes of the members and padding in the memory representation. Let me know if you have any further questions!

Share
Retry




``` ```

In this memory representation, we have labeled the size of each member and
padding. Please note that the sizes might vary depending on the specific
implementation and compiler settings.

- `value` member: Size = 4 bytes (assuming an `int` takes 4 bytes on your
  system).
- First padding: Size = 4 bytes (to align `data` on a 8-byte boundary).
- `data` member: Size = 8 bytes (assuming a `double` takes 8 bytes on your system).
- Remaining padding: Size = 44 bytes (to align the overall struct on a 64-byte boundary).

The padding bytes are added to ensure that the `data` member is properly aligned according to the `alignas(16)` specifier. The size of the struct is the sum of the sizes of its members and padding, which in this case is 60 bytes.

I hope this clarifies the sizes of the members and padding in the memory representation. Let me know if you have any further questions!



  0                 8                 16                24                32                40                48                56                64                72                80                88                96                104               112               120
  +-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+
  |        value    |               padding             |                                               data                                               |                 padding               |
  +-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+
MyStruct "obj"

MemoryRepresentation --> MyStruct: obj


  |------|------|------|------|------|------|------|------|
  |   value     |                padding                  |
  |------|------|------|------|------|------|------|------|
  |                data                     |   padding   |
  |------|------|------|------|------|------|------|------|


|------|------|------|------|------|------|------|------|------|------|------|------|------|------|------|------|
|   value     |                padding                  |                data                     |   padding   |
|------|------|------|------|------|------|------|------|------|------|------|------|------|------|------|------|
^                  ^                                  ^                                   ^
|                      |                                       |                                        |
&obj           &obj.value                      &obj.data                       (End of object)
```

In this diagram, we have a `MyStruct` object consisting of two members: `value`
and `data`. The `value` member is an `int`, and the `data` member is a `double`.

Due to the `alignas(16)` specifier, the object is aligned on a 16-byte boundary.
This means that there will be padding bytes inserted between the members to
ensure the alignment requirement is met.

The diagram shows the layout of the memory allocation for the `MyStruct` object.
The padding bytes are represented as empty spaces. The addresses of `obj`,
`obj.value`, and `obj.data` are indicated by the caret symbol (^).

Note that the size of the `MyStruct` object is the sum of the sizes of its
members (`int` and `double`). The alignment requirement does not change the size
of the object itself, but it affects the placement of the members in memory.

I hope this text-based diagram helps visualize the memory allocation with
`alignas`. Let me know if you have any further questions!

[[./memory_address.jpeg]]


[[./access_memory.webp]**


* word : 4 bytes/32 bit 
* Endian

[[./memory-addressing-addressing-modes-l.jpg]]
* Is aligned when: Address modulus size = 0

* Memory addressing mode influence
** Instruction Count (IC)
** Clock Cycle per Instruction (CPI)
 [[./memory-addressing-addressing-modes-l.jpg]] 
