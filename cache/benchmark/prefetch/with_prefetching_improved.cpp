#include <benchmark/benchmark.h>
#include "prefetch_benchmark.h"
#include <list>



// Improved function to process orders with prefetching
void processOrdersWithPrefetching(std::list<Order>& orders) {
    const int PREFETCH_DISTANCE = 2; // Prefetch 2 elements ahead for better timing

    auto it = orders.begin();

    // Prefetch the first few elements up to PREFETCH_DISTANCE
    for (int i = 0; i < PREFETCH_DISTANCE && it != orders.end(); ++i, ++it) {
        __builtin_prefetch(&(*it), 0, 3);
    }

    // Iterate through the list and process orders with prefetching
    for (it = orders.begin(); it != orders.end(); ++it) {
        auto prefetchIt = it;
        for (int i = 0; i < PREFETCH_DISTANCE; ++i) {
            ++prefetchIt;
            if (prefetchIt != orders.end()) {
                __builtin_prefetch(&(*prefetchIt), 0, 3);
            } else {
                break;
            }
        }
        processOrder(*it);
    }
}

// Benchmark function with prefetching
static void BM_ProcessOrdersWithPrefetching2Elements(benchmark::State& state) {
    for (auto _ : state) {
        processOrdersWithPrefetching(orders);
    }
}
BENCHMARK(BM_ProcessOrdersWithPrefetching2Elements);


BENCHMARK_MAIN();
