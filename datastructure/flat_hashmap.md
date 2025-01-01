# Flat Hashtable

The main difference between a **regular hashtable with linear probing** and a **flat hashtable** lies in the memory layout and how collisions are resolved. Let's break it down:

---

### **1. Regular Hashtable with Linear Probing**
- **Memory Layout**:
  - Typically implemented with a **table of pointers to nodes**.
  - Each bucket in the table may point to a dynamically allocated object containing the key-value pair.
  - This introduces an **indirection**: accessing a bucket first accesses the pointer, then dereferences it to the actual data.

- **Collision Resolution**:
  - Linear probing involves sequentially scanning the table for the next available slot in case of a collision.
  - Even though linear probing keeps probing in the same contiguous memory region, the indirection to access values (due to pointers) remains.

- **Cache Performance**:
  - Poorer cache performance because the actual data resides in **dynamically allocated memory**, which may not be contiguous.
  - Every access involves pointer dereferencing, which might result in cache misses if the memory for the keys/values is scattered.

---

### **2. Flat Hashtable**
- **Memory Layout**:
  - Stores key-value pairs **directly in a contiguous memory array**.
  - No pointers or separate memory allocations for individual elements.

- **Collision Resolution**:
  - Linear probing or other open addressing schemes are used, but all the data resides **inline** in the same contiguous memory block.
  - This eliminates pointer indirection.

- **Cache Performance**:
  - More **cache-friendly** because:
    - Data is laid out contiguously, so accessing adjacent elements (e.g., during probing) benefits from **spatial locality**.
    - No dereferencing of pointers is required, reducing potential cache misses.

---

### **Why Regular Hashtable is Less Cache-Friendly?**

1. **Pointer Indirection**:
   - A regular hashtable often uses pointers to dynamically allocated memory for storing actual key-value pairs.
   - Accessing a key-value pair requires multiple memory accesses: one for the pointer and one for the actual data.

2. **Fragmented Memory**:
   - Dynamically allocated memory can be scattered across the heap, leading to poor **spatial locality**.
   - This increases the likelihood of cache misses.

3. **Contiguous Memory in Flat Hashtable**:
   - In a flat hashtable, the key-value pairs are stored directly in contiguous memory. When the CPU fetches a cache line, it loads several adjacent entries, reducing the need for additional memory accesses.

---

### **Illustration**
Consider a simple comparison:

1. **Regular Hashtable (with Linear Probing)**:
   ```plaintext
   Table (array of pointers):
   [ptr1, ptr2, ptr3, ...]
    ↓       ↓       ↓
   Key-Value Nodes in Memory:
   [k1, v1] [k2, v2] [k3, v3]
   ```

   - To access a value, the table accesses `ptr` (cache miss possible), then dereferences it (another potential cache miss).

2. **Flat Hashtable**:
   ```plaintext
   Table (array of key-value pairs directly):
   [[k1, v1], [k2, v2], [k3, v3], ...]
   ```

   - To access a value, the table directly reads `[k, v]` from the contiguous memory array (fewer cache misses).

---

### **When to Use Flat Hashtable?**
- Use flat hashtables when:
  - **Low latency** is critical (e.g., trading systems, real-time systems).
  - The dataset is relatively small or fits comfortably in memory.
  - Cache efficiency matters more than flexibility in memory allocation.

- Regular hashtables may still be useful for:
  - Very large datasets where dynamic resizing and flexible memory management are necessary.
  - Applications where low memory overhead for sparse data structures is a priority.
