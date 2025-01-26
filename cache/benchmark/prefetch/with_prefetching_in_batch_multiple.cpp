#include "prefetch_benchmark.h"
#include <benchmark/benchmark.h>
#include <list>
#include <vector>
#include <algorithm>

struct OrdersSoA {
  std::vector<int> orderIDs;
  std::vector<double> prices;
  std::vector<int> quantities;
};

// Access and process in batches
void processOrdersSoA(OrdersSoA &orders) {
  const size_t batchSize = 4;
  size_t n = orders.orderIDs.size();

  for (size_t i = 0; i < n; i += batchSize) {
    size_t end = std::min(i + batchSize, n);

    // Prefetch the batch of IDs, prices, and quantities
    for (size_t j = i; j < end; ++j) {
      __builtin_prefetch(&orders.orderIDs[j]);
      __builtin_prefetch(&orders.prices[j]);
      __builtin_prefetch(&orders.quantities[j]);
    }

    // Process the batch
    for (size_t j = i; j < end; ++j) {
      processOrder(orders.orderIDs[j], orders.prices[j], orders.quantities[j]);
    }
  }
}

void processOrdersWithMultiBatchPrefetch(std::list<Order> &orders) {
  auto it = orders.begin();
  auto nextIt = orders.begin();

  const int batchSize = 4;
  alignas(64) Order* orderBatch[batchSize];

  while (it != orders.end()) {
    size_t currentBatchSize = 0;

    // Prefetch the next batch in advance
    for (int i = 0; i < batchSize && nextIt != orders.end(); ++i, ++nextIt) {
      __builtin_prefetch(&(*nextIt)); // Prefetch ahead
    }

    // Prefetch the next 4 orders and collect them in a batch
    for (int i = 0; i < batchSize && it != orders.end(); ++i, ++it) {
      __builtin_prefetch(&(*it));
      orderBatch[i] = &(*it);
        }

    processOrderBatch(orderBatch, currentBatchSize); // Process the batch
  }
}
// Benchmark function with prefetching
static void BM_ProcessOrdersWithPrefetchingInMultipleBatch(benchmark::State& state) {
    for (auto _ : state) {
        processOrdersWithMultiBatchPrefetch(orders);
    }
}
BENCHMARK(BM_ProcessOrdersWithPrefetchingInMultipleBatch);

BENCHMARK_MAIN();
