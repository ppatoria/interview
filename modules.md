# Based on https://clang.llvm.org/docs/StandardCPlusPlusModules.html

A summary of the key points about standard C++ modules as described:

- **Modules** in Clang can refer to Objective-C Modules, Clang C++ Modules, or Standard C++ Modules, each with distinct semantics and interfaces.
- **Standard C++ Modules** are detailed in the C++ Language Specification as either "Named Modules" or "Header Units."
- The term **Modules** in the document refers to the standard C++ modules feature, while **Clang Modules** refers to Clang's C++ modules extension.
- A **module** is composed of one or more **module units**,
    which are special translation units with a module declaration of the form 
    `[export] module module_name[:partition_name];`.
- **Module interface unit** refers to primary module interface units or module interface partition units.
- **Importable module unit** includes any module interface unit or internal module partition unit.
- **Module partition unit** encompasses module interface partition units or internal module partition units.
- **Built Module Interface (BMI)** file is the precompiled result of an importable module unit.
- **Global module fragment** is the section in a module unit from `module;` to the module declaration.

This summary encapsulates the structure and terminology associated with standard C++ modules in Clang.



Clang C++ Modules and Standard C++ Modules are different.
Clang C++ Modules, also known as Clang header modules or Clang module map modules,
are an extension provided by Clang that offers a way to work with modules in C++.
On the other hand, Standard C++ Modules are defined by the C++ Language Specification
and have distinct semantics and workflows compared to Clang's modules.

Standard C++ Modules aim to improve the build times and modularization of C++ code
by providing a more robust and scalable way to handle code dependencies.
They are part of the C++20 standard and beyond, offering a standardized approach to modular programming in C++.

Clang has implemented support for these Standard C++ Modules,
but it's important to note that the command line interfaces
and the way you interact with them can be very different from Clang's own module system.

Examples of built-in Clang modules and Standard C++ Modules:

**Built-in Clang Modules:**
- The `-fbuiltin-module-map` option loads the Clang builtins module map file⁶.
- The `-fimplicit-module-maps` option enables implicit search for module map files named `module.modulemap`⁶.
As for examples and a list of Clang C++ modules, Clang's documentation provides detailed information on how to use modules,
including examples of module declarations and module map files.
However, Clang C++ Modules are an extension of Clang and do not have a predefined list like Standard C++ Modules.
They are defined by the user or the project's module map files.

Here's a simple example of a Clang C++ module declaration in a module map file:

```cpp
module MyModule {
  header "my_header.hpp"
  export *
}
```

This declares a module named `MyModule` that exports everything from `my_header.hpp`. The actual list of modules would depend on the specific project and its structure.

**Standard C++ Modules:**
The `-fimplicit-module-maps` and  `-fbuiltin-module-map` options are typically associated with Clang's own module system
and is not required for Standard C++ Modules as they are enabled automatically when using the `-std=c++20` or newer language standard.

Pre-compilation is not strictly mandatory for C++ modules, but it is a common practice.
Pre-compiling a module creates a Binary Module Interface (BMI**,
which can significantly speed up the compilation process by avoiding the need to parse and compile the module interface repeatedly.

**The main difference between a C++ module and a precompiled header** is that modules are a standard feature designed to replace header files,
providing better encapsulation and control over exported names.
Modules allow multiple imports, whereas most compilers restrict a translation unit to a single precompiled header.
Modules also encapsulate macros, preventing them from leaking into the global namespace, which is not the case with precompiled headers.

- A simple "hello world" example using Standard C++ Modules is as follows:

```cpp
// Hello.cppm
module;
#include <iostream>
export module Hello;
export void hello() {
    std::cout << "Hello World!\n";
}

// use.cpp
import Hello;
int main() {
    hello();
    return 0;
}
```
To compile this, you would use commands like:
```shell
$ clang++ -std=c++20 Hello.cppm --precompile -o Hello.pcm
$ clang++ -std=c++20 use.cpp -fprebuilt-module-path=.
```
This example demonstrates the creation of a module named `Hello` that exports a function `hello()` which prints "Hello World!" to the console.
The `use.cpp` file imports this module and calls the `hello()` function⁷.

- The C++ Standard Library provides modules such as `std.regex`, `std.filesystem`, `std.memory`, and `std.threading`,
which correspond to their respective headers `<regex>`, `<filesystem>`, `<memory>`, and `<thread>`².

# CMake
To use C++20 modules with CMake, you should use the `FILE_SET` property to specify the module files.
Here's an updated `CMakeLists.txt` example that includes the `FILE_SET` directive for building the modules:

```cmake
cmake_minimum_required(VERSION 3.28)
project(HelloWorldModules)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(use use.cpp)
target_sources(use PUBLIC FILE_SET all_my_modules TYPE CXX_MODULES FILES Hello.cppm)

# Specify the use of Ninja as the generator
# Note: This line is for illustration purposes only and won't set the generator.
# You should use the -G option when invoking CMake or use a PreLoad.cmake file.
set(CMAKE_GENERATOR "Ninja" CACHE INTERNAL "" FORCE)
```
Remember, the last line is just for illustration and won't actually set the generator within the `CMakeLists.txt`.
You'll need to use the `-G` option or a `PreLoad.cmake` file as described earlier..

This `CMakeLists.txt` file sets up a project called `HelloWorldModules` that requires at least CMake version 3.28.
It specifies the use of C++20 standards and adds an executable named `use` which is built from the `use.cpp` source file.
The `target_sources` command is used to specify the module files using the `FILE_SET` property¹.

## FILE SET
When you create a `FILE_SET` with multiple modules in CMake, it does not compile them into a single precompiled module.
Instead, each module file listed in the `FILE_SET` will be compiled separately into its own Binary Module Interface (BMI).
The `FILE_SET` is simply a way to organize and manage these files within the build system.
Each module file will be processed individually, allowing for modular compilation and reuse.

## Ninja Generator
The Unix Generator does not support modules so one nee to use Ninja Generator.

The `target_sources` command in CMake is used to specify additional sources for a target.
When you're working with C++20 modules, you can use the `FILE_SET` option within `target_sources` to handle module files⁹.

Here's a breakdown of the command you mentioned:

- `target_sources`   : This command specifies sources to use when building a target.
- `use`              : This is the name of the target, which in this case is an executable.
- `PUBLIC`           : This keyword indicates the scope of the sources.
                       `PUBLIC` means the sources are used when building the target and when building anything that depends on the target.
- `FILE_SET`         : This is a new feature in CMake 3.23 that allows you to define a set of files associated with a target.
- `all_my_modules`   : This is not a keyword but a name you give to the file set.
                       You can name it anything you like.
- `TYPE CXX_MODULES` : This specifies the type of files in the set. `CXX_MODULES` is a type indicating that the files are C++ module interface units or partition units.
- `FILES Hello.cppm` : This lists the files that are part of the file set.
                       If you have multiple module files, you would list them all here separated by spaces.

If you had multiple modules, your command might look like this:

```cmake
target_sources(use PUBLIC FILE_SET all_my_modules TYPE CXX_MODULES FILES Hello.cppm Goodbye.cppm)
```

This would indicate that both `Hello.cppm` and `Goodbye.cppm` are C++ module files associated with the `use` target.
The `FILE_SET` property is particularly useful for organizing and managing module files in a CMake project.

## Setting Generator which supports modules:
While you cannot directly specify a generator inside the `CMakeLists.txt` file,
you can use a `PreLoad.cmake` file to set the generator before running the CMake configuration.
For example, to set Ninja as the default generator,
you could create a `PreLoad.cmake` in your project directory with the following content:

```cmake
set(CMAKE_GENERATOR "Ninja" CACHE INTERNAL "" FORCE)
```

Then, when you run CMake without specifying a generator, it will use Ninja.
However, this approach is not standard and may not be the most elegant solution.
It's generally recommended to specify the generator using the `-G` option when invoking CMake.

The `CMAKE_CXX_EXTENSIONS` property controls whether compiler-specific extensions are used.
If set to `ON`, CMake may add flags like `-std=gnu++11` instead of `-std=c++11`, which enables GNU extensions to the C++ standard.
Setting this property to `OFF` ensures that only the standard C++ is used, which can increase portability across different compilers.

# Module units are categorized into:
  - **Primary module interface unit**   : The main interface of a module     (`export module module_name;`).
  - **Module implementation unit**      : An implementation part of a module (`module module_name;`).
  - **Module interface partition unit** : An exportable part of a module     (`export module module_name:partition_name;`).
  - **Internal module partition unit**  : A non-exportable part of a module  (`module module_name:partition_name;`).

A look at each type of module unit with examples:

1. **Primary module interface unit**:
    This is the main interface of a module.
    It defines the API that other parts of the program can use. Here's an example:

    ``` cpp
    // math.cppm - Primary module interface unit
    module; // Global module fragment declaration

    #include <iostream> // Example include that is not exported

    export module math; // Module declaration

    export int add(int a, int b) {
        return a + b;
    }
    ```
    In a C++ module interface unit, the global module fragment should be declared at the top of the file before any module partition or export declarations.
    The global module fragment is used to specify includes and declarations that are meant to be private to the module itself.


    In this example, `module;` is the global module fragment declaration, and it precedes the `export module math;` declaration.
    Any includes or declarations placed after `module;` and before `export module math;` are considered part of the global module fragment and are not visible to importers of the module.

2. **Module implementation unit**:
   This contains the implementation details of a module.
   It's part of the module but doesn't export any symbols.

    ```cpp
    // math_impl.cpp - Module implementation unit
    module math; // module declaration without 'export'

    int subtract(int a, int b) {
        return a - b;
    }
    ```

3. **Module interface partition unit**:
   This is an exportable part of a module that can be imported separately.
   It's useful for large modules that can be divided into smaller, reusable components.

    ```cpp
    // math_operations.cppm - Module interface partition unit
    export module math:operations; // partition declaration

    export int multiply(int a, int b) {
        return a * b;
    }
    ```

4. **Internal module partition unit**:
   This is a non-exportable part of a module.
   It's used internally within the module and isn't accessible from outside the module.

    ```cpp
    // math_details.cppm - Internal module partition unit
    module math:details; // partition declaration

    int divide(int a, int b) {
        return a / b;
    }
    ```

In a consuming file, you might see something like this:

```cpp
// use_math.cpp
import math; // Imports the primary module interface unit
import math:operations; // Imports the module interface partition unit

int main() {
    int sum = add(1, 2); // Uses the 'add' function from the primary module interface
    int product = multiply(3, 4); // Uses the 'multiply' function from the module interface partition
    return 0;
}
```

In this example, `use_math.cpp` imports:
    - the primary module interface `math` to use the `add` function and
    - the module interface partition `math:operations` to use the `multiply` function.

    - The `subtract` function from the module implementation unit and
    - the `divide` function from the internal module partition unit are not accessible from `use_math.cpp` because they are not exported.

# A guide to build projects using modules in C++.

It includes two examples and explains the process of enabling standard C++ modules,
producing a Binary Module Interface (BMI), and maintaining consistency.

**Example 1: "Hello World"**
```cpp
// Hello.cppm
module;
#include <iostream>
export module Hello;
export void hello() {
  std::cout << "Hello World!\n";
}

// use.cpp
import Hello;
int main() {
  hello();
  return 0;
}
```
This example creates a simple module `Hello` which contains only a primary module interface unit `Hello.cppm`.
The commands to compile and run are:
```bash
clang++ -std=c++20 Hello.cppm --precompile -o Hello.pcm
clang++ -std=c++20 use.cpp -fmodule-file=Hello=Hello.pcm Hello.pcm -o Hello.out
./Hello.out
```

**Example 2: More Complex "Hello World"**
```cpp
// M.cppm
export module M;
export import :interface_part;
import :impl_part;
export void Hello();

// interface_part.cppm
export module M:interface_part;
export void World();

// impl_part.cppm
module;
#include <iostream>
#include <string>
module M:impl_part;
import :interface_part;

std::string W = "World.";
void World() {
  std::cout << W << std::endl;
}

// Impl.cpp
module;
#include <iostream>
module M;
void Hello() {
  std::cout << "Hello ";
}

// User.cpp
import M;
int main() {
  Hello();
  World();
  return 0;
}
```
This example uses four kinds of module units. The commands to compile and run are:
```bash
clang++ -std=c++20 interface_part.cppm --precompile -o M-interface_part.pcm
clang++ -std=c++20 impl_part.cppm --precompile -fprebuilt-module-path=. -o M-impl_part.pcm
clang++ -std=c++20 M.cppm --precompile -fprebuilt-module-path=. -o M.pcm
clang++ -std=c++20 Impl.cpp -fprebuilt-module-path=. -c -o Impl.o
clang++ -std=c++20 User.cpp -fprebuilt-module-path=. -c -o User.o
clang++ -std=c++20 M-interface_part.pcm -fprebuilt-module-path=. -c -o M-interface_part.o
clang++ -std=c++20 M-impl_part.pcm -fprebuilt-module-path=. -c -o M-impl_part.o
clang++ -std=c++20 M.pcm -fprebuilt-module-path=. -c -o M.o
clang++ User.o M-interface_part.o  M-impl_part.o M.o Impl.o -o a.out
```

**Key Points:**
- Standard C++ modules are enabled automatically if the language standard is `-std=c++20` or newer.
- A BMI for an importable module unit can be generated by either `--precompile` or `-fmodule-output` flags.
- The filename of an importable module unit should end with `.cppm` and a module implementation unit should end with `.cpp`.
- The filename of BMIs should end with `.pcm`. The filename of the BMI of a primary module interface unit should be `module_name.pcm`.
- All module names beginning with an identifier consisting of `std` followed by zero or more digits or containing a reserved identifier are reserved
  and shall not be specified in a module declaration.
- There are three methods to specify the dependent BMIs:
  `-fprebuilt-module-path=<path/to/directory>`,
  `-fmodule-file=<path/to/BMI>` (Deprecated), and
  `-fmodule-file=<module-name>=<path/to/BMI>`.
- Module units are translation units. They need to be compiled to object files and linked.
- The language option of module units and their non-module-unit users should be consistent.
- The C++ language defines that same declarations in different translation units should have the same definition, known as the One Definition Rule (ODR).
  The ODR check for the declarations in the global module fragment is disabled by default for better user experience and consistency with MSVC.
  Users who want a more strict check can use the `-Xclang -fno-skip-odr-check-in-gmf` flag to enable the ODR check.

# The impact of Application Binary Interface (ABI) on module units in C++.
It explains that declarations in a module unit, which are not in the global module fragment, have new linkage names.

**Example 1:**
```cpp
export module M;
namespace NS {
  export int foo();
}
```
In this example, the linkage name of `NS::foo()` would be `_ZN2NSW1M3fooEv`.
This couldn't be demangled by previous versions of the debugger or demangler.
As of LLVM 15.x, users can utilize `llvm-cxxfilt` to demangle this:
```bash
llvm-cxxfilt _ZN2NSW1M3fooEv
```
The result would be `NS::foo@M()`, which reads as `NS::foo()` in module `M`.

The ABI implies that we can't declare something in a module unit and define it in a non-module unit (or vice-versa), as this would result in linking errors.

**Example 2:**
```cpp
export module M;
namespace NS {
  export extern "C++" int foo();
}
```
In this example, if we still want to implement declarations within the compatible ABI in module unit,
we can use the language-linkage specifier.
Since the declarations in the language-linkage specifier are attached to the global module fragments,
the linkage name of `NS::foo()` will now be `_ZN2NS3fooEv`.

# Dealing with duplicated declarations in the global module fragments of different module units in C++.
It explains that while it's legal to have such duplications, it's not cost-free for Clang to handle them.
This means that a translation unit will compile slower if it and its importing module units contain a lot of duplicated declarations.

**Example 1:**
```cpp
// M-partA.cppm
module;
#include "big.header.h"
export module M:partA;
...

// M-partB.cppm
module;
#include "big.header.h"
export module M:partB;
...

// other partitions
...

// M-partZ.cppm
module;
#include "big.header.h"
export module M:partZ;
...

// M.cppm
export module M;
export import :partA;
export import :partB;
...
export import :partZ;

// use.cpp
import M;
... // use declarations from module M.
```
In this example, when `big.header.h` is large enough and there are many partitions, the compilation of `use.cpp` may be significantly slower.

**Example 2:**
```cpp
module;
#include "big.header.h"
export module m:big.header.wrapper;
export ... // export the needed declarations

// M-partA.cppm
export module M:partA;
import :big.header.wrapper;
...

// M-partB.cppm
export module M:partB;
import :big.header.wrapper;
...

// other partitions
...

// M-partZ.cppm
export module M:partZ;
import :big.header.wrapper;
...

// M.cppm
export module M;
export import :partA;
export import :partB;
...
export import :partZ;

// use.cpp
import M;
... // use declarations from module M.
```
In this example, the key part of the tip is to reduce the duplications from the text includes.
By creating a wrapper for `big.header.h` and importing it in the module units,
the compilation speed of `use.cpp` can be significantly improved.
This is because the declarations from `big.header.h` are only included once in the wrapper,
reducing the amount of duplicated declarations.

# Converting existing libraries to modules in C++.
The emphasizing that while it's beneficial for new libraries to use modules from the start,
existing libraries may need to provide both headers and module interfaces to avoid breaking changes.

**Example 1: ABI Non-Breaking Styles**
- **Export-Using Style**: This style involves including all headers containing declarations that need to be exported and using such declarations in an export block.
```cpp
module;
#include "header_1.h"
#include "header_2.h"
...
#include "header_n.h"
export module your_library;
export namespace your_namespace {
  using decl_1;
  using decl_2;
  ...
  using decl_n;
}
```
- **Export Extern-C++ Style**: This style involves defining a macro and placing `EXPORT` at the beginning of the declarations you want to export.
    It also suggests refactoring your headers to include third-party headers conditionally.
```cpp
module;
#include "third_party/A/headers.h"
#include "third_party/B/headers.h"
...
#include "third_party/Z/headers.h"
export module your_library;
#define IN_MODULE_INTERFACE
extern "C++" {
  #include "header_1.h"
  #include "header_2.h"
  ...
  #include "header_n.h"
}
```
In your headers, you need to define the macro:
```cpp
#ifdef IN_MODULE_INTERFACE
#define EXPORT export
#else
#define EXPORT
#endif
```

**Example 2: ABI Breaking Style**
This style is similar to the Export Extern-C++ style but changes the ABI to force users to either include your headers all the way or import your modules all the way.
This prevents users from including your headers and importing your modules at the same time in the same repo.
```cpp
module;
#include "third_party/A/headers.h"
#include "third_party/B/headers.h"
...
#include "third_party/Z/headers.h"
export module your_library;
#define IN_MODULE_INTERFACE
#include "header_1.h"
#include "header_2.h"
...
#include "header_n.h"

#if the number of .cpp files in your project are small
module :private;
#include "source_1.cpp"
#include "source_2.cpp"
...
#include "source_n.cpp"
#endif
```
The article also discusses the case where there are headers only included by the source files, providing a header to skip parsing redundant headers,
and importing modules when there are dependent libraries providing modules.
It provides code examples for each case and suggests ways to optimize the process.
The key point in all cases is to remove duplicated declarations in translation units as much as possible.

# Known problems in the current implementation of modules in C++.
Here are the issues along with their examples:

1. **Including headers after import is problematic**:
    The order of `#include <iostream>` and `import foo;` matters.
    The first example is accepted, but the second one is rejected.
    This is due to a limitation in the implementation.
    This issue is tracked [here](https://github.com/llvm/llvm-project/issues/61465).

```cpp
// Accepted
#include <iostream>
import foo;
int main(int argc, char *argv[]) {
    std::cout << "Test\n";
    return 0;
}

// Rejected
import foo;
#include <iostream>
int main(int argc, char *argv[]) {
    std::cout << "Test\n";
    return 0;
}
```

2. **Ignored PreferredName Attribute**:
    When Clang writes BMIs, it ignores the `preferred_name` attribute.
    This issue is tracked [here](https://github.com/llvm/llvm-project/issues/56490).

3. **Don’t emit macros about module declaration**:
    Using macros like `MODULE`, `IMPORT header_name`, `EXPORT_MODULE MODULE_NAME;`, `IMPORT header_name`, and `EXPORT`
    to write code that could be compiled both by modules or non-modules is forbidden by P1857R3.
    This issue is tracked [here](https://github.com/llvm/llvm-project/issues/56917).

4. **Inconsistent filename suffix requirement for importable module units**:
    Clang requires the filename of an importable module unit to end with `.cppm` (or `.ccm`, `.cxxm`, `.c++m`).
    This behavior is inconsistent with other compilers.
    This issue is tracked [here](https://github.com/llvm/llvm-project/issues/57416).

5. **clang-cl is not compatible with the standard C++ modules**:
    Currently, `/clang:-fmodule-file` or `/clang:-fprebuilt-module-path` can't be used to specify the BMI within `clang-cl.exe`.
    This issue is tracked [here](https://github.com/llvm/llvm-project/issues/64118).

6. **False positive ODR violation diagnostic**:
    Sometimes, the compiler gives false positive diagnostics for ODR violation.
    This issue is tracked [here](https://github.com/llvm/llvm-project/issues/78850).

```cpp
// part.cc
module;
typedef long T;
namespace ns {
inline void fun() {
    (void)(T)0;
}
}
export module repro:part;

// repro.cc
module;
typedef long T;
namespace ns {
    using ::T;
}
namespace ns {
inline void fun() {
    (void)(T)0;
}
}
export module repro;
export import :part;
```

7. **Using TU-local entity in other units**:
    Entities that should only be local to the module unit itself shouldn’t be used by other units.
    This issue is tracked [here](https://github.com/llvm/llvm-project/issues/78173).

# The use of header units in C++ and provides several examples:

1. **Quick Start**:
    The article provides a simple example of how to compile a program using header units.
    The code `import <iostream>;` is compiled using the commands `clang++ -std=c++20 -xc++-system-header --precompile iostream -o iostream.pcm`
    and `clang++ -std=c++20 -fmodule-file=iostream.pcm main.cpp`.

2. **Producing BMIs**:
    The article explains how to produce Binary Module Interface (BMI) files.
    It provides an example where a header file `foo.h` is compiled into a BMI using the command `clang++ -std=c++20 -fmodule-header foo.h -o foo.pcm`.
    The resulting BMI can then be used in another file `use.cpp` with the command `clang++ -std=c++20 -fmodule-file=foo.pcm use.cpp`.

3. **Specifying Dependent BMIs**:
    The `-fmodule-file` option can be used multiple times to specify multiple BMIs.
    However, `-fprebuilt-module-path` cannot be used for header units.

4. **Don't Compile the BMI**:
    The article emphasizes that you cannot compile the BMI from a header unit.
    An example is provided to illustrate this.

5. **Include Translation**:
   The article explains that the C++ specification allows vendors to convert `#include header-name` to `import header-name;` when possible.
   Clang does this translation for the `#include` in the global module fragment.

6. **Relationships between Clang Modules**:
   The article discusses the similarities and differences between header units and Clang modules.
   It provides an example of how to "mimic" the style of header units using Clang modules.
   However, it explains that Clang modules have additional semantics that are not part of Standard C++ Header units,
   which is why they are not used to implement header units.

# The use of dependencies in C++ modules and examples:

1. **Discover Dependencies**:
    The article explains that unlike traditional translation units,
    module units cannot be compiled in parallel due to their dependencies.
    The `clang-scan-deps` scanner is used to describe the order of compilation.

2. **Compilation Database**:
    A compilation database is required to use `clang-scan-deps`.
    The article provides an example of a compilation database and how to get dependency information in P1689 format using `clang-scan-deps`.

3. **Dependency Information Per File**:
    The article explains how to get dependency information for each file using `clang-scan-deps`.
    It also discusses how `clang-scan-deps` can generate dependency information for headers if specified preprocessor options are given in the command line.

4. **Possible Issues**:
    The article discusses potential issues users may encounter, such as failing to find system headers.
    It provides four potential solutions to these problems.

5. **Import Modules with clang-repl**:
    The article provides an example of how to import C++20 named modules with `clang-repl`.
    It explains that the module unit needs to be compiled into a dynamic library so that `clang-repl` can load the object files of the module units.

6. **How Modules Speed Up Compilation**:
    The article discusses the theory of how modules can speed up compilation and explains that the actual behavior depends on the optimization level.
    It provides a detailed explanation of how the compilation process works at different optimization levels.

7. **Interoperability with Clang Modules**:
    The article mentions that they aim to support both clang modules and standard C++ modules at the same time, but the mixed usage form is not well tested yet.
    Users are encouraged to file new GitHub issues if they find interoperability problems.
