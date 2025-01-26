#include <benchmark/benchmark.h>
#include <immintrin.h>
#include <list>
#include <random>

// Structure definition remains the same
struct Order {
    double price;
    int orderID;
    int quantity;
};

// Global list of orders
std::list<Order> orders;

// Function to populate the list of orders
void populateOrders(size_t numOrders) {
    std::mt19937 gen(42); // Fixed seed for reproducibility
    std::uniform_int_distribution<> orderDist(100, 10000);
    std::uniform_real_distribution<> priceDist(100.0, 200.0);
    std::uniform_int_distribution<> quantityDist(1, 100);

    for (size_t i = 0; i < numOrders; ++i) {
        int orderID = orderDist(gen);
        double price = priceDist(gen);
        int quantity = quantityDist(gen);

        orders.push_back({price, orderID, quantity});
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
