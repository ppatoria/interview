#include "prefetch_benchmark.h"
#include <benchmark/benchmark.h>
#include <cstring> // For memcpy
#include <list>

// Function to process a batch of orders (dummy function for illustration)
void processOrderBatch(Order orderBatch[4], size_t batchSize) {
    for (size_t i = 0; i < batchSize; ++i) {
        benchmark::DoNotOptimize(&orderBatch[i]);
    }
}

void processOrdersWithSIMDBatch(std::list<Order> &orders) {
  auto it = orders.begin();
  alignas(64) Order orderBatch[4]; // Aligned batch for SIMD-friendly access

  while (it != orders.end()) {
    size_t batchSize = 0;

    // Prefetch and copy 4 orders into the batch
    for (int i = 0; i < 4 && it != orders.end(); ++i, ++it) {
      __builtin_prefetch(&(*it)); // Prefetch the actual object
      std::memcpy(&orderBatch[batchSize++], &(*it),
                  sizeof(Order)); // SIMD-optimized memcpy
    }

    // Process the batch of copied orders
    processOrderBatch(orderBatch, batchSize);
  }
}
// Benchmark function with prefetching 4 orders at a time
static void BM_ProcessOrdersWithPrefetchBatchUsingmemcpy(benchmark::State& state) {
    for (auto _ : state) {
      processOrdersWithSIMDBatch(orders);
    }
}
BENCHMARK(BM_ProcessOrdersWithPrefetchBatchUsingmemcpy);
BENCHMARK_MAIN();
