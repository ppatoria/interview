# Cache-Oblivious Algorithms: Overview and Purpose

**Purpose**:
Cache-oblivious algorithms are designed to utilize the memory hierarchy (including CPU caches) efficiently,
without requiring explicit knowledge of the cache size, block size, or other architectural parameters.
They achieve this by inherently leveraging locality of reference (both spatial and temporal) to minimize cache misses across multiple levels of the memory hierarchy.

**Key Idea**:
Instead of tuning algorithms for specific cache sizes or configurations, cache-oblivious algorithms rely on recursive divide-and-conquer strategies.
This ensures that the algorithm works efficiently regardless of the memory architecture, making it portable and robust.

---

### How Cache-Oblivious Algorithms Work at Memory and Cache Levels

1. **Recursive Subdivision**:
   - Divide the problem into smaller subproblems until each subproblem fits into the cache.
   - Operate on each subproblem independently before moving to the next, ensuring that data is reused while still in the cache.

2. **Exploiting Locality**:
   - **Temporal Locality**: Repeatedly accessing the same small dataset during recursive calls keeps it in the cache.
   - **Spatial Locality**: Sequential access patterns ensure that prefetching mechanisms efficiently load adjacent data into cache lines.

3. **Implicit Cache Optimization**:
   - No explicit tuning for block or cache size is required; the recursive nature ensures optimal use of cache at every level of the hierarchy.
   - Works seamlessly for multi-level caches, including L1, L2, and L3, as well as main memory.

---

### Achievements of Cache-Oblivious Algorithms

1. **Portability**:
   - The algorithm adapts to any memory hierarchy without modification.
   - Reduces dependency on specific hardware parameters.

2. **Scalability**:
   - Performs efficiently across various problem sizes and cache configurations.
   - Can handle larger datasets without manual tuning.

3. **Reduced Cache Misses**:
   - Recursive design inherently minimizes cache misses.
   - Optimal for both caches and TLBs (Translation Lookaside Buffers).

---

## Real-World Example: Order Book Updates in Low-Latency Trading

In a high-frequency trading system, the order book is a critical data structure that maintains the state of market orders.
Cache-oblivious algorithms can optimize updates to the order book for low latency.

---

#### **Scenario**: Efficiently Update an Order Book

##### **Step 1**: Problem Definition
An order book contains multiple levels of buy and sell orders for various stock symbols. Each level is represented by:
- **Price**: The price at this level.
- **Quantity**: The number of shares available at this price.
- **Timestamp**: The last update time.

Incoming updates must be processed in real time:
- Add new levels or update existing levels.
- Remove levels with zero quantity.

---

##### **Step 2**: Cache-Oblivious Design Approach

1. **Data Structure**:
   - Represent the order book as a balanced binary tree or an array-based heap.
   - Each node contains:
     - Price
     - Quantity
     - Timestamp

2. **Recursive Divide-and-Conquer**:
   - Split the order book into smaller subtrees or blocks.
   - Each recursive step processes a block independently, ensuring it fits into the cache.

3. **In-Place Updates**:
   - Perform updates directly within the subtree.
   - Leverage temporal locality by keeping frequently accessed subtrees in the cache.

4. **Merging Results**:
   - Combine updated subtrees to reflect changes in the main order book.

---

##### **Step 3**: Implementation Details

**Recursive Algorithm for Order Book Updates**:
```cpp
struct OrderBookNode {
    double price;
    int quantity;
    long long timestamp;
};

// Cache-oblivious recursive update
void updateOrderBook(OrderBookNode* orderBook, int start, int end, const OrderBookNode& update) {
    // Base case: Single node
    if (start == end) {
        if (orderBook[start].price == update.price) {
            orderBook[start].quantity += update.quantity;
            orderBook[start].timestamp = update.timestamp;
        }
        return;
    }

    // Recursive case: Divide the array into two halves
    int mid = start + (end - start) / 2;

    if (update.price <= orderBook[mid].price) {
        // Process left half
        updateOrderBook(orderBook, start, mid, update);
    } else {
        // Process right half
        updateOrderBook(orderBook, mid + 1, end, update);
    }
}

// Example usage
int main() {
    OrderBookNode orderBook[1000]; // Simulated order book
    OrderBookNode update = {101.5, 200, 1673578563}; // Sample update

    updateOrderBook(orderBook, 0, 999, update);
    return 0;
}
```

---

##### **Step 4**: Memory and Cache-Level Behavior

1. **Recursive Subdivision**:
   - The recursive calls process smaller sections of the order book.
   - Each section fits into the L1 or L2 cache during processing.

2. **Temporal Locality**:
   - Frequently accessed nodes (e.g., root or parent nodes) stay in the cache across recursive calls.

3. **Spatial Locality**:
   - Access patterns ensure adjacent nodes are loaded together into cache lines.

4. **Reduced Cache Misses**:
   - Only relevant parts of the order book are brought into the cache.

---

##### **Step 5**: Benefits in Low-Latency Trading

1. **Latency Reduction**:
   - Minimizes cache misses, speeding up the order book update process.

2. **Scalability**:
   - Handles larger order books without requiring cache-specific tuning.

3. **Portability**:
   - Adapts to various cache configurations across trading systems.

---

!### Key Takeaways

1. **Purpose**: Cache-oblivious algorithms abstract away hardware-specific parameters, achieving efficient memory usage across all cache levels.
2. **How It Works**: Recursive divide-and-conquer ensures optimal use of cache without manual tuning.
3. **Achievements**: Reduced cache misses, scalability, and portability.
4. **Application in Trading**: Efficient order book updates improve performance in high-frequency trading systems, ensuring minimal latency and scalability.

This design ensures robustness and performance, critical for maintaining competitiveness in low-latency trading environments.

### Compared to Traditional Orderbook implementation.
In real-world **low-latency trading applications**, the choice of data structure for an **order book** depends on the specific use case and performance requirements. While **array-based binary trees** offer excellent **cache efficiency**, they are not the most common choice for implementing order books in live trading systems. Here's why, along with an explanation of what is typically used:

---

#### **1. Challenges of Using Array-Based Binary Trees in Order Books**
1. **Dynamic Nature of Trading Data**:
   - Order books are highly dynamic, with orders being added, modified, and removed in real-time.
   - Maintaining the balance of an array-based binary tree during frequent updates can be complex and less efficient, as it may require rebalancing and resizing operations.

2. **Fixed-Size Limitations**:
   - Array-based binary trees typically require pre-allocation of memory, which may not be practical for highly volatile markets with unpredictable order book depths.

3. **Order Matching**:
   - Matching orders often involves finding the **best bid** or **best ask**, which is simpler and faster with a **heap** or a well-structured **map** rather than navigating through an array.

---

#### **2. Commonly Used Data Structures in Real-Life Trading Applications**
In practice, the most commonly used data structures for order books are:

#### **a. `std::map` or Equivalent**
- **Why It's Used**:
  - Represents a **price-to-orders mapping**, where the key is the price, and the value is a list (or vector) of orders at that price level.
  - Allows efficient insertion, deletion, and lookup operations in `O(log N)` time.
- **Example**:
  ```cpp
  std::map<double, std::vector<Order>> orderBook;
  ```

- **Advantages**:
  - Naturally handles dynamic updates (inserts, deletes, modifications).
  - Orders are sorted by price, making it easy to find the best bid and ask.
  - Widely supported and robust implementation in most programming libraries.

- **Disadvantages**:
  - Less cache-friendly due to pointer-based tree traversal.

---

#### **b. `std::vector` (or Flat Array)**
- **Why It's Used**:
  - Some trading systems use a **flat array or vector**, especially for fixed price ranges (e.g., whole-number price levels).
  - Each index in the array corresponds to a price level, and the value at that index represents the quantity or list of orders.

- **Advantages**:
  - Extremely cache-friendly due to contiguous memory storage.
  - Fast lookups and updates for fixed price ranges.

- **Disadvantages**:
  - Inefficient for sparse price distributions (e.g., low trading volumes at some price levels).
  - Requires reallocation if the price range exceeds the pre-allocated bounds.

---

#### **c. Custom Data Structures**
- Many trading firms design **custom hybrid data structures** tailored to their specific needs, often combining the benefits of multiple approaches:
  - **Heap**: For quickly identifying the best bid or ask.
  - **Hash Table + Doubly Linked List**: For fast updates and traversal of order levels.

---

#### **3. Why Array-Based Binary Trees Are Rare in Real-Time Trading**
1. **Dynamic Resizing**:
   - Trading data is highly dynamic, and resizing or rebalancing an array-based binary tree can introduce unwanted latency.

2. **Ease of Maintenance**:
   - Implementing efficient order matching and modifications is more complex with array-based binary trees than with `std::map` or `std::vector`.

3. **Tooling and Support**:
   - Standard libraries like `std::map` and `std::vector` are highly optimized and well-supported, making them the default choice for many trading applications.

---

#### **4. When Array-Based Binary Trees Might Be Used**
Although rare, array-based binary trees might be used in specific scenarios:
1. **Static Order Books**:
   - For simulations or backtesting where the order book does not change frequently.

2. **Cache-Sensitive Subsystems**:
   - Certain components (e.g., batch processing of trades) may benefit from the cache efficiency of array-based binary trees.

---

#### **5. Summary of Real-Life Usage**
| **Data Structure**      | **Used in Real-Life?** | **Typical Use Case**                                                             |
|--------------------------|------------------------|----------------------------------------------------------------------------------|
| `std::map`               | Yes                   | Dynamic order books with frequent updates and sorted price levels.               |
| `std::vector`            | Yes                   | Dense price ranges with pre-allocated fixed size (e.g., highly liquid assets).   |
| Array-Based Binary Tree  | Rare                  | Static or cache-sensitive systems where updates are infrequent.                  |
| Custom Hybrid Structures | Yes                   | High-frequency trading systems for optimized matching and performance.           |

In conclusion, **`std::map`** and **`std::vector`** are the most commonly used data structures for order books in real-life trading applications. Array-based binary trees are typically avoided due to their complexity in handling dynamic updates, despite their theoretical cache efficiency.
## Real-world Example of Moving Average Calculation
A **real-world application of a cache-oblivious algorithm** in trading systems where it can excel compared to traditional cache-friendly techniques is in **historical data backtesting**.
Backtesting involves processing large datasets (e.g., price data, trades, or order book snapshots) to simulate the performance of a trading strategy.
In this scenario:

1. **Cache-oblivious algorithms** are well-suited because:
   - They optimize for **batch processing**.
   - They handle large datasets efficiently without requiring explicit tuning for hardware-specific cache parameters.

2. **Key operations** often involve:
   - Aggregating data (e.g., computing moving averages, cumulative sums).
   - Traversing large datasets sequentially or recursively.

---

### **Real-World Application: Moving Average Calculation**

A **moving average** is a common operation in trading for analyzing historical price trends.
We will use a **cache-oblivious algorithm** to compute a moving average on a large dataset efficiently.

---

#### **Code Implementation**

```cpp
#include <iostream>
#include <vector>
#include <numeric> // For std::accumulate
#include <algorithm> // For std::min

// Function to compute moving averages recursively (cache-oblivious algorithm)
void computeMovingAverageRecursively(
    const std::vector<double>& prices,
    std::vector<double>& movingAverages,
    size_t start,
    size_t end,
    size_t windowSize
) {
    // Base case: Process directly if the range is small enough
    constexpr size_t ChunkSize = 8; // Tuned for cache efficiency
    if (end - start <= ChunkSize) {
        for (size_t i = start; i + windowSize <= end; ++i) {
            movingAverages[i] = std::accumulate(prices.begin() + i, prices.begin() + i + windowSize, 0.0) / windowSize;
        }
        return;
    }

    // Divide: Split the range into two halves
    size_t mid = start + (end - start) / 2;

    // Conquer: Process the left and right halves recursively
    computeMovingAverageRecursively(prices, movingAverages, start, mid, windowSize);
    computeMovingAverageRecursively(prices, movingAverages, mid, end, windowSize);

    // Combine: Handle edge cases across the split boundary
    for (size_t i = std::max(mid - windowSize + 1, start); i < std::min(mid + windowSize, end - windowSize + 1); ++i) {
        movingAverages[i] = std::accumulate(prices.begin() + i, prices.begin() + i + windowSize, 0.0) / windowSize;
    }
}

int main() {
    // Simulated historical price data
    std::vector<double> prices = {100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114};
    size_t windowSize = 3; // Window size for moving average
    std::vector<double> movingAverages(prices.size(), 0.0);

    // Compute moving averages using the cache-oblivious algorithm
    computeMovingAverageRecursively(prices, movingAverages, 0, prices.size(), windowSize);

    // Output results
    std::cout << "Moving Averages:" << std::endl;
    for (size_t i = 0; i < movingAverages.size(); ++i) {
        if (i + windowSize <= prices.size()) {
            std::cout << "Index " << i << ": " << movingAverages[i] << std::endl;
        }
    }

    return 0;
}
```

---

### **How This Works**

1. **Recursive Divide-and-Conquer**:
   - The price data is divided into smaller chunks recursively.
   - Each chunk is processed independently once it fits into the cache.

2. **Base Case**:
   - When the chunk size is small enough (e.g., `ChunkSize = 8`), the algorithm directly computes the moving average for the chunk.

3. **Combining Results**:
   - At each recursive step, the algorithm ensures that the moving averages for overlapping regions (at the split boundaries) are computed correctly.

4. **Final Output**:
   - The resulting vector contains the moving averages for all valid indices.

---

### **Why Cache-Oblivious Algorithms Work Better Here**

1. **Optimized Cache Usage**:
   - Recursive subdivision ensures that smaller chunks of data fit into the CPU cache.
   - This minimizes cache misses compared to sequential processing of the entire dataset.

2. **Scalability**:
   - Works efficiently for datasets of any size, regardless of hardware-specific cache parameters.

3. **Hardware Independence**:
   - Unlike explicitly cache-friendly techniques (e.g., block-based processing tuned for specific cache sizes), this approach adapts automatically to different memory hierarchies.

---

### **Advantages Over Traditional Techniques**

| **Aspect**              | **Cache-Oblivious Algorithm**                                      | **Traditional Cache-Friendly Techniques**                 |
|--------------------------|-------------------------------------------------------------------|----------------------------------------------------------|
| **Cache Tuning**         | No explicit tuning required; adapts to any cache size.           | Requires manual tuning of block size for specific hardware. |
| **Performance**          | Efficient across all levels of the memory hierarchy.             | Performance drops if block size does not match cache.     |
| **Flexibility**          | Works for varying dataset sizes without adjustments.             | May require retuning for larger datasets.                |
| **Complexity**           | Slightly more complex due to recursive logic.                    | Simpler logic but requires hardware-specific optimizations. |

---

### **Applications in Real-World Trading**

This approach is well-suited for tasks like:
1. **Historical Data Backtesting**:
   - Simulating strategies over large datasets of price and trade data.
2. **Risk Metrics Calculation**:
   - Computing rolling metrics like volatility or moving averages on historical positions.
3. **Post-Trade Analytics**:
   - Aggregating and normalizing trade data for performance evaluation.

---

### **Conclusion**

A **cache-oblivious algorithm** is a great fit for historical data processing in trading systems, where large datasets need to be traversed and processed efficiently. This approach outperforms traditional techniques when:
1. The dataset size is large.
2. Hardware-specific tuning is not feasible or desirable.
3. Scalability and portability are critical. 

The moving average example demonstrates its practical application and advantages in real-world trading scenarios.
