# Cache Optimization through Prefetching: Enhancing Non-Contiguous Data Processing

In my previous article, I discussed cache optimization for contiguous memory. However, real-world data isn’t always contiguous. This post focuses on optimizing non-contiguous data using prefetching techniques.

---

## What is Prefetching?
Prefetching instructs the CPU to load data into the cache before it’s needed, reducing wait times and avoiding cache misses in non-contiguous data.

---

## Example: Order Processing with Linked List

### Initial Structure:
```cpp
struct Order {
    double price;
    int orderID;
    int quantity;
};
```

### Without Prefetching:
```cpp
void processOrdersWithoutPrefetching(std::list<Order>& orders) {
    for (const auto& order : orders) {
        processOrder(order);
    }
}
```
This can result in cache misses as linked lists store elements non-contiguously.

---

## Introducing Prefetching
Modern CPUs support non-blocking prefetching, which reduces latency and allows execution of other tasks during data fetch.

### With Prefetching:
```cpp
void processOrdersWithPrefetching(std::list<Order>& orders) {
    auto it = orders.begin();
    if (it != orders.end()) __builtin_prefetch(&(*it));

    for (; it != orders.end(); ++it) {
        auto nextIt = std::next(it);
        if (nextIt != orders.end()) __builtin_prefetch(&(*nextIt));
        processOrder(*it);
    }
}
```

---

## Key Points About Prefetching
- **Non-Blocking Nature:** Prefetching allows the CPU to continue processing while fetching.
- **Optimal Distance:** Avoid prefetching too close or far ahead to minimize wasted resources.

---

## Improved Prefetching with Batch Processing
Batch processing reduces cache misses by fetching and processing multiple items together.

### Batch Processing Example:
```cpp
void processOrdersInBatches(std::list<Order>& orders) {
    auto it = orders.begin();
    Order* orderBatch[4];

    while (it != orders.end()) {
        size_t batchSize = 0;

        for (int i = 0; i < 4 && it != orders.end(); ++i, ++it) {
            __builtin_prefetch(&(*it));
            orderBatch[batchSize++] = &(*it);
        }

        processOrderBatch(orderBatch, batchSize);
    }
}
```

### Why Batch Size 4?
Each cache line typically holds 4 `Order` structs, ensuring efficient cache utilization.

---

## Conclusion
Prefetching, combined with batch processing, optimizes cache usage for non-contiguous data structures like linked lists. These techniques reduce cache misses and improve performance.
