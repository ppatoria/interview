# Cache Optimization: Focusing on Data Alignment

## Introduction
Cache optimization is a cornerstone of high-performance computing. This article focuses on optimizing data alignment and data structures for improved cache utilization. By leveraging benchmarking results, we refine the design and implementation of a `MarketData` struct to illustrate tangible performance gains through alignment techniques.

### Note
This is the first article in a series on cache optimization, concentrating on alignment. To keep things simple, we omit multicore processing issues. In such scenarios, the shared nature of L1 and L2 caches introduces complexities like false sharing, fault tolerance, and more. These will be addressed in future articles. Techniques like buffering and prefetching, while omitted here, will also be covered in subsequent discussions.

---

## Initial Implementation: Baseline Design
The initial design of the `MarketData` struct is as follows:

```cpp
struct MarketData {
    int symbol_id;  // 4 bytes
    double price;   // 8 bytes
    int volume;     // 4 bytes
};
```

### Analysis
- **Field Sizes and Padding:**
  - `int` fields (`symbol_id` and `volume`) are 4 bytes each, while `double price` is 8 bytes.
  - Due to alignment rules, the compiler adds padding after `symbol_id` (4 bytes) and after `volume` (4 bytes), making the total size of the struct 24 bytes (16 bytes for fields + 8 bytes padding).
- **Cache Line Fit:**
  - Assuming a typical cache line size of 64 bytes, each instance of `MarketData` leaves unused space in the cache line. This could lead to inefficient cache utilization when processing arrays of `MarketData`.

Let’s address these inefficiencies through iterative improvements.

---

## Improving Memory Layout and Alignment
### Reordering Fields to Minimize Padding
By rearranging fields, we can reduce the padding and optimize the memory layout.

```cpp
struct MarketDataReordered {
    double price;   // 8 bytes
    int symbol_id;  // 4 bytes
    int volume;     // 4 bytes
};
```

### Explanation
- **Field Sizes and Padding:**
  - Placing the largest field (`double price`) first eliminates the padding after `symbol_id`.
  - Total size is now 16 bytes (all fields fit contiguously without padding).
- **Cache Line Fit:**
  - This smaller size increases the number of `MarketDataReordered` instances that fit in a single cache line, improving cache efficiency during sequential access.

---

## Explicit Alignment for Cache Line Optimization
Aligning the struct to the cache line size ensures that each instance starts at a cache-line boundary, reducing cache contention in multithreaded scenarios.

```cpp
struct alignas(64) MarketDataAligned {
    double price;   // 8 bytes
    int symbol_id;  // 4 bytes
    int volume;     // 4 bytes
    // Padding: 48 bytes (added to make total size 64 bytes)
};
```

### Note
- While aligning to 64 bytes is useful in multithreaded contexts to avoid false sharing, it introduces unnecessary memory overhead for single-threaded applications. For optimal single-threaded performance, the `MarketDataReordered` struct (16 bytes) is preferred.

---

## Further Improvement by Aligning Containers to Cache Line
Efficient batch processing requires that arrays or vectors of data are also cache-aligned.

### Using Aligned Arrays
Aligned arrays ensure contiguous, cache-friendly storage for fixed-size data.

```cpp
template <typename T, std::size_t N>
struct AlignedArray {
    alignas(64) std::array<T, N> data;
};

using AlignedMarketDataArray = AlignedArray<MarketDataReordered, 1000>;
```

### Explanation
- **Field Sizes and Padding:**
  - Each `MarketDataReordered` instance is 16 bytes.
  - The total size of the array is a multiple of 64 bytes, ensuring cache alignment.
- **Cache Line Fit:**
  - Sequential processing of `AlignedMarketDataArray` leverages cache lines effectively, reducing cache misses.

### C-Style Aligned Arrays
For applications requiring C-style arrays, similar alignment can be achieved using explicit memory alignment.

```cpp
struct AlignedCArray {
    alignas(64) MarketDataReordered data[1000];
};
```

### Using Aligned Vectors
Dynamic arrays can also benefit from cache alignment by using a custom aligned allocator.

```cpp
template <typename T, std::size_t Alignment>
struct aligned_allocator {
    using value_type = T;

    T* allocate(std::size_t n) {
        void* ptr = nullptr;
        if (posix_memalign(&ptr, Alignment, n * sizeof(T)) != 0) throw std::bad_alloc();
        return static_cast<T*>(ptr);
    }

    void deallocate(T* ptr, std::size_t) { free(ptr); }
};

using AlignedVector = std::vector<MarketDataReordered, aligned_allocator<MarketDataReordered, 64>>;
```

### Explanation
- **Dynamic Size Flexibility:**
  - While dynamic allocation adds overhead, the aligned allocator ensures that data is cache-aligned for efficient access.

---

## Additional Optimizations

### Loop Unrolling
Unrolling loops reduces loop control overhead and enhances instruction-level parallelism. Additionally, it leverages cache efficiency by processing multiple elements loaded into a cache line in a single iteration.

```cpp
void Process(AlignedMarketDataArray updates) {
    #pragma GCC unroll 4
    for (const auto& update : updates.data) {
        // Process market data
    }
}
```

### Why Unroll 4?
- **Cache Line Fit:**
  - With a `MarketDataReordered` size of 16 bytes and a 64-byte cache line, four instances fit perfectly into a single cache line. Unrolling the loop by 4 ensures that all elements loaded into the cache line are processed in one iteration, maximizing cache utilization.
- **Reduced Loop Overhead:**
  - Fewer loop control instructions are executed, improving efficiency.
- **Other Benefits:**
  - Unrolling also allows better instruction pipelining and parallelism, enabling the CPU to execute multiple instructions simultaneously.

### Explanation
- By unrolling the loop to match the number of elements fitting into a cache line, we align the processing logic with hardware-level optimizations. This reduces memory access latencies and maximizes throughput.

---

## Conclusion
Through iterative optimizations, we achieved:
1. Reduced padding by reordering fields.
2. Aligned data structures to cache line boundaries (where necessary).
3. Leveraged aligned containers for batch processing.
4. Enhanced performance with loop unrolling tailored to cache line size.

Future work will explore techniques like buffering and prefetching, along with advanced considerations for multicore architectures, in subsequent articles.

