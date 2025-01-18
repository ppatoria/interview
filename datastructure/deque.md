# DEQUE
## **What is `std::deque`?**
`std::deque` (short for **double-ended queue**) is a container in C++ that allows efficient insertion and deletion of elements at both ends (front and back). It combines some properties of `std::vector` and `std::list`:
- Like a `std::vector`, it supports random access to elements.
- Like a `std::list`, it allows \( O(1) \) insertion and deletion at both ends.

`std::deque` is highly versatile and suitable for scenarios where you need:
1. Fast insertion/removal at both ends.
2. Random access to elements.
3. Better cache locality compared to a `std::list`.

---

### **Key Characteristics of `std::deque`**
1. **Dynamic Sizing**:
   - It grows or shrinks dynamically as needed, similar to `std::vector`.
   
2. **Double-Ended**:
   - Provides efficient operations at both the front and back using functions like `push_front`, `push_back`, `pop_front`, and `pop_back`.

3. **Random Access**:
   - Allows \( O(1) \) access to any element by index, like `std::vector`.

---

### **Internal Implementation of `std::deque`**
The internal implementation of `std::deque` is **not a single contiguous array** like `std::vector`. Instead, it is implemented as a **two-level data structure**:
1. **Blocks (or Buffers)**:
   - The data is stored in **fixed-size blocks** or chunks, each of which is allocated in memory.
   
2. **Map (Index Array)**:
   - A central map (not to be confused with `std::map`) maintains pointers to these blocks.
   - The map acts as an **index** to locate which block contains a particular element.

#### Key Details:
- When elements are added or removed from the deque:
  - If there is space within the current blocks, operations are \( O(1) \).
  - If the blocks become full, a new block is allocated, and the map is updated.
- The map itself is dynamically resized if needed (e.g., when the number of blocks increases significantly).

---

### **Illustration of `std::deque`**
Suppose we have a deque with 12 elements and block size \( B = 4 \):

**Before adding elements:**
```
Map: [ Block 1 --> Block 2 --> Block 3 ]
Block 1: [1, 2, 3, 4]
Block 2: [5, 6, 7, 8]
Block 3: [9, 10, 11, 12]
```

**After adding an element at the front (`push_front`):**
1. A new block is created if no space is available in Block 1.
2. The new element is added to the new block.
```
Map: [ New Block --> Block 1 --> Block 2 --> Block 3 ]
New Block: [0]
Block 1: [1, 2, 3, 4]
Block 2: [5, 6, 7, 8]
Block 3: [9, 10, 11, 12]
```

---

### **Advantages of `std::deque`**
1. **Efficient Insertions/Deletions at Ends**:
   - \( O(1) \) average-case performance for adding/removing elements at both front and back.
2. **Random Access**:
   - \( O(1) \) random access to elements, similar to `std::vector`.
3. **Dynamic Growth**:
   - Unlike `std::vector`, a `std::deque` does not require copying the entire contents when resizing. Instead, it allocates additional blocks.

---

### **Disadvantages of `std::deque`**
1. **Non-Contiguous Memory**:
   - While more cache-friendly than a `std::list`, a `std::deque` is less cache-efficient compared to `std::vector` because its elements are not stored in a single contiguous array.
   
2. **Overhead for Block Management**:
   - The map and block structure introduce some memory and performance overhead compared to `std::vector`.

---

### **When to Use `std::deque`**
- Use `std::deque` if you need frequent insertions and deletions at both ends.
- If operations are primarily at one end, `std::vector` may be a better choice due to its better cache locality.
- If random access is not required and you need frequent insertions/deletions anywhere, `std::list` may be a better fit.

---

### **Summary**
- `std::deque` is a flexible container that balances the benefits of `std::vector` and `std::list`.
- Internally, it uses a two-level structure with fixed-size blocks and a central map for efficient management.
- It is a great choice for double-ended operations and scenarios requiring moderate cache locality with random access.


## **How `std::deque` Handles Front and Rear Blocks**
1. **Separate Blocks for Front and Rear**:
   - Yes, `std::deque` has separate blocks for the front and rear. It maintains a two-level structure where each block (chunk) can hold multiple elements.
   - When inserting at the front or rear, if there is no space in the current block, a new block is allocated.

2. **New Block Creation for Front Insertions**:
   - If you insert an element at the front (`push_front`) and the frontmost block is already full:
     - A new block is allocated in memory.
     - The map (index array) is updated to point to this new block as the first block.

---

### **Block Capacity in `std::deque`**
1. **Fixed Block Size**:
   - The block size (number of elements each block can hold) in `std::deque` is **implementation-defined** and typically depends on the size of the stored data type and system architecture.
   - For small data types (e.g., integers), blocks often hold more elements, while for large data types (e.g., structs), blocks hold fewer elements.

2. **Can You Set the Block Size?**
   - **No**, the block size of `std::deque` is not configurable in standard C++.
   - The block size is determined internally by the Standard Library implementation and cannot be directly controlled by the user.

---

### **Why You Can't Directly Control Block Size**
The block size is designed to balance:
1. **Memory Usage**: Avoid excessive overhead for managing too many small blocks.
2. **Performance**: Minimize frequent memory allocations and maximize cache efficiency.

The Standard Library assumes a general-purpose use case and optimizes block size based on typical usage patterns.

---
