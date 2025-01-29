#include "contigous_allocator.h"
#include <benchmark/benchmark.h>
#include <immintrin.h>
#include <list>
#include <random>
#include <vector>

// Structure definition remains the same
struct Order {
    double price;
    int orderID;
    int quantity;
};

// Cache line size (example: 64 bytes)
constexpr size_t CACHE_LINE_SIZE = 64;

// Custom allocator for contiguous storage
template<typename T>
class ContiguousAllocator {
public:
    using value_type = T;

    ContiguousAllocator() {
        data.resize(CACHE_LINE_SIZE / sizeof(T) * 100); // Initial size
        currentIndex = 0;
    }

    template <typename U>
    ContiguousAllocator(const ContiguousAllocator<U>&) noexcept {}

    T* allocate(std::size_t n) {
        // Ensure enough space by resizing the vector if needed
        if (currentIndex + n > data.size()) {
            data.resize(data.size() + n);
        }
        T* ptr = &data[currentIndex];
        currentIndex += n;
        return ptr;
    }

    void deallocate(T*, std::size_t) noexcept {
        // No deallocation needed as we are using a contiguous block
    }

private:
    std::vector<T> data;
    std::size_t currentIndex;
};

template <typename T, typename U>
bool operator==(const ContiguousAllocator<T>&, const ContiguousAllocator<U>&) noexcept {
    return true;
}

template <typename T, typename U>
bool operator!=(const ContiguousAllocator<T>&, const ContiguousAllocator<U>&) noexcept {
    return false;
}

// Global list of orders with custom allocator
std::list<Order, ContiguousAllocator<Order>> ordersWithCustomAllocator;

// Function to populate the list of orders
void populateOrders(std::list<Order, ContiguousAllocator<Order>>& orders, size_t numOrders) {
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





// Function to process an order (dummy function for illustration)
void processOrder(const Order& order) {
    benchmark::DoNotOptimize(&order);
}

// Benchmark for custom allocator
static void BM_CustomAllocator(benchmark::State& state) {
    for (auto _ : state) {
        for (const auto& order : ordersWithCustomAllocator) {
          processOrder(order);
        }
    }
}
BENCHMARK(BM_CustomAllocator);

int main(int argc, char** argv) {
    // Populate orders
    size_t numOrders = 1000000;
    populateOrders(ordersWithCustomAllocator, numOrders);

    // Run benchmarks
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
    return 0;
}
