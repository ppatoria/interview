#include <iostream>
#include <unordered_map>
#include <list>
#include <utility>

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

