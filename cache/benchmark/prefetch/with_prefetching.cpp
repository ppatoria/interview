#include "prefetch_benchmark.h"
#include <list>

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
BENCHMARK_MAIN();
