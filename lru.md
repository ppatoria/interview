# Explanation of LRU Cache

#### **What is LRU Cache?**
An **LRU (Least Recently Used) Cache** is a data structure that manages a fixed amount of memory or storage. It keeps track of the order in which items are accessed and evicts the least recently used item when the cache reaches its maximum capacity.

#### **Purpose**
The main purpose of an LRU cache is to optimize access to frequently used data, reducing the need to recompute or fetch data from a slower storage medium (e.g., a database, file system, or network).

#### **Usage in Trading Applications**
In trading systems, LRU Cache can be useful in scenarios where:
1. **Market Data Caching**: Frequently accessed market data (e.g., stock prices, bid-ask spreads) can be cached for quick retrieval instead of repeatedly fetching from external APIs.
2. **Order Book Management**: Recent trade orders or book snapshots can be stored in the cache.
3. **Indicators and Metrics**: Pre-computed indicators like moving averages or Bollinger Bands can be cached to improve performance during high-frequency trading.
4. **Historical Data Retrieval**: Frequently queried historical data can be cached to reduce I/O latency.

---

# Implementations
## An implementation using a combination of a doubly linked list and an unordered map (hash table):
```c++

#include <iostream>
#include <list>
#include <unordered_map>
using namespace std;

class LRUCache {
private:
    int capacity;
    list<pair<int, int>> cacheList; // Doubly linked list to store (key, value) pairs
    unordered_map<int, list<pair<int, int>>::iterator> cacheMap; // Map of key to list iterator

    // Move an accessed node to the front of the list
    void moveToFront(list<pair<int, int>>::iterator it) {
        // Explanation of splice:
        // splice(position, source_list, iterator)
        // - Moves the element pointed to by `iterator` from `source_list` to the position `position` in the same or another list.
        // - This is done in constant time because it only adjusts the internal pointers of the doubly linked list.
        // - No element copying or allocation occurs during the operation.

        // `cacheList.begin()` specifies the position to move to (front of the list).
        // `it` is the iterator pointing to the node being moved.
        cacheList.splice(cacheList.begin(), cacheList, it);

        // Internally:
        // 1. Removes the node at `it` from its current position:
        //    - Updates the `next` pointer of the previous node and the `prev` pointer of the next node to skip this node.
        // 2. Inserts the node at `cacheList.begin()`:
        //    - Updates the `next` pointer of this node to point to the old first node.
        //    - Updates the `prev` pointer of the old first node to point to this node.
        //    - Updates the `prev` pointer of this node to point to the dummy head (internally managed by `std::list`).
    }

public:
    LRUCache(int cap) : capacity(cap) {}

    // Get the value of a key if it exists in the cache
    int get(int key) {
        if (cacheMap.find(key) != cacheMap.end()) {
            auto it = cacheMap[key];
            moveToFront(it); // Move accessed node to the front
            return it->second;
        }
        return -1; // Key not found
    }

    // Put a key-value pair into the cache
    void put(int key, int value) {
        if (cacheMap.find(key) != cacheMap.end()) {
            // Key exists, update value and move to front
            auto it = cacheMap[key];
            it->second = value;
            moveToFront(it);
        } else {
            // Key does not exist, insert new key-value pair
            if (cacheList.size() == capacity) {
                // Cache is full, remove least recently used (back of the list)
                auto lru = cacheList.back();
                cacheMap.erase(lru.first);
                cacheList.pop_back();
            }
            // Add new node to the front of the list
            cacheList.emplace_front(key, value);
            cacheMap[key] = cacheList.begin();
        }
    }
};

int main() {
    LRUCache lru(3);

    lru.put(1, 10);
    lru.put(2, 20);
    lru.put(3, 30);

    cout << lru.get(1) << endl; // Outputs 10
    lru.put(4, 40);             // Evicts key 2
    cout << lru.get(2) << endl; // Outputs -1
    lru.put(5, 50);             // Evicts key 3
    cout << lru.get(3) << endl; // Outputs -1

    return 0;
}

```


---

### **Key Components**
1. **Doubly Linked List**:
   - Stores the key-value pairs.
   - Allows quick insertion, deletion, and reordering.

2. **Hash Map**:
   - Maps keys to their corresponding iterator in the linked list.
   - Enables \(O(1)\) lookups to check if a key exists.

3. **Eviction Policy**:
   - When the cache is full, the least recently used (back of the list) item is evicted.

---

This implementation ensures \(O(1)\) time complexity for both `get` and `put` operations, making it efficient for trading applications where low latency is critical.




---

### **Time Complexity**
1. **Get Operation (`get`)**:
   - **Lookup in `std::unordered_map`**: \( O(1) \) average-case time to locate the iterator of the list corresponding to the key.
   - **Reordering the element**: \( O(1) \) using `std::list::splice`, as it only involves pointer manipulation to move the element to the front of the list.
   - **Total**: \( O(1) \).

2. **Put Operation (`put`)**:
   - **Lookup in `std::unordered_map`**: \( O(1) \) average-case time to check if the key exists.
   - **Insertion in `std::list`**:
     - **When key already exists**: Update the value and move the element to the front using `splice`, which is \( O(1) \).
     - **When key is new**: Insert at the front of the list using `emplace_front`, which is \( O(1) \).
   - **Eviction**:
     - If the cache is full, the least recently used element (at the back of the list) is removed. Removing the back element from `std::list` is \( O(1) \), and erasing it from the `std::unordered_map` is also \( O(1) \).
   - **Total**: \( O(1) \) for both existing and new keys.

---

### **Space Complexity**
1. **`std::list`**:
   - Stores at most \( n \) elements (key-value pairs).
   - Each node in the list requires space for the key, the value, and two pointers (next and previous).
   - **Space Usage**: \( O(n) \).

2. **`std::unordered_map`**:
   - Stores \( n \) key-value pairs, where the value is an iterator pointing to a node in the list.
   - The hash table itself may require some extra space for internal bookkeeping and potential resizing.
   - **Space Usage**: \( O(n) \).

3. **Total Space Complexity**:
   - **List**: \( O(n) \).
   - **Hash Map**: \( O(n) \).
   - Combined: \( O(n) \).

---

### **Summary**
- **Time Complexity**:
  - `get`: \( O(1) \)
  - `put`: \( O(1) \)
- **Space Complexity**:
  - \( O(n) \), where \( n \) is the cache capacity (maximum number of key-value pairs the cache can hold).
  
## Implementation cache friendly LRU using deque
Yes, the current **LRU Cache** implementation using `std::list` and `std::unordered_map` is not cache-friendly because the data in `std::list` is not stored contiguously in memory, leading to poor CPU cache utilization. To make the implementation **cache-friendly**, we can replace the linked list with a **vector** or a **deque**, as these store data contiguously or in a more cache-optimized manner.

---

### **Cache-Friendly LRU Cache Implementation**
To make the LRU Cache cache-friendly:
1. Replace `std::list` with `std::vector` or `std::deque` to ensure contiguous or semi-contiguous storage.
2. Use a **map of indices** instead of a map of iterators to reference elements in the vector.

#### **Key Changes**
- Use a `std::vector` or `std::deque` to store key-value pairs.
- Maintain the order of elements explicitly using an index instead of relying on pointers.
- Move the most recently used elements to the end of the vector for efficient eviction.

---

### **Drawbacks of Using `std::vector`**
1. Moving elements (e.g., when reordering) has a time complexity of \( O(n) \), which can increase the cost of operations.
2. Maintaining the LRU property requires more explicit management, such as frequent shifting of elements.

---

### **Implementation Using `std::deque`**
A better cache-friendly approach is to use `std::deque`, which provides:
1. \( O(1) \) insertion/removal at both ends.
2. Semi-contiguous storage (better than `std::list` for cache locality).

Here's a **cache-friendly LRU Cache implementation** using `std::deque`:

```cpp
#include <iostream>
#include <unordered_map>
#include <deque>
using namespace std;

class LRUCache {
private:
    int capacity;
    deque<pair<int, int>> dq; // Store key-value pairs
    unordered_map<int, deque<pair<int, int>>::iterator> cacheMap; // Map key to iterator in deque

public:
    LRUCache(int cap) : capacity(cap) {}

    int get(int key) {
        if (cacheMap.find(key) == cacheMap.end()) {
            return -1; // Key not found
        }
        // Move accessed key to the front of the deque
        auto it = cacheMap[key];
        int value = it->second;
        dq.erase(it);
        dq.emplace_front(key, value);
        cacheMap[key] = dq.begin();
        return value;
    }

    void put(int key, int value) {
        if (cacheMap.find(key) != cacheMap.end()) {
            // Key exists, update its value and move it to the front
            auto it = cacheMap[key];
            dq.erase(it);
        } else if (dq.size() == capacity) {
            // Cache is full, remove the least recently used item
            auto lru = dq.back();
            cacheMap.erase(lru.first);
            dq.pop_back();
        }
        // Add the new key-value pair to the front
        dq.emplace_front(key, value);
        cacheMap[key] = dq.begin();
    }
};

int main() {
    LRUCache lru(3);

    lru.put(1, 10);
    lru.put(2, 20);
    lru.put(3, 30);

    cout << lru.get(1) << endl; // Outputs 10
    lru.put(4, 40);             // Evicts key 2
    cout << lru.get(2) << endl; // Outputs -1
    lru.put(5, 50);             // Evicts key 3
    cout << lru.get(3) << endl; // Outputs -1

    return 0;
}
```

---

### **How It Is Cache-Friendly**
1. **Data Storage**:
   - `std::deque` provides semi-contiguous memory, which is better for CPU caching compared to a linked list.
2. **Access Pattern**:
   - Keys are accessed and moved efficiently to the front or back without pointer dereferencing.

---

### **Time and Space Complexity**
- **Time Complexity**:
  - `get`: \( O(n) \) in the worst case due to potential shifting.
  - `put`: \( O(n) \) in the worst case if elements need to be moved.
- **Space Complexity**:
  - Same as before: \( O(n) \).

### **Summary**
Switching to a `std::deque` makes the cache more CPU cache-friendly at the cost of slightly higher operation times compared to `std::list`. For even better performance, a custom implementation using a contiguous array (like a ring buffer) combined with manual indexing may further optimize cache utilization.

## Comparison of efficency of different implementations.
You're right that there are inefficiencies in the circular buffer implementation for LRU, especially in terms of rearrangement when accessing elements. To help clarify the trade-offs, let's compare the three approaches in terms of **time efficiency**, **space efficiency**, and **cache friendliness**.

### **1. List with Unordered Map (LRU using `std::list` + `std::unordered_map`)**

#### **Time Efficiency**:
- **Insertion (`put`)**: \( O(1) \)
  - Insertion at the front of the list is efficient due to the doubly-linked nature of `std::list`.
- **Access (`get`)**: \( O(1) \)
  - Direct access to elements via the unordered map (`std::unordered_map`) is \( O(1) \).
  - Moving an element to the front also takes \( O(1) \), as `std::list` allows constant time insertion/removal at both ends.
- **Eviction**: \( O(1) \)
  - Eviction is also \( O(1) \) because it involves removing an element from the back of the list.

#### **Space Efficiency**:
- **Memory Overhead**: Moderate
  - The `std::list` requires extra memory for the links (pointers) between elements, which increases memory usage compared to a simple array or buffer.
  - The unordered map adds memory overhead for managing hash table buckets.

#### **Cache Friendliness**:
- **Cache Efficiency**: **Low**
  - `std::list` elements are scattered in memory due to the doubly linked nature, meaning frequent accesses can cause cache misses.

---

### **2. Deque with Unordered Map (LRU using `std::deque` + `std::unordered_map`)**

#### **Time Efficiency**:
- **Insertion (`put`)**: \( O(1) \)
  - Insertion at either end is \( O(1) \), which makes it very efficient.
- **Access (`get`)**: \( O(1) \)
  - Access via `unordered_map` is \( O(1) \), and moving elements around is relatively efficient in the deque.
- **Eviction**: \( O(1) \)
  - Eviction (removal of elements from the front) is efficient because `std::deque` allows \( O(1) \) operations at both ends.

#### **Space Efficiency**:
- **Memory Overhead**: Moderate to High
  - `std::deque` allocates blocks for elements, which may cause additional overhead due to the allocation of multiple blocks.
  - The unordered map also requires memory for the key-value mapping.
  - Unlike `std::list`, the blocks in `std::deque` help reduce memory fragmentation.

#### **Cache Friendliness**:
- **Cache Efficiency**: **Moderate**
  - `std::deque` provides better cache locality compared to `std::list` because its elements are stored in contiguous memory blocks.
  - However, it is still not as cache-friendly as a completely contiguous buffer due to the multiple memory blocks.

---

### **3. Circular Buffer with Unordered Map (LRU using Circular Buffer + `std::unordered_map`)**

#### **Time Efficiency**:
- **Insertion (`put`)**: \( O(1) \)
  - Insertions are fast because the buffer is pre-allocated, and only the index needs to be updated.
- **Access (`get`)**: \( O(1) \)
  - Access is fast, but it requires rearranging the elements on every access, which can result in inefficient operations for frequent access.
  - Rearrangement of elements on every access is \( O(n) \), where \( n \) is the number of elements, because each accessed element needs to be moved to the front.
- **Eviction**: \( O(1) \)
  - Eviction is \( O(1) \ because the last accessed element is replaced by the new one, and the index is simply updated.

#### **Space Efficiency**:
- **Memory Overhead**: Low
  - The circular buffer is a fixed-size array, so no extra memory is required for pointers, as with `std::list` or `std::deque`.
  - The unordered map requires memory for the key-value mappings, but it does not introduce the overhead associated with multiple memory allocations.

#### **Cache Friendliness**:
- **Cache Efficiency**: **High**
  - Since the data is stored in a contiguous memory array, a circular buffer is much more cache-friendly than `std::list` or `std::deque`.
  - The memory locality improves, as accessing contiguous blocks of memory is faster for the CPU cache.

---

### **Comparison Summary**

| **Approach**                             | **Time Efficiency**                     | **Space Efficiency**                   | **Cache Friendliness** |
|------------------------------------------|-----------------------------------------|----------------------------------------|------------------------|
| **List with Unordered Map**              | \( O(1) \) for insertion, access, eviction | Moderate (due to extra pointers)       | Low (scattered memory) |
| **Deque with Unordered Map**             | \( O(1) \) for insertion, access, eviction | Moderate to high (due to block allocation) | Moderate (better than list, but not contiguous) |
| **Circular Buffer with Unordered Map**   | \( O(1) \) for insertion, eviction, but \( O(n) \) for access (due to rearrangement) | Low (fixed-size buffer)               | High (contiguous memory) |

### **Conclusion**:
- **Best for time and space efficiency**: The **List with Unordered Map** approach is generally the most time-efficient due to its constant time access and minimal rearrangement, but it has higher memory overhead due to the doubly linked structure.
- **Best for cache friendliness**: The **Circular Buffer** approach provides the best cache locality since the data is contiguous, but rearranging elements on every access makes it inefficient for frequent access patterns.
- **Balanced approach**: **Deque with Unordered Map** strikes a balance between time efficiency and cache friendliness, but still incurs some overhead due to the non-contiguous nature of blocks. It's a good choice when you need efficient insertions and deletions at both ends while maintaining better cache performance than a `std::list`.

For LRU caches, if **cache efficiency** is a priority, a **circular buffer** is ideal, but for applications with frequent accesses and fewer insertions/removals, the **list with unordered map** approach might be a better fit.
You're absolutely correct! 

In real-world applications, **O(n) for access** due to rearrangement is generally **not acceptable**, even if the data structure is cache-friendly. Here's why:

### **Why O(n) for Access Is Inefficient:**

1. **Frequent Accesses**:
   - In an **LRU Cache**, you often need to access elements multiple times. If each access involves rearranging a large portion of the data (which is \( O(n) \)), it can significantly slow down the system, especially as the cache grows in size.

2. **Impact on Performance**:
   - If the **LRU cache** is used in performance-critical applications (such as trading algorithms or web applications), the cost of rearranging elements on every access will quickly add up. The rearrangement involves shifting elements, which defeats the purpose of having **efficient \( O(1) \) access** for typical cache operations.

3. **Increased Latency**:
   - While a **circular buffer** is cache-friendly (due to its contiguous memory layout), the fact that every access requires \( O(n) \) rearrangement means that the cache's overall response time will become slower as the number of elements increases, particularly when you have frequent accesses.

### **Ideal Time Complexity for LRU Cache:**

- **O(1) for Insertion, Access, and Eviction**:
   - In a well-optimized LRU cache, **all operations**—insertion, access, and eviction—should ideally be **O(1)**.
   - This is achieved by **not rearranging** elements on every access. Instead, each operation (insertion, access, eviction) should only involve **constant-time manipulations** (like moving elements to the front or back), with no need to traverse or shift large portions of the data structure.

### **Real-World Acceptability**:

- While **cache friendliness** is important, **time efficiency** is paramount in many real-world scenarios, especially when you have high-frequency access patterns.
- If your cache requires rearranging elements in **O(n)** time on every access, the time spent on each access would be inefficient, even with great cache locality.

Thus, even though **circular buffers** are **cache-friendly**, they **should not be used** for LRU caches unless you can eliminate the \( O(n) \) rearrangement cost on each access. **List with unordered map** (or **deque with unordered map**) is far more practical for such scenarios because it guarantees **O(1) access time** while still offering reasonable cache performance (though not as good as a circular buffer).





# Real world usage in Trading Applications:
In **low-latency trading applications**, **LRU (Least Recently Used) Caches** play a crucial role in managing data and ensuring that the system operates at peak efficiency without exceeding memory limits. Here's a real-world example of how LRU caches are used in such systems:

## **Real-World Example: LRU Cache for Storing Market Data in a Trading System**

#### **Scenario**:
A trading firm develops a **high-frequency trading (HFT) algorithm** that needs to make rapid decisions based on real-time market data such as stock prices, bid/ask spreads, volume, etc.
The algorithm must process this data with very low latency to react to market movements in milliseconds or microseconds. 

The trading system has limited **memory capacity** to store market data (for example, data for thousands of different stocks or financial instruments), and **processing speed** is crucial. The LRU cache can be used to efficiently manage which stock data is kept in memory, ensuring that only the most relevant and recently accessed data is available for the algorithm.

#### **How LRU Cache is Used**:

1. **Problem**:
   - The trading system needs to keep track of live market data for multiple instruments (e.g., thousands of stocks), but it has limited memory to store all this information.
   - The data for all instruments cannot be stored in memory due to **space limitations** and **latency requirements**.
   - Older data or data that hasn’t been accessed for a while (e.g., for less-traded stocks) should be evicted to make space for new or frequently accessed data.

2. **LRU Cache Implementation**:
   - An **LRU cache** is set up to store market data for a fixed number of instruments (e.g., 1000 stocks).
   - The cache is managed using a combination of **unordered_map** (for fast lookups by stock symbol) and a **doubly linked list** (to maintain the order of access).
   - When new data for a stock is received, it is inserted into the cache.
   - When a stock’s data is accessed (e.g., the algorithm queries it), that stock’s data is moved to the **front of the list**.
   - If the cache exceeds its capacity (e.g., 1000 stocks), the **least recently accessed stock** (the one at the **back** of the list) is **evicted** to free up space for new data.

3. **Benefits in Low-Latency Trading**:
   - **Efficient Memory Use**: Only the most recent and relevant market data is kept in memory, ensuring that memory usage is optimal without overloading the system.
   - **Low Latency**: The LRU cache ensures that data retrieval (via the unordered_map) is **O(1)**, so the algorithm can access market data in constant time, which is crucial in a high-frequency trading environment where **microseconds matter**.
   - **Eviction of Unused Data**: Old and unused data is evicted automatically, allowing the system to adapt dynamically to changing market conditions. If a stock hasn't been traded for a while or its data hasn't been accessed, it's evicted in favor of more active instruments.
   - **Cache Miss Handling**: When new market data is needed (i.e., the cache doesn't have it), the trading algorithm can request it from the exchange or data provider. In such cases, the cache's eviction policy ensures that only relevant data stays in memory.

#### **Example Use Case: Trading Algorithm Decision Making**
Imagine a **market-making algorithm** that is responsible for quoting prices for a basket of stocks. The algorithm needs to constantly update its prices based on real-time bid/ask spreads, last trades, and volume data.

- **Access Patterns**: The algorithm tends to focus on stocks that are actively being traded, and the focus shifts from stock to stock based on market movements.
  - Stocks that have large price fluctuations or high trading volumes are accessed frequently.
  - Stocks that haven’t moved in a while or are less active are accessed less often.
  
- **LRU Cache in Action**:
  - When the algorithm needs to update its price quotes, it first checks the LRU cache to see if the relevant market data is already available.
  - If the data is in the cache, it’s quickly retrieved with **O(1)** time complexity, and the stock's data is moved to the front of the cache.
  - If the data isn't in the cache (i.e., a cache miss), the algorithm queries the exchange for the latest price and then stores the data in the cache, potentially evicting the least recently used stock data to make room.
  
This approach ensures that the algorithm can make trading decisions with **minimal latency** because the most important data is always readily available in memory.

---

## **Another Example: Order Book Management in HFT**

#### **Scenario**:
In high-frequency trading, managing the order book is another key application where LRU caches are often used.

- **Order Book Data**: The order book stores **buy and sell orders** for financial instruments. It constantly updates as orders are placed, filled, or canceled.
- **Cache Requirements**: The system needs to keep track of the most recent order book snapshots but has limited memory to store all of them.
  
#### **LRU Cache Usage**:
- The system uses an **LRU cache** to store the most recent order book snapshots for the instruments being traded.
- When a new order comes in, the cache is updated, and the relevant order book data is moved to the front.
- If the order book data is not accessed for a certain period, it is evicted to make space for new, more relevant data.

This ensures that the algorithm has the most up-to-date order book data for decision-making while keeping memory usage optimal.

---

### **Key Takeaways in Low-Latency Trading**:
- **Real-Time Data Management**: LRU caches provide an efficient way to manage frequently accessed real-time data, ensuring low-latency performance.
- **Memory Optimization**: By evicting the least recently used data, the cache ensures optimal use of available memory, which is especially important in systems with high throughput and limited resources.
- **Scalability**: In fast-moving markets, the cache can scale to handle increasing data throughput while maintaining constant-time access to the most relevant information.

In conclusion, **LRU caches** are a fundamental tool in ensuring that trading algorithms operate efficiently and quickly by keeping the most relevant market data in memory, allowing them to make decisions with minimal delay.
# Implementation of LRU for OrderBook
To make the `MultiInstrumentOrderBook` more efficient with an **LRU cache**, we can integrate an LRU mechanism to manage **active order books**. This is particularly useful when the trading application deals with a large number of instruments but only a subset is actively traded at any given time.

The **idea** is to keep the order books for the most recently traded instruments in memory, while the least recently accessed order books can be evicted or persisted to a secondary storage system. This reduces memory usage while maintaining fast access to the most relevant data.

---

### **Why Use LRU for the Order Book?**

1. **Memory Optimization**:
   - The application may deal with thousands of instruments, but keeping all order books in memory is inefficient and expensive.
   - Instruments that are not actively traded do not need to occupy memory.

2. **Low Latency**:
   - The LRU cache ensures that order books for the most recently traded instruments are readily available in memory, providing fast access.

3. **Eviction of Inactive Books**:
   - Order books for less frequently traded instruments are evicted when the cache reaches capacity, freeing up memory.

4. **Cache Management**:
   - When an evicted order book becomes active again, it can be reloaded from secondary storage or initialized fresh.

---

### **How LRU Works in This Context**

1. **LRU Integration**:
   - Use an LRU cache to store order books for instruments. Each time an order book is accessed or updated, it is moved to the front of the LRU.

2. **Eviction**:
   - When the cache exceeds its capacity (e.g., maximum number of instruments in memory), the least recently accessed order book is evicted.

3. **Rehydration**:
   - If an evicted order book becomes active again (e.g., an order arrives for an inactive instrument), it can be reloaded into memory.

---

### **Modified Implementation with LRU Cache**

Here’s the code with LRU integration:
Here’s the modularized implementation of the **LRU Cache** for the above **MultiInstrumentOrderBook** using `deque` and `unordered_map`. The LRU will help in maintaining recently used orders for quick retrieval, especially in cases where frequently accessed or modified orders (like cancellations or amendments) need fast access.

### Modular Implementation

#### Code
```cpp
#include <unordered_map>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <deque>
#include <algorithm>

// Order structure
struct Order {
    int orderId;
    std::string type; // "buy" or "sell"
    int quantity;
    double price;
    long timestamp;
    std::string instrument; // Instrument name

    Order(int id, const std::string& t, int q, double p, long ts, const std::string& instr)
        : orderId(id), type(t), quantity(q), price(p), timestamp(ts), instrument(instr) {}
};

// LRU Cache to track most recently used orders
class LRUCache {
private:
    int capacity;
    std::unordered_map<int, std::deque<Order>::iterator> orderMap; // Maps orderId to deque iterator
    std::deque<Order> orderDeque; // Holds orders in LRU order

public:
    LRUCache(int cap) : capacity(cap) {}

    // Add or update an order in the cache
    void accessOrder(const Order& order) {
        if (orderMap.find(order.orderId) != orderMap.end()) {
            // Move accessed order to the front
            orderDeque.erase(orderMap[order.orderId]);
        } else if (orderDeque.size() == capacity) {
            // Remove the least recently used order
            orderMap.erase(orderDeque.back().orderId);
            orderDeque.pop_back();
        }
        orderDeque.push_front(order);
        orderMap[order.orderId] = orderDeque.begin();
    }

    // Remove an order from the cache
    void removeOrder(int orderId) {
        if (orderMap.find(orderId) != orderMap.end()) {
            orderDeque.erase(orderMap[orderId]);
            orderMap.erase(orderId);
        }
    }

    // Check if an order is in the cache
    bool containsOrder(int orderId) const {
        return orderMap.find(orderId) != orderMap.end();
    }

    // Retrieve an order (if available)
    const Order* getOrder(int orderId) const {
        if (orderMap.find(orderId) != orderMap.end()) {
            return &(*orderMap.at(orderId));
        }
        return nullptr;
    }
};

// OrderBook structure
class OrderBook {
private:
    std::map<double, std::vector<Order>, std::greater<double>> buyOrders;
    std::map<double, std::vector<Order>, std::less<double>> sellOrders;

public:
    void addOrder(const Order& order) {
        if (order.type == "buy") {
            buyOrders[order.price].push_back(order);
        } else if (order.type == "sell") {
            sellOrders[order.price].push_back(order);
        }
    }

    void matchOrders() {
        while (!buyOrders.empty() && !sellOrders.empty()) {
            auto& highestBuy = buyOrders.begin();
            auto& lowestSell = sellOrders.begin();

            if (highestBuy->first >= lowestSell->first) {
                auto& buyOrdersAtPrice = highestBuy->second;
                auto& sellOrdersAtPrice = lowestSell->second;

                while (!buyOrdersAtPrice.empty() && !sellOrdersAtPrice.empty()) {
                    Order& buyOrder = buyOrdersAtPrice.back();
                    Order& sellOrder = sellOrdersAtPrice.back();

                    int matchedQty = std::min(buyOrder.quantity, sellOrder.quantity);

                    std::cout << "Matched Buy Order " << buyOrder.orderId
                              << " with Sell Order " << sellOrder.orderId
                              << ", Quantity: " << matchedQty << "\n";

                    buyOrder.quantity -= matchedQty;
                    sellOrder.quantity -= matchedQty;

                    if (buyOrder.quantity == 0) buyOrdersAtPrice.pop_back();
                    if (sellOrder.quantity == 0) sellOrdersAtPrice.pop_back();
                }

                if (buyOrdersAtPrice.empty()) buyOrders.erase(highestBuy);
                if (sellOrdersAtPrice.empty()) sellOrders.erase(lowestSell);
            } else {
                break;
            }
        }
    }

    void printOrders() const {
        std::cout << "Buy Orders:\n";
        for (const auto& [price, orders] : buyOrders) {
            std::cout << "  Price: " << price << "\n";
            for (const auto& order : orders) {
                std::cout << "    OrderID: " << order.orderId << ", Quantity: " << order.quantity << "\n";
            }
        }

        std::cout << "Sell Orders:\n";
        for (const auto& [price, orders] : sellOrders) {
            std::cout << "  Price: " << price << "\n";
            for (const auto& order : orders) {
                std::cout << "    OrderID: " << order.orderId << ", Quantity: " << order.quantity << "\n";
            }
        }
    }
};

// MultiInstrumentOrderBook structure
class MultiInstrumentOrderBook {
private:
    std::unordered_map<std::string, OrderBook> books;
    LRUCache lruCache;

public:
    MultiInstrumentOrderBook(int lruCapacity) : lruCache(lruCapacity) {}

    void addOrder(const Order& order) {
        books[order.instrument].addOrder(order);
        lruCache.accessOrder(order);
        books[order.instrument].matchOrders();
    }

    void cancelOrder(int orderId) {
        if (lruCache.containsOrder(orderId)) {
            const Order* cachedOrder = lruCache.getOrder(orderId);
            if (cachedOrder) {
                std::cout << "Canceling OrderID: " << cachedOrder->orderId << "\n";
                lruCache.removeOrder(orderId);
            }
        } else {
            std::cout << "OrderID " << orderId << " not found in cache.\n";
        }
    }

    void printOrderBooks() const {
        for (const auto& [instrument, book] : books) {
            std::cout << "Instrument: " << instrument << "\n";
            book.printOrders();
        }
    }
};

// Main function
int main() {
    MultiInstrumentOrderBook multiBook(5); // LRU Cache with capacity 5

    multiBook.addOrder(Order(1, "buy", 100, 50.5, 1000, "AAPL"));
    multiBook.addOrder(Order(2, "sell", 50, 49.5, 1001, "AAPL"));
    multiBook.addOrder(Order(3, "buy", 150, 200.0, 1002, "GOOG"));
    multiBook.addOrder(Order(4, "sell", 75, 199.5, 1003, "GOOG"));
    multiBook.addOrder(Order(5, "buy", 200, 300.0, 1004, "MSFT"));

    multiBook.printOrderBooks();

    multiBook.cancelOrder(3);
    multiBook.cancelOrder(6);

    return 0;
}
```

---

### Explanation
1. **LRU Cache**:
   - Maintains recently accessed orders using a `deque` for order and an `unordered_map` for fast lookups.
   - On access, the order is moved to the front of the deque.
   - On exceeding capacity, the least recently used order (at the back) is removed.

2. **OrderBook Matching**:
   - Matches `buy` and `sell` orders for each instrument based on price levels.
   - Ensures efficient trade execution.

3. **Modularization**:
   - `LRUCache` is a separate class for managing order recency.
   - `OrderBook` handles price levels and matching logic for each instrument.
   - `MultiInstrumentOrderBook` combines LRU and order management for multi-instrument functionality.

This approach ensures clean separation of concerns and optimizes order access using LRU.

---
