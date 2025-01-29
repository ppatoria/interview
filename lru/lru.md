# Using Linked List:
Here’s a **very simple implementation** of an LRU Cache in C++ without templates or additional complexities. It uses a combination of a **hash map** and a **doubly linked list** for efficient operations.

### Implementation

```cpp

#include <iostream>
#include <unordered_map>
#include <list>

class LRUCache {
private:
    int capacity; // Maximum capacity of the cache
    std::list<std::pair<int, int>> lruList; // Doubly linked list: {key, value}
    std::unordered_map<int, std::list<std::pair<int, int>>::iterator> cache; // Key -> List iterator

    void moveToFront(int key) {
    /**
     * std::list::splice simply adjusts the internal pointers of the linked list nodes without changing or invalidating iterators.
     * This is one of the key benefits of using `std::list` in this implementation.
     *   - No additional update to the `cache` is needed because the iterator in `cache[key]` remains valid and points to the correct node after the `splice`.
     *   - This behavior ensures that the `moveToFront` function works efficiently with constant-time complexity ( O(1) ).
     */
        lruList.splice(lruList.begin(), lruList, cache[key]);
    }

public:
    LRUCache(int cap) : capacity(cap) {}
    
    int get(int key) {
        if (!cache.contains(key)) { // Check if the key exists
            return -1; // Key not found
        }
        moveToFront(key); // Mark as most recently used
        return cache[key]->second; // Return the value
    }

    void put(int key, int value) {
        if (cache.contains(key)) { // Key already exists
            cache[key]->second = value;
            moveToFront(key);
        } else {
            if (lruList.size() == capacity) {
                // Remove the least recently used item (back of the list)
                int lruKey = lruList.back().first;
                lruList.pop_back();
                cache.erase(lruKey);
            }
            // Add the new key-value pair to the front of the list
            lruList.emplace_front(key, value);
            cache[key] = lruList.begin();
        }
    }
};

int main() {
    LRUCache cache(3); // Create an LRU cache with capacity 3

    cache.put(1, 10);
    cache.put(2, 20);
    cache.put(3, 30);
    std::cout << "Get 1: " << cache.get(1) << std::endl; // Output: 10

    cache.put(4, 40); // Evicts key 2
    std::cout << "Get 2: " << cache.get(2) << std::endl; // Output: -1 (not found)
    std::cout << "Get 3: " << cache.get(3) << std::endl; // Output: 30

    cache.put(5, 50); // Evicts key 1
    std::cout << "Get 1: " << cache.get(1) << std::endl; // Output: -1 (not found)
    std::cout << "Get 4: " << cache.get(4) << std::endl; // Output: 40
    std::cout << "Get 5: " << cache.get(5) << std::endl; // Output: 50

    return 0;
}


```

### Explanation:
1. **Node Structure**:
   - Each node stores a `key` and `value` with pointers to the previous and next nodes for doubly linked list functionality.

2. **Core Methods**:
   - **`get(key)`**:
     - Checks if the key exists in the cache.
     - If it exists, moves the node to the head (marking it as most recently used) and returns its value.
     - If not, returns `-1`.
   - **`put(key, value)`**:
     - If the key already exists, updates the value and moves the node to the head.
     - If the key doesn’t exist:
       - Checks if the cache is full.
       - If full, removes the least recently used node (from the tail) and adds the new node to the head.
       - If not full, simply adds the new node to the head.

3. **Efficiency**:
   - `get` and `put` both run in **O(1)** time due to the combination of the hash map (for quick key lookups) and the doubly linked list (for fast updates).

4. **Memory Management**:
   - Dynamically allocated nodes are properly deleted in the destructor to avoid memory leaks.

### Output:
```
Get 1: 10
Get 2: -1
Get 3: 30
Get 1: -1
Get 4: 40
Get 5: 50
``` 

# Using dqueue
Here's an implementation of the **LRU Cache** using a `std::deque` instead of a doubly linked list, followed by a comparison of the two designs in terms of time complexity, space complexity, and cache-friendliness.

---

### **Implementation Using `std::deque`**

```cpp
#include <iostream>
#include <unordered_map>
#include <deque>

class LRUCache {
private:
    int capacity;
    std::deque<int> dq;  // Stores keys in order of usage
    std::unordered_map<int, std::pair<int, std::deque<int>::iterator>> cache;

    void moveToFront(int key) {
        dq.erase(cache[key].second); // Remove key from its current position
        dq.push_front(key);          // Add key to the front of the deque
        cache[key].second = dq.begin(); // Update iterator in the map
    }

public:
    LRUCache(int cap) : capacity(cap) {}

    int get(int key) {
        if (cache.find(key) == cache.end()) {
            return -1; // Key not found
        }
        moveToFront(key); // Mark as most recently used
        return cache[key].first; // Return the value
    }

    void put(int key, int value) {
        if (cache.find(key) != cache.end()) {
            // Key already exists, update its value and move to front
            cache[key].first = value;
            moveToFront(key);
        } else {
            // Key does not exist
            if (dq.size() == capacity) {
                // Cache is full, evict least recently used key
                int lru = dq.back();
                dq.pop_back();
                cache.erase(lru);
            }
            // Add new key to the cache
            dq.push_front(key);
            cache[key] = {value, dq.begin()};
        }
    }
};

int main() {
    LRUCache cache(3); // Create an LRU cache with capacity 3

    cache.put(1, 10);
    cache.put(2, 20);
    cache.put(3, 30);
    std::cout << "Get 1: " << cache.get(1) << std::endl; // Output: 10

    cache.put(4, 40); // Evicts key 2
    std::cout << "Get 2: " << cache.get(2) << std::endl; // Output: -1 (not found)
    std::cout << "Get 3: " << cache.get(3) << std::endl; // Output: 30

    cache.put(5, 50); // Evicts key 1
    std::cout << "Get 1: " << cache.get(1) << std::endl; // Output: -1 (not found)
    std::cout << "Get 4: " << cache.get(4) << std::endl; // Output: 40
    std::cout << "Get 5: " << cache.get(5) << std::endl; // Output: 50

    return 0;
}
```

---

### **Comparison of `std::deque` vs Doubly Linked List**

| **Aspect**                | **`std::deque`**                                  | **Doubly Linked List**                         |
|---------------------------|--------------------------------------------------|-----------------------------------------------|
| **Time Complexity**       |                                                  |                                               |
| - `get(key)`              | `O(n)` (erase operation moves elements)          | `O(1)` (direct node removal via pointer)      |
| - `put(key, value)`       | `O(n)` (erase and push_front may cause shifts)   | `O(1)` (add/remove from head or tail)         |
| **Space Complexity**      | `O(n)` for deque and hash map                    | `O(n)` for list and hash map                  |
| **Cache-Friendliness**    | Relatively better, as deque elements are stored in contiguous memory. | Worse due to scattered allocations.          |
| **Ease of Use**           | Easier: STL-managed memory and iterators         | Requires manual memory management             |
| **Flexibility**           | Limited (no custom allocation)                   | Flexible (can optimize allocator)             |
| **Memory Overhead**       | Slightly higher for `std::deque` due to its internal resizing | Lower, as each node is managed explicitly     |

---

### **Which to Choose?**

1. **Use Doubly Linked List If:**
   - You require **strict O(1)** operations for `get` and `put`.
   - You want finer control over memory allocation and node placement.
   - Cache-friendliness is not a primary concern.

2. **Use `std::deque` If:**
   - You want a simpler, more STL-based solution.
   - Cache-friendliness is critical (e.g., better performance with modern CPU cache hierarchies).
   - You don't mind a slight trade-off in performance (slower `erase`).

---

### Summary
While **`std::deque`** is easier to use and can benefit from **cache-friendliness**, the **doubly linked list** is the better choice for achieving strict **O(1)** complexity in all operations, which is often crucial for high-performance caching scenarios. If memory locality and CPU caching are not significant bottlenecks in your use case, the doubly linked list is generally more efficient for implementing an LRU Cache.
