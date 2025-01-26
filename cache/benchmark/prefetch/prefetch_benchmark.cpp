#include "prefetch_benchmark.h"
#include <list>

// Function to process orders without prefetching
void processOrdersWithoutPrefetching(std::list<Order>& orders) {
    for (const auto& order : orders) {
        processOrder(order);
    }
}

// Benchmark function without prefetching
static void BM_ProcessOrdersWithoutPrefetching(benchmark::State& state) {
    for (auto _ : state) {
        processOrdersWithoutPrefetching(orders);
    }
}
BENCHMARK(BM_ProcessOrdersWithoutPrefetching);

// Function to process orders with prefetching
void processOrdersWithPrefetching(std::list<Order>& orders) {
    auto it = orders.begin();
    // Prefetch the first element
    if (it != orders.end()) {
        // _mm_prefetch(reinterpret_cast<const char*>(&(*it)), _MM_HINT_T0);
        __builtin_prefetch(&(*it));
    }

    for (; it != orders.end(); ++it) {
        auto nextIt = std::next(it);
        if (nextIt != orders.end()) {
            // _mm_prefetch(reinterpret_cast<const char*>(&(*nextIt)), _MM_HINT_T0);
            __builtin_prefetch(&(*nextIt));
        }
        processOrder(*it);
    }
}

// Benchmark function with prefetching
static void BM_ProcessOrdersWithPrefetching(benchmark::State& state) {
    for (auto _ : state) {
        processOrdersWithPrefetching(orders);
    }
}
BENCHMARK(BM_ProcessOrdersWithPrefetching);


/**/
// Function to process a batch of orders (dummy function for illustration)
void processOrderBatch(const Order orderBatch[4], size_t batchSize) {
    for (size_t i = 0; i < batchSize; ++i) {
        benchmark::DoNotOptimize(&orderBatch[i]);
    }
}

// Function to process orders with prefetching 4 orders at a time
void processOrdersWithPrefetchBatch(std::list<Order>& orders) {
    auto it = orders.begin();
    Order orderBatch[4];

    while (it != orders.end()) {
        size_t batchSize = 0;

        // Prefetch the next 4 orders and collect them in a batch
        for (int i = 0; i < 4 && it != orders.end(); ++i, ++it) {
          // _mm_prefetch(reinterpret_cast<const char*>(&(*it)), _MM_HINT_T0);
          __builtin_prefetch(&(*it));
          orderBatch[batchSize++] = *it;
        }

        // Process the batch of prefetched orders
        processOrderBatch(orderBatch, batchSize);
    }
}

// Benchmark function with prefetching 4 orders at a time
static void BM_ProcessOrdersWithPrefetchBatch(benchmark::State& state) {
    for (auto _ : state) {
        processOrdersWithPrefetchBatch(orders);
    }
}
BENCHMARK(BM_ProcessOrdersWithPrefetchBatch);

BENCHMARK_MAIN();
