**Cache Optimization through Prefetching: Enhancing Non-Contiguous Data Processing**


In the realm of high-performance computing, cache optimization is crucial for ensuring efficient data processing. My previous article focused on optimizing cache usage through data alignment, where we rearranged data structures and leveraged container alignment to maximize cache utilization. This technique works well for data stored in contiguous memory. However, in real-world scenarios, data might not always be stored in contiguous blocks. This article focuses on how to handle or optimize such data for cache using prefetching techniques.

# Understanding Prefetching:

## What is Prefetching?
Prefetching is a technique that tells the CPU to load data into the cache before it is needed. By doing this, we can reduce the time the CPU spends waiting for data from memory, which can significantly speed up processing.

## Why Use Prefetching?
When data is not stored contiguously (e.g., linked lists), accessing it can cause cache misses. Prefetching helps avoid these misses by bringing the required data into the cache in advance.

# Example: Order Processing with Linked List

Let’s consider a simple example of processing `Order` data stored in a linked list:

## Initial Structure and Processing
```cpp
struct Order {
    double price;
    int orderID;
    int quantity;
};
```

**Processing orders in a linked list without prefetching:**

```cpp
void processOrdersWithoutPrefetching(std::list<Order>& orders) {
    for (const auto& order : orders) {
        processOrder(order);
    }
}
```
This straightforward approach may suffer from cache misses because linked lists store their elements in non-contiguous memory.

## Introducing Prefetching

Prefetching instructs the CPU to load data into the cache before it is accessed, reducing memory access latency. Modern CPUs support prefetching with non-blocking instructions, allowing subsequent instructions to execute while prefetching occurs in parallel. This non-blocking nature ensures that the CPU can continue executing other instructions while the data is being fetched into the cache.

**Enhancing the above code with prefetching:**
```cpp
void processOrdersWithPrefetching(std::list<Order>& orders) {
    auto it = orders.begin();
    // Prefetch the first element
    if (it != orders.end()) {
        __builtin_prefetch(&(*it));
    }

    for (; it != orders.end(); ++it) {
        auto nextIt = std::next(it);
        if (nextIt != orders.end()) {
            __builtin_prefetch(&(*nextIt));
        }
        processOrder(*it);
    }
}
```
**Note:** In this article, we provide a general overview of prefetching and use the default parameters for __builtin_prefetch. Detailed explanations of the parameters and their tuning are beyond the scope of this article.

### Key Points About Prefetching

#### **Non-Blocking Nature**:
   - Prefetching does not stop the CPU from executing other instructions. While the data is being fetched, the CPU continues to process.

#### **Optimal Distance**:
   - The CPU needs time to fetch the prefetched data into the cache. Prefetching too close to the current access point may not give enough time, causing delays. Prefetching too far ahead may waste resources.

##### Step-by-Step Example
        1. **First Prefetch***:
        - At the start, we prefetch the first order before we enter the loop.
        - This ensures the first piece of data is ready to use right away.

        ```cpp
        auto it = orders.begin();
        if (it != orders.end()) {
            __builtin_prefetch(&(*it));
        }
        ```

        2. **Inside the Loop**:
          - Inside the loop, we keep prefetching the next order while processing the current one.
          - This keeps the next piece of data ready in advance.

        ```cpp
        for (; it != orders.end(); ++it) {
            auto nextIt = std::next(it);
            if (nextIt != orders.end()) {
                __builtin_prefetch(&(*it));
            }
            processOrder(*it);
        }
        ```

        3. **Platform-Specific Instructions**:
          - `_mm_prefetch` works for Intel platforms. For other systems, check compiler-specific options, such as GCC’s `__builtin_prefetch`.


## Improved Prefetching with Batch Processing

Batch processing involves fetching and processing multiple data items together to reduce cache misses and improve performance.

### Why Batch Processing?
- **Cache Line Utilization**: Modern CPUs fetch data in blocks (cache lines). Using data in batches ensures efficient use of these blocks.

### Example Code for Batch Prefetching

Let’s fetch and process orders in batches of 4:

```cpp
void processOrdersInBatches(std::list<Order>& orders) {
    auto it = orders.begin();
    Order* orderBatch[4];

    while (it != orders.end()) {
        size_t batchSize = 0;

        // Prefetch the next 4 orders and collect them in a batch
        for (int i = 0; i < 4 && it != orders.end(); ++i, ++it) {
            __builtin_prefetch(&(*it));
            orderBatch[batchSize++] = &(*it);
        }

        // Process the batch of prefetched orders
        processOrderBatch(orderBatch, batchSize);
    }
}
```

#### Key Steps in Batch Processing
##### Why Size 4?
- **Cache Line Size**: A typical cache line is 64 bytes.
- **Order Size**: Each `Order` struct is 16 bytes (after reordering).
- **Fit 4 Orders**: Since \( 64 \, \text{bytes} \div 16 \, \text{bytes/order} = 4 \), we can fit 4 `Order` structs in one cache line.

##### Step-by-Step Example

      1. **Initialize Batch and Iterator**:
        - We start by initializing an iterator and a batch array to hold 4 orders.

      ```cpp
      auto it = orders.begin();
      Order* orderBatch[4];
      ```

      2. **Prefetch and Process in Batches**:
        - Inside the loop, we prefetch the next 4 orders and store them in the batch array.
        - This ensures that the next 4 orders are fetched into the cache before they are processed.

      ```cpp
      while (it != orders.end()) {
          size_t batchSize = 0;

    // Prefetch the next 4 orders and collect them in a batch
    for (int i = 0; i < 4 && it != orders.end(); ++i, ++it) {
        __builtin_prefetch(&(*it));
        orderBatch[batchSize++] = &(*it);
    }

          // Process the batch of prefetched orders
          processOrderBatch(orderBatch, batchSize);
      }
      ```

      3. **Prefetch 4 Orders**:
        - The loop runs 4 times (if there are 4 orders available).
        - Each iteration prefetches one order and adds it to the batch array.

      4. **Process the Batch**:
        - After collecting 4 orders (or fewer, if fewer are left), we process them in the batch.
        - This ensures that the fetched orders are used efficiently.

# Google Benchmark:
```bash
Run on (4 X 3300 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 3072 KiB (x1)
Load Average: 3.07, 2.56, 1.98
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-----------------------------------------------------------------------------
Benchmark                                   Time             CPU   Iterations
-----------------------------------------------------------------------------
BM_ProcessOrdersWithoutPrefetching    5556028 ns      5548987 ns          254
BM_ProcessOrdersWithPrefetching       5335091 ns      5329073 ns          263
BM_ProcessOrdersWithPrefetchBatch     4569271 ns      4563944 ns          312

```

## Explanation:
- **Compiler and Flags**: The benchmark was built using GCC 13 with `-O3` optimization.
- **Observations**:
  - Prefetching showed a modest improvement over the baseline (`WithoutPrefetching`).
  - The difference between `WithoutPrefetching` and `WithPrefetching` was small, likely due to compiler optimizations and automatic vectorization.
  - Batch prefetching demonstrated the most significant performance gain due to efficient cache utilization.

---

# Conclusion
Prefetching is a powerful tool for optimizing cache usage, especially for non-contiguous data structures like linked lists. By understanding the non-blocking nature of prefetch instructions, combining them with batch processing, and tailoring them to specific hardware, you can significantly reduce cache misses and improve performance. This article complements the earlier discussion on data alignment, offering a comprehensive approach to cache optimization in diverse scenarios.

