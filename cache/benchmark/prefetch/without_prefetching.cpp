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
BENCHMARK_MAIN();
