#include <iostream>
#include <unordered_map>
#include <deque>
#include <utility>

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
