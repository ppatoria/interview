#include "prefetch_benchmark.h"
#include <benchmark/benchmark.h>
#include <list>

// Function to process a batch of orders (dummy function for illustration)
void processOrderBatch(const Order* orderBatch[4], size_t batchSize) {
    for (size_t i = 0; i < batchSize; ++i) {
        benchmark::DoNotOptimize(&orderBatch[i]);
    }
}

// Function to process orders with prefetching 4 orders at a time
void processOrdersWithPrefetchBatch(std::list<Order>& orders) {
    auto it = orders.begin();
    const Order* orderBatch[4];

    while (it != orders.end()) {
        size_t batchSize = 0;

        // Prefetch the next 4 orders and collect them in a batch
        for (int i = 0; i < 4 && it != orders.end(); ++i, ++it) {
          // _mm_prefetch(reinterpret_cast<const char*>(&(*it)), _MM_HINT_T0);
          __builtin_prefetch(&(*it));
          orderBatch[batchSize++] = &(*it);
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
