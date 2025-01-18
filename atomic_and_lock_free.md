### Functions of an `std::atomic` Variable Apart from `load` and `store`

In addition to `load` and `store`, the `std::atomic` type in C++ provides a range of member functions to support atomic operations on shared variables. These functions ensure thread-safe access and modifications. Here’s a list of these functions along with a brief explanation and the applicable memory ordering parameters:

---

### **Atomic Functions**
1. **`fetch_add`** and **`fetch_sub`**:
   - **Description**: Atomically increments or decrements the value of the atomic variable.
   - **Example**:
     ```cpp
     std::atomic<int> counter = 0;
     int old_value = counter.fetch_add(1, std::memory_order_relaxed); // Increment
     ```
   - **Use Case**: Counters, accumulators.

2. **`fetch_and`** and **`fetch_or`**:
   - **Description**: Atomically performs bitwise AND or OR operation.
   - **Example**:
     ```cpp
     std::atomic<int> flags = 0b1010;
     flags.fetch_and(0b0110, std::memory_order_acquire); // Result: 0b0010
     ```
   - **Use Case**: Bitwise flags and state management.

3. **`fetch_xor`**:
   - **Description**: Atomically performs a bitwise XOR operation.
   - **Example**:
     ```cpp
     std::atomic<int> value = 0b1010;
     value.fetch_xor(0b1100, std::memory_order_release); // Result: 0b0110
     ```

4. **`compare_exchange_strong`** and **`compare_exchange_weak`**:
   - **Description**: Atomically compares the value of the atomic variable with an expected value and exchanges it with a new value if they match.
   - **Difference**: `compare_exchange_weak` may fail spuriously (e.g., due to contention).
   - **Example**:
     ```cpp
     std::atomic<int> data = 42;
     int expected = 42;
     data.compare_exchange_strong(expected, 100, std::memory_order_acquire); // Updates data to 100 if expected is 42
     ```
   - **Use Case**: Lock-free data structures and synchronization.

5. **`exchange`**:
   - **Description**: Atomically sets the atomic variable to a new value and returns the old value.
   - **Example**:
     ```cpp
     std::atomic<int> value = 10;
     int old_value = value.exchange(20, std::memory_order_relaxed); // Sets value to 20, returns 10
     ```

6. **`is_lock_free`**:
   - **Description**: Indicates whether the atomic variable is lock-free on the current platform.
   - **Example**:
     ```cpp
     std::atomic<int> value;
     bool lock_free = value.is_lock_free();
     ```

---

### **Memory Ordering Parameters**
Each of the above functions (except `is_lock_free`) accepts an optional `std::memory_order` parameter to control the ordering of atomic operations. The standard provides the following memory orderings:

1. **`std::memory_order_relaxed`**:
   - No synchronization or ordering constraints.
   - Example: Suitable for simple counters or statistics.

2. **`std::memory_order_consume`**:
   - Ensures data dependencies are respected but not widely used due to weak guarantees.

3. **`std::memory_order_acquire`**:
   - Prevents memory reordering of reads that follow the atomic operation.
   - Example: Used when reading a flag to ensure data preceding the flag write is visible.

4. **`std::memory_order_release`**:
   - Prevents memory reordering of writes that precede the atomic operation.
   - Example: Used when writing a flag to signal that preceding writes are complete.

5. **`std::memory_order_acq_rel`**:
   - Combines acquire and release semantics.
   - Example: Used in read-modify-write operations like `fetch_add`.

6. **`std::memory_order_seq_cst`**:
   - Strictest ordering that enforces sequential consistency across threads.
   - Example: Default for most atomic operations.

---

Would you like detailed examples or diagrams for any of these functions?
