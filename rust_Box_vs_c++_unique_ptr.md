# Rust's Box<T> vs. C++'s std::unique_ptr
## Conceptual Similarities:

    - Both Box<T> in Rust and std::unique_ptr in C++ manage heap-allocated memory.
    - They enforce ownership rules, ensuring only one owner at a time, which provides automatic deallocation of the owned object.

## Key Extensions and Modifications in Rust:

### Compile-Time Checks:
    Rust’s borrow checker enforces ownership, borrowing, and lifetime rules at compile time, preventing many common memory safety issues like dangling pointers.
### Ownership Transfer:
    In Rust, ownership transfer is straightforward, with the original owner becoming inaccessible after the move.
    ```
    Example: let b2 = b; // b is no longer valid
    ```
### Borrowing (References):
    Rust allows creating immutable (&T) or mutable (&mut T) references from Box<T>.
    The original Box is temporarily inaccessible while a mutable reference exists but isn't invalidated.
    ```
    Example:
    let mut b = Box::new(5);
    let r = &mut *b; // `b` is temporarily inaccessible 
    ```
### Preventing Dangling References:
    Rust’s borrow checker prevents the use of references after ownership has been transferred, ensuring safety.
    ```
    Example:
    let b = Box::new(5);
    let b2 = b; // Ownership moved
    // println!("{}", b); // Compile-time error
    ```
