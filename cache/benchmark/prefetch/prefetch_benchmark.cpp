#include <benchmark/benchmark.h>
#include <immintrin.h>
#include <list>
#include <map>
#include <unordered_map>
#include <string>
#include <random>
#include <vector>

// Structure definition remains the same
struct Order {
    std::string symbol;
    double price;
    int quantity;
    bool isBuy; // true for buy orders, false for sell orders
};

// Alias for the order book
using OrderBook = std::map<double, std::list<Order>>;

// Global order books and incoming order
std::unordered_map<std::string, OrderBook> orderBooks;
Order incomingOrder = {"AAPL", 150.5, 50, false};

class PopulateOrderBooks {
  PopulateOrderBooks() {
    size_t numOrders = 1000;
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

      orderBooks[symbol][price].push_back({symbol, price, quantity, isBuy});
    }
  }
};

// Populate the order books with 1,000,000 orders
PopulateOrderBooks simulatedData();

// Function to process an order (dummy function for illustration)
void processOrder(const Order& order) {
    benchmark::DoNotOptimize(&order);
}

// Function to match orders without prefetching
void matchOrdersWithoutPrefetching(
    std::unordered_map<std::string, OrderBook>& orderBooks,
    const Order& incomingOrder) {
    auto bookIt = orderBooks.find(incomingOrder.symbol);
    if (bookIt != orderBooks.end()) {
        OrderBook& book = bookIt->second;
        for (auto it = book.begin(); it != book.end(); ++it) {
            for (auto& order : it->second) {
                processOrder(order);
            }
        }
    }
}

// Benchmark function without prefetching
static void BM_MatchOrdersWithoutPrefetching(benchmark::State& state) {
    for (auto _ : state) {
        matchOrdersWithoutPrefetching(orderBooks, incomingOrder);
    }
}
BENCHMARK(BM_MatchOrdersWithoutPrefetching);

void matchOrdersWithPrefetching(
    std::unordered_map<std::string, OrderBook> &orderBooks,
    const Order &incomingOrder) {
  auto bookIt = orderBooks.find(incomingOrder.symbol);
  if (bookIt != orderBooks.end()) {
    OrderBook &book = bookIt->second;

    auto it = book.begin();
    // Prefetch the first element
    if (it != book.end()) {
      _mm_prefetch(reinterpret_cast<const char *>(&(*it)), _MM_HINT_T0);
    }

    for (; it != book.end(); ++it) {
      auto nextIt = std::next(it);
      if (nextIt != book.end()) {
        _mm_prefetch(reinterpret_cast<const char *>(&(*nextIt)), _MM_HINT_T0);
      }
      for (auto &order : it->second) {
        processOrder(order);
      }
    }
  }
}

// Benchmark function with prefetching
static void BM_MatchOrdersWithPrefetching(benchmark::State& state) {
    for (auto _ : state) {
        matchOrdersWithPrefetching(orderBooks, incomingOrder);
    }
}
BENCHMARK(BM_MatchOrdersWithPrefetching);

BENCHMARK_MAIN();
