#include <algorithm>
#include <map>
#include <stdexcept>
#include <string>
#include <unordered_map>

enum class Side { Buy, Sell };
struct Order {
  int orderID;
  std::string symbol;
  int quantity;
  double price;
  Side side;
};

class Orderbook {
private:
  using PriceLevel = std::multimap<double, Order>;
  PriceLevel bids;
  PriceLevel asks;

  using Cache = std::unordered_map<int, PriceLevel::iterator>;
  Cache cache;

public:
  void insert(const Order &order) {
    auto iter = cache.find(order.orderID);
    if (iter != cache.end())
      throw std::runtime_error("Order is already present.");

    auto& priceLevel = (order.side == Side::Buy) ? bids : asks;

    auto priceLevelIter = priceLevel.insert({order.price, order});
    cache[order.orderID] = priceLevelIter;
  }

  void cancel(const Order &order) {
    auto iter = cache.find(order.orderID);
    if (iter == cache.end())
      throw std::runtime_error("Order not present in the order book.");

    auto& priceLevel = (order.side == Side::Buy) ? bids : asks;

    priceLevel.erase(iter->second);
    cache.erase(order.orderID);
  }

  void modify(const Order &order) {
    auto iter = cache.find(order.orderID);
    if (iter == cache.end())
      throw std::runtime_error("Order not present in the order book.");

    auto& priceLevel = (order.side == Side::Buy) ? bids : asks;

    auto &existingOrder = iter->second->second;

    if (existingOrder.orderID != order.orderID ||
        existingOrder.symbol != order.symbol ||
        existingOrder.side != order.side) {
      throw std::runtime_error(
          "Invalid Order: Incoming Order does not match the existing order.");
    }

    if (existingOrder.price != order.price) {
      cancel(order);
      insert(order);
    } else if (existingOrder.quantity != order.quantity) {
      existingOrder.quantity = order.quantity;
    }
  }

  void match() {
    while (!bids.empty() && !asks.empty()) {
      auto bestBid = bids.rbegin();
      auto bestAsk = asks.begin();

      auto &[bestBidPrice, bestBidOrder] = *bestBid;
      auto &[bestAskPrice, bestAskOrder] = *bestAsk;

      if (bestBidPrice < bestAskPrice)
        break;

      auto matchedQuantity =
          std::min(bestBidOrder.quantity, bestAskOrder.quantity);
      bestBidOrder.quantity -= matchedQuantity;
      bestAskOrder.quantity -= matchedQuantity;

      if (bestBidOrder.quantity == 0)
        bids.erase(std::prev(bestBid.base()));

      if (bestAskOrder.quantity == 0)
        asks.erase(bestAsk);
    }
  }

  void match(Order &order) {
    if (order.side == Side::Buy) {
      while (!asks.empty() && order.quantity != 0) {
        auto bestAsk = asks.begin();
        auto &[bestAskPrice, bestAskOrder] = *bestAsk;
        if (order.price < bestAskPrice)
          return;
        auto matchedQuantity = std::min(order.quantity, bestAskOrder.quantity);
        order.quantity -= matchedQuantity;
        bestAskOrder.quantity -= matchedQuantity;
        if (bestAskOrder.quantity == 0)
          asks.erase(bestAsk);
      }
    } else if (order.side == Side::Sell) {
      while (!bids.empty() && order.quantity != 0) {
        auto bestBid = bids.rbegin();
        auto &[bestBidPrice, bestBidOrder] = *bestBid;
        if (bestBidPrice < order.price)
          return;
        auto matchedQuantity = std::min(order.quantity, bestBidOrder.quantity);
        order.quantity -= matchedQuantity;
        bestBidOrder.quantity -= matchedQuantity;
        if (bestBidOrder.quantity == 0)
          bids.erase(std::prev(bestBid.base()));
      }
    }
  }
};

#include <iostream>

int main() {
  Orderbook ob;

  // Insert Orders
  Order o1 = {1, "AAPL", 10, 150.0, Side::Buy};
  Order o2 = {2, "AAPL", 15, 152.0, Side::Buy};
  Order o3 = {3, "AAPL", 20, 149.0, Side::Sell};
  Order o4 = {4, "AAPL", 5, 151.0, Side::Sell};

  ob.insert(o1);
  ob.insert(o2);
  ob.insert(o3);
  ob.insert(o4);
  std::cout << "Inserted orders successfully.\n";

  // Modify Order: Change quantity of order 1
  Order mod1 = {1, "AAPL", 12, 150.0, Side::Buy};
  ob.modify(mod1);
  std::cout << "Modified order 1 successfully.\n";

  // Modify Order: Change price (must be cancel + insert)
  Order mod2 = {2, "AAPL", 15, 153.0, Side::Buy};
  ob.modify(mod2);
  std::cout << "Modified order 2 (price changed).\n";

  // Cancel Order
  ob.cancel(o3);
  std::cout << "Canceled order 3 successfully.\n";

  // Match Orders (Auto trade execution)
  ob.match();
  std::cout << "Matching completed.\n";

  // Place a new aggressive order to trigger partial fills
  Order aggressiveBuy = {5, "AAPL", 18, 151.0, Side::Buy};
  ob.match(aggressiveBuy);
  std::cout << "Aggressive order matched partially/fully.\n";

  return 0;
}
