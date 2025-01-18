Your list is off to a solid start, but there are a few more techniques you could consider:

# Cache Oblivious Algorithms:
    These algorithms are designed to optimize for all levels of the memory hierarchy without having specific knowledge of the cache size or line size.
    They inherently make good use of the cache by ensuring that data access patterns are favorable.

# Data Locality Optimization:
    Ensuring that data which is frequently accessed together is stored close to each other in memory.
    This minimizes cache misses and takes advantage of spatial locality.
   - **Spatial Locality:** Arrange data in memory such that accessing one element brings nearby elements into the cache.
     - **Example:**
       ```cpp
       struct Point {
           float x, y, z;
       };
       Point points[100];
       ```
       Accessing `points[i]` ensures `x`, `y`, and `z` of the same point are loaded together.

   - **Temporal Locality:** Reuse data frequently accessed in a short time span.
     - Cache reused data in local variables for repeated computations.
# Loop Interchange:
    Changing the order of nested loops to ensure that the innermost loop accesses data contiguously.
    This can improve cache performance significantly by enhancing spatial locality.
## Loop Interchange:
   Rearrange nested loops to access data in a cache-friendly order.
     - Example: Access 2D arrays row-wise if stored in row-major order.
       ```cpp
       for (int i = 0; i < rows; ++i)
           for (int j = 0; j < cols; ++j)
               process(matrix[i][j]); // Cache-friendly
       ```

## Loop Blocking (Tiling):
    Divide large loops into smaller blocks to fit into cache.
     - Example:
       ```cpp
       for (int i = 0; i < N; i += blockSize)
           for (int j = i; j < i + blockSize; ++j)
               process(data[j]);
       ```
## Loop Fusion:
    Combining two or more loops that share the same iteration space.
    This reduces overhead and can improve cache utilization by keeping relevant data in the cache longer.
    It is an optimization technique where two or more loops that iterate over the same range (iteration space) are combined into a single loop.
    This reduces the overhead of loop control (e.g., initialization, condition checking, incrementing counters) and improves **cache utilization**
    by keeping the relevant data in the cache for a longer duration.

### Example Without Loop Fusion (Separate Loops):
    Here, data might get evicted from the cache between the two loops:

    ```cpp
    #include <vector>
    #include <iostream>

    void separateLoops(std::vector<int>& a, std::vector<int>& b, std::vector<int>& c) {
        size_t n = a.size();

        // First loop
        for (size_t i = 0; i < n; ++i) {
            a[i] = a[i] + 2; // Update array 'a'
        }

        // Second loop
        for (size_t i = 0; i < n; ++i) {
            b[i] = a[i] * 3; // Use updated 'a' to update 'b'
        }

        // Third loop
        for (size_t i = 0; i < n; ++i) {
            c[i] = b[i] - 5; // Use 'b' to compute 'c'
        }
    }

    int main() {
        std::vector<int> a(1000, 1), b(1000), c(1000);
        separateLoops(a, b, c);

        std::cout << "c[0]: " << c[0] << std::endl; // Output a value to avoid optimizations
    }
    ```

#### Issues:
    1. **Loop Overhead:** Each loop has its initialization, condition checking, and incrementing.
    2. **Cache Inefficiency:** If the array `a` is large, its values might get evicted from the cache between the first and second loops, requiring another fetch from slower memory.

### Example With Loop Fusion:
    By fusing the loops, all operations on an element are performed together in one loop, ensuring better cache utilization:

    ```cpp
    #include <vector>
    #include <iostream>

    void fusedLoop(std::vector<int>& a, std::vector<int>& b, std::vector<int>& c) {
        size_t n = a.size();

        // Single fused loop
        for (size_t i = 0; i < n; ++i) {
            a[i] = a[i] + 2;  // Update array 'a'
            b[i] = a[i] * 3;  // Use updated 'a' to update 'b'
            c[i] = b[i] - 5;  // Use 'b' to compute 'c'
        }
    }

    int main() {
        std::vector<int> a(1000, 1), b(1000), c(1000);
        fusedLoop(a, b, c);

        std::cout << "c[0]: " << c[0] << std::endl; // Output a value to avoid optimizations
    }
    ```

#### Benefits of Loop Fusion:
    1. **Reduced Loop Overhead:**
      - Only one loop is executed instead of three, saving the overhead of multiple loop counters and checks.
    2. **Better Cache Utilization:**
      - Operations on `a[i]`, `b[i]`, and `c[i]` happen consecutively, ensuring the data remains in the cache longer, reducing cache misses.
    3. **Improved Performance:**
      - Reduced memory access latency, as the program avoids fetching the same data multiple times.

# Cache-Aware Data Structures or Cache-Friendly Algorithms:
   - Design algorithms to minimize random access and prioritize sequential access.
   - Use iterative methods instead of recursive calls where applicable.
   - Designing data structures that fit well into cache lines and make efficient use of cache hierarchies.
   - For example, using smaller, more cache-friendly data structures or modifying existing structures to better align with cache sizes.

# Data Structure Layout
## Structure of Arrays (SoA):
    Improves spatial locality for specific fields by separating large arrays into individual components.
     - Example:
       ```cpp
       struct PointSoA {
           std::vector<float> x, y, z;
       };
       ```
   - **Avoid False Sharing:** Ensure data used by different threads is not stored in the same cache line.


# Prefetching
   - Manually prefetch data into the cache using compiler intrinsics.
     - Example (using GCC/Clang):
       ```cpp
       __builtin_prefetch(&data[i + offset], 0, 3);
       ```

# Memory Alignment
   - Align data to cache line boundaries to improve access speed.
     - Use `alignas` keyword in modern C++.
       ```cpp
       alignas(64) int array[1024];
       ```

# Avoid Pointer Dereferencing
   - Reduce pointer-based indirection as it leads to non-contiguous memory access.

# Minimize Cache Thrashing
   - Reduce conflicts by avoiding repeated eviction and reloading of cache lines.
   - Use hashing or padding to distribute accesses more evenly.

# Software-Controlled Buffering
   - Use local buffers to process data in cache-sized chunks.
     - Example:
       ```cpp
       char buffer[64];
       for (size_t i = 0; i < bufferSize; i += 64) {
           memcpy(buffer, &data[i], 64);
           process(buffer);
       }
       ```

# Use Cache-Friendly Libraries
   - Libraries like Intel TBB and Eigen are optimized for cache efficiency.
   - Example:
     ```cpp
     #include <tbb/parallel_for.h>
     tbb::parallel_for(0, n, [&](int i) { process(data[i]); });
     ```

