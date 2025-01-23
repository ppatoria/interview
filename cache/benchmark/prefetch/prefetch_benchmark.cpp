#include <benchmark/benchmark.h>
#include <immintrin.h>
#include <list>
#include <random>
#include <string>
#include <vector>

// Structure definition remains the same
struct Order {
    std::string symbol;
    double price;
    int quantity;
    bool isBuy; // true for buy orders, false for sell orders
};

// Global list of orders
std::list<Order> orders;

// Function to populate the list of orders
void populateOrders(size_t numOrders) {
    std::mt19937 gen(42); // Fixed seed for reproducibility
    std::uniform_real_distribution<> priceDist(100.0, 200.0);
    std::uniform_int_distribution<> quantityDist(1, 100);
    std::uniform_int_distribution<> symbolDist(0, 2);
    std::vector<std::string> symbols = {"AAPL", "GOOG", "MSFT"};

    for (size_t i = 0; i < numOrders; ++i) {
        std::string symbol = symbols[symbolDist(gen)];
        double price = priceDist(gen);
        int quantity = quantityDist(gen);
        bool isBuy = (i % 2 == 0);

        orders.push_back({symbol, price, quantity, isBuy});
    }
}

// Populate orders before running benchmarks
struct PopulateOrdersBeforeBenchmarks {
    PopulateOrdersBeforeBenchmarks() {
        populateOrders(1000000);
    }
};
static PopulateOrdersBeforeBenchmarks populateOrdersInstance;

// Function to process an order (dummy function for illustration)
void processOrder(const Order& order) {
    benchmark::DoNotOptimize(&order);
}

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
        _mm_prefetch(reinterpret_cast<const char*>(&(*it)), _MM_HINT_T0);
    }

    for (; it != orders.end(); ++it) {
        auto nextIt = std::next(it);
        if (nextIt != orders.end()) {
            _mm_prefetch(reinterpret_cast<const char*>(&(*nextIt)),
                         _MM_HINT_T0);
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

/***/

// Function to process orders with prefetching
void processOrdersWithPrefetchAll(std::list<Order>& orders) {

  for (const auto& order : orders) {
    _mm_prefetch(reinterpret_cast<const char *>(&order), _MM_HINT_T0);
    processOrder(order);
  }
}

// Benchmark function with prefetching
static void BM_ProcessOrdersWithPrefetchAll(benchmark::State& state) {
    for (auto _ : state) {
        processOrdersWithPrefetchAll(orders);
    }
}
BENCHMARK(BM_ProcessOrdersWithPrefetchAll);


BENCHMARK_MAIN();
