#include <algorithm>
#include <deque>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

// Order structure
struct Order {
  int orderId;
  std::string type; // "buy" or "sell"
  int quantity;
  double price;
  long timestamp;
  std::string instrument; // Instrument name

  Order(int id, const std::string &t, int q, double p, long ts,
        const std::string &instr)
      : orderId(id), type(t), quantity(q), price(p), timestamp(ts),
        instrument(instr) {}
};

// LRU Cache to track most recently used orders
class LRUCache {
private:
  int capacity;
  // Maps orderId to deque iterator
  std::unordered_map<int, std::deque<Order>::iterator> orderMap;
  std::deque<Order> orderDeque; // Holds orders in LRU order

public:
  LRUCache(int cap) : capacity(cap) {}

  // Add or update an order in the cache
  void accessOrder(const Order &order) {
    if (orderMap.find(order.orderId) != orderMap.end()) {
      // Move accessed order to the front
      orderDeque.erase(orderMap[order.orderId]);
    } else if (orderDeque.size() == capacity) {
      // Remove the least recently used order
      orderMap.erase(orderDeque.back().orderId);
      orderDeque.pop_back();
    }
    orderDeque.push_front(order);
    orderMap[order.orderId] = orderDeque.begin();
  }

  // Remove an order from the cache
  void removeOrder(int orderId) {
    if (orderMap.find(orderId) != orderMap.end()) {
      orderDeque.erase(orderMap[orderId]);
      orderMap.erase(orderId);
    }
  }

  // Check if an order is in the cache
  bool containsOrder(int orderId) const {
    return orderMap.find(orderId) != orderMap.end();
  }

  // Retrieve an order (if available)
  const Order *getOrder(int orderId) const {
    if (orderMap.find(orderId) != orderMap.end()) {
      return &(*orderMap.at(orderId));
    }
    return nullptr;
  }
};

// OrderBook structure
class OrderBook {
private:
  std::map<double, std::vector<Order>, std::greater<double>> buyOrders;
  std::map<double, std::vector<Order>, std::less<double>> sellOrders;

public:
  void addOrder(const Order &order) {
    if (order.type == "buy") {
      buyOrders[order.price].push_back(order);
    } else if (order.type == "sell") {
      sellOrders[order.price].push_back(order);
    }
  }

  void matchOrders() {
    while (!buyOrders.empty() && !sellOrders.empty()) {
      const auto &highestBuy = buyOrders.begin();
      const auto &lowestSell = sellOrders.begin();

      if (highestBuy->first >= lowestSell->first) {
        auto &buyOrdersAtPrice = highestBuy->second;
        auto &sellOrdersAtPrice = lowestSell->second;

        while (!buyOrdersAtPrice.empty() && !sellOrdersAtPrice.empty()) {
          Order &buyOrder = buyOrdersAtPrice.back();
          Order &sellOrder = sellOrdersAtPrice.back();

          int matchedQty = std::min(buyOrder.quantity, sellOrder.quantity);

          std::cout << "Matched Buy Order " << buyOrder.orderId
                    << " with Sell Order " << sellOrder.orderId
                    << ", Quantity: " << matchedQty << "\n";

          buyOrder.quantity -= matchedQty;
          sellOrder.quantity -= matchedQty;

          if (buyOrder.quantity == 0)
            buyOrdersAtPrice.pop_back();
          if (sellOrder.quantity == 0)
            sellOrdersAtPrice.pop_back();
        }

        if (buyOrdersAtPrice.empty())
          buyOrders.erase(highestBuy);
        if (sellOrdersAtPrice.empty())
          sellOrders.erase(lowestSell);
      } else {
        break;
      }
    }
  }

  void printOrders() const {
    std::cout << "Buy Orders:\n";
    for (const auto &[price, orders] : buyOrders) {
      std::cout << "  Price: " << price << "\n";
      for (const auto &order : orders) {
        std::cout << "    OrderID: " << order.orderId
                  << ", Quantity: " << order.quantity << "\n";
      }
    }

    std::cout << "Sell Orders:\n";
    for (const auto &[price, orders] : sellOrders) {
      std::cout << "  Price: " << price << "\n";
      for (const auto &order : orders) {
        std::cout << "    OrderID: " << order.orderId
                  << ", Quantity: " << order.quantity << "\n";
      }
    }
  }
};

// MultiInstrumentOrderBook structure
class MultiInstrumentOrderBook {
private:
  std::unordered_map<std::string, OrderBook> books;
  LRUCache lruCache;

public:
  MultiInstrumentOrderBook(int lruCapacity) : lruCache(lruCapacity) {}

  void addOrder(const Order &order) {
    books[order.instrument].addOrder(order);
    lruCache.accessOrder(order);
    books[order.instrument].matchOrders();
  }

  void cancelOrder(int orderId) {
    if (lruCache.containsOrder(orderId)) {
      const Order *cachedOrder = lruCache.getOrder(orderId);
      if (cachedOrder) {
        std::cout << "Canceling OrderID: " << cachedOrder->orderId << "\n";
        lruCache.removeOrder(orderId);
      }
    } else {
      std::cout << "OrderID " << orderId << " not found in cache.\n";
    }
  }

  void printOrderBooks() const {
    for (const auto &[instrument, book] : books) {
      std::cout << "Instrument: " << instrument << "\n";
      book.printOrders();
    }
  }
};

// Main function
int main() {
  std::cout << "starting" << std::endl;

  MultiInstrumentOrderBook multiBook(5); // LRU Cache with capacity 5

  multiBook.addOrder(Order(1, "buy", 100, 50.5, 1000, "AAPL"));
  multiBook.addOrder(Order(2, "sell", 50, 49.5, 1001, "AAPL"));
  multiBook.addOrder(Order(3, "buy", 150, 200.0, 1002, "GOOG"));
  multiBook.addOrder(Order(4, "sell", 75, 199.5, 1003, "GOOG"));
  multiBook.addOrder(Order(5, "buy", 200, 300.0, 1004, "MSFT"));

  multiBook.printOrderBooks();

  multiBook.cancelOrder(3);
  multiBook.cancelOrder(6);

  return 0;
}
