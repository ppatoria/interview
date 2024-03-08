# How would you implement a cache with a Least Recently Used (LRU) eviction policy?

## Data Structure:
    - Use a **doubly linked list** to store the key-value pairs in the cache.
    - Each node in the linked list represents a cached item and holds the key, value, and pointers to the previous and next nodes.

## LRU Policy:
    - The **head** of the linked list represents the **most recently used** item.
    - The **tail** of the linked list represents the **least recently used** item.

## Operations:**
    - **Get:**
      - Search for the key in the linked list.
        - If found, move the node containing the key to the head (mark it as most recently used).
        - If not found, return "not found".
    - **Put:**
      - If the key already exists, update the value and move the node to the head.
      - If the key doesn't exist and the cache is full, remove the least recently used item (tail) from the list and insert the new key-value pair at the head.
      - If the key doesn't exist and the cache has space, simply insert the new key-value pair at the head.

## Additional Considerations:
    - **Size Limit:** Define a maximum size for the cache to prevent it from growing indefinitely.
    - **Thread Safety:** If the cache is accessed concurrently, implement synchronization mechanisms (like mutexes) to ensure data consistency.

## C++ Implementations:

### C++ implementation of LRU using a doubly linked list:

```
#include <mutex>

class Node {
public:
    int key;
    int value;
    Node* prev;
    Node* next;

    Node(int key, int value) : key(key), value(value), prev(nullptr), next(nullptr) {}
};

class LRUCache {
private:
    Node* head;
    Node* tail;
    int capacity;
    std::mutex mtx;

public:
    LRUCache(int capacity) : capacity(capacity), head(nullptr), tail(nullptr) {}

    void remove(Node* node) {
        if (node == head) {
            head = head->next;
        } else if (node == tail) {
            tail = tail->prev;
        }

        if (node->prev) {
            node->prev->next = node->next;
        }
        if (node->next) {
            node->next->prev = node->prev;
        }
    }

    void addToHead(Node* node) {
        node->next = head;
        if (head) {
            head->prev = node;
        }
        head = node;
        if (!tail) {
            tail = node;
        }
    }

    int get(int key) {
        std::lock_guard<std::mutex> lock(mtx);
        Node* node = findNode(key);
        if (node) {
            remove(node);
            addToHead(node);
            return node->value;
        }
        return -1;
    }

    void put(int key, int value) {
        std::lock_guard<std::mutex> lock(mtx);
        Node* node = findNode(key);
        if (node) {
            node->value = value;
            remove(node);
            addToHead(node);
            return;
        }

        Node* newNode = new Node(key, value);
        addToHead(newNode);

        if (size() > capacity) {
            remove(tail);
            delete tail;
            tail = tail->prev;
        }
    }

private:
    Node* findNode(int key) {
        Node* curr = head;
        while (curr && curr->key != key) {
            curr = curr->next;
        }
        return curr;
    }

    int size() {
        int count = 0;
        Node* curr = head;
        while (curr) {
            count++;
            curr = curr->next;
        }
        return count;
    }
};
```

This implementation includes the following features:

- **Doubly linked list:** Nodes store key, value, and pointers to the previous and next nodes for efficient insertion and removal.
- **Mutex:** Ensures thread safety for concurrent access to the cache.
- **`get(key)`:** Searches for the key, moves it to the head if found, and returns the value.
- **`put(key, value)`:** Inserts a new node if the key doesn't exist, updates the value if it does, and evicts the least recently used node if the cache is full.
- **Helper functions:** `findNode` to locate a node by key, `size` to calculate the number of elements in the list.

Remember to replace `std::mutex` with an appropriate synchronization mechanism for your specific use case and threading environment.

### C++ implementation of LRU using a doubly linked list and Hashtable:

```
#include <iostream>
#include <unordered_map>

using namespace std;

class LRUCache {
public:
    LRUCache(int capacity) : capacity(capacity) {}

    int get(int key) {

        int value = -1;                   /** not found **/

        if (cache.contains(key)) {        /** Move the accessed key to the front of the list **/
            auto it = cache[key];
            value = it->second;
            lru.erase(it);
            lru.push_front({key, value});
            cache[key] = lru.begin();
        **
        return value;
    }

    void put(int key, int value) {
        if (cache.contains(key)) {        /** Update the value of the key **/
            auto it    = cache[key];
            it->second = value;
            lru.erase(it);
            lru.push_front({key, value});
            cache[key] = lru.begin();
        } else {                          /** Insert the new key-value pair **/
            if (lru.size() == capacity) { /** if cache is full remove the least recently used key **/
                int k = lru.back().first;
                cache.erase(k);
                lru.pop_back();
            }
            lru.push_front({key, value});
            cache[key] = lru.begin();
        }
    }

private:
    int capacity;
    list<pair<int, int>> lru;
    unordered_map<int, list<pair<int, int>>::iterator> cache;
};

```

# what is emplace
# iterator invalidation:
Iterator invalidation is a common issue in the C++ standard library.
It occurs when an iterator no longer points to a valid element in a container, or when the state of the container changes in a way that makes the iterator invalid.

For example, if you erase an element from a vector, all iterators and references after the point of erase are invalidated.
Similarly, if you insert an element into a deque, all iterators and references are invalidated, unless the inserted member is at an end (front or back) of the deque.

To avoid iterator invalidation, it's important to be aware of the rules for each container and to use the appropriate container member functions.
In general, invoking a container member function or passing a container as an argument to a library function shall not invalidate iterators to, or change the values of, objects within that container, unless otherwise specified.

## The possible scenarios of iterator invalidation for various containers:

### Insertion Sequence containers:
    * `vector`: All iterators and references before the point of insertion are unaffected, unless the new container size is greater than the previous capacity (in which case all iterators and references are invalidated).
    * `deque`: All iterators and references are invalidated, unless the inserted member is at an end (front or back) of the deque (in which case all iterators are invalidated, but references to elements are unaffected).
    * `list`: All iterators and references are unaffected.
- Associative containers [multi] {set,map}: All iterators and references are unaffected.
- Container adaptors:
    * `stack`: Inherited from underlying container.
    * `queue`: Inherited from underlying container.
    * `priority_queue`: Inherited from underlying container.
- Erasure Sequence containers:
    * `vector`: Every iterator and reference after the point of erase is invalidated.
    * `deque`: All iterators and references are invalidated, unless the erased members are at an end (front or back) of the deque (in which case only iterators and references to the erased members are invalidated).
    * `list`: Only the iterators and references to the erased element is invalidated.
- Resizing:
    * `vector`: As per insert/erase.
    * `deque`: As per insert/erase.
    * `list`: As per insert/erase.

Please note that unless otherwise specified, invoking a container member function or passing a container as an argument to a library function shall not invalidate iterators to, or change the values of, objects within that container ¹.

I hope this helps! Let me know if you have any other questions.

Source: Conversation with Bing, 3/7/2024
(1) Iterator invalidation rules for C++ containers - Stack Overflow. https://stackoverflow.com/questions/6438086/iterator-invalidation-rules-for-c-containers.
(2) Safe Iterator Framework for the C++ Standard Template Library. https://pdfs.semanticscholar.org/771f/8cf8c3cc90c8cee2f27764e3aba51d761f33.pdf.
(3) Do the iterator invalidation rules mean thread safety?. https://stackoverflow.com/questions/32017363/do-the-iterator-invalidation-rules-mean-thread-safety.
(4) c++ - What is iterator invalidation? - Stack Overflow. https://stackoverflow.com/questions/16904454/what-is-iterator-invalidation.
(5) Iterator library - cppreference.com. https://en.cppreference.com/w/cpp/iterator.


Invalidation of iterators in the C++ Standard Library refers to situations where an iterator becomes unusable because the underlying container it references has been modified in a way that breaks the association between the iterator and the element it pointed to. Here are the common scenarios of iterator invalidation in the standard library containers:


**1. Modification Operations:**

- **Insertion:** Inserting elements before the iterator (using `insert` or similar methods) invalidates all iterators at or after the insertion point.
- **Removal:** Removing elements pointed to by the iterator or elements before it invalidates the iterator.
- **Swapping:** Swapping elements within the container might invalidate iterators pointing to the swapped elements, depending on the implementation.
- **`clear`:** Clearing the container invalidates all iterators.

**2. Resizing Operations:**

- **`resize`:** Resizing the container (changing its size) can invalidate all iterators, especially if the container needs to reallocate memory.

**3. Specific Container Behaviors:**

- **`std::vector`:** Iterators become invalid after any operation that modifies the vector's capacity (potentially reallocating memory), such as `push_back` when the vector is full, or `erase` followed by insertions that might cause reallocation.
- **`std::deque`:** Iterators might become invalid after insertions or removals near the beginning or end of the deque, as these operations might involve reallocation.
- **`std::list`:** Iterators remain valid after most operations, except for `splice` when it moves elements within the same list and the iterator points to a moved element.

**Important Notes:**

- **Past-the-end iterators:** These iterators (returned by `end()`) are generally not invalidated by modifications to the container, but their behavior might vary slightly depending on the specific container and operations performed.
- **Const member functions:** Iterators returned by const member functions (e.g., `cbegin()`, `cend()`) are typically guaranteed not to be invalidated by const member functions that don't modify the container.

**Recommendations:**

- **Re-obtain iterators:** If you're unsure about the validity of an iterator after an operation, it's generally safe to re-obtain the iterator from the container (e.g., using `begin()`) to ensure it points to a valid element.
- **Const iterators:** Use const iterators whenever possible when you only need to read elements without modifying the container. These iterators are less likely to be invalidated.
- **Understand container behavior:** Be familiar with the specific invalidation rules for the container you're using. Consult the C++ documentation for details.

By understanding these scenarios of iterator invalidation, you can write more robust and predictable C++ code that avoids unexpected behavior when iterating through containers. 



## Range-based for loops (range-for) and Iterator Invalidation

**Yes, range-based for loops are generally considered to be constant-time iterations.** This means the number of iterations doesn't directly affect the performance of the loop itself. However, it's important to be aware of iterator invalidation within the loop body, which can cause unexpected behavior.

Here's a breakdown of iterator invalidation for common containers and how to prevent it:

**1. `std::vector`:**

- **Scenarios:**
    - Inserting elements before the iterator (using `insert`) invalidates all iterators at or after the insertion point.
    - Removing elements pointed to by the iterator or elements before it invalidates the iterator.
    - Swapping elements within the vector might invalidate iterators pointing to the swapped elements if the swap involves reallocating memory (potential with full vector).
    - Resizing the vector (`resize`) can invalidate all iterators due to potential reallocation.
- **Prevention:**
    - Avoid modifying the vector's size or elements within the loop body if you rely on the iterator remaining valid.
    - If you need to modify the vector, consider using a different loop construct that allows you to control iterator invalidation (e.g., traditional for loop with index).

**2. `std::deque`:**

- **Scenarios:**
    - Inserting or removing elements near the beginning or end of the deque might invalidate iterators, especially for large insertions/removals, as they might trigger reallocation.
- **Prevention:**
    - Similar to `std::vector`, avoid modifying the deque's size or elements near the beginning or end within the loop if you rely on the iterator.
    - Consider using a different loop construct or techniques like iterators pointing to specific elements (e.g., by index) if needed.

**3. `std::list`:**

- **Scenarios:**
    - `splice` when it moves elements within the same list and the iterator points to a moved element.
- **Prevention:**
    - If using `splice` within the loop, be cautious about iterators pointing to elements that might be moved.
    - Consider re-obtaining iterators from the list after `splice` if necessary.

**4. `std::set` and `std::map`:**

- **Scenarios:**
    - Inserting or removing elements generally don't invalidate iterators unless the element being removed is the one pointed to by the iterator.
- **Prevention:**
    - Modifying elements within the loop (which isn't typical for sets/maps) might invalidate iterators in some cases. It's generally recommended to avoid modifying elements within the loop for these containers.

**Example of Swapping Leading to Invalidation:**

```c++
#include <iostream>
#include <vector>

int main() {
  std::vector<int> vec = {1, 2, 3, 4};
  std::vector<int>::iterator it = vec.begin(); // Iterator pointing to the first element (1)

  // Loop through the vector using range-for
  for (int& elem : vec) {
    elem *= 2; // Double each element
    if (elem == 4) { // Check if current element is 4
      std::swap(elem, vec[2]); // Swap element with element at index 2
    }
  }

  // Try to access the first element after the loop (assuming it was 2 before)
  std::cout << *it << std::endl; // This might print unexpected value or cause undefined behavior

  return 0;
}
```

In this example:

- The loop iterates and doubles each element.
- If an element is 4, it's swapped with the element at index 2.
- However, swapping elements within the vector might involve reallocation if the vector is full. This can invalidate the iterator `it` as it might no longer point to the originally intended element.

**Prevention:**

- Avoid modifying the vector's structure (size or element order) within the range-based for loop if you rely on the iterator's validity throughout the loop.
- If you need to swap elements, consider using a different approach like a traditional for loop with index-based access or a separate loop for modifications.

**General Tips:**

- Be cautious about modifying the container within a range-based for loop, especially if you rely on iterator validity.
- If unsure about potential invalidation, re-obtain iterators from the container or use a different loop construct (e.g., traditional for loop with index) that allows more control over iterators.
- Understand the specific invalidation rules for the container you're using
