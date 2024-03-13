#include <chrono>
#include <memory>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#pragma once

using namespace std;

namespace OrderBook {
enum class OrderType {
    GFD,
    IOC
};

enum class Side {
    BUY,
    SELL
};

struct Order {
    string id;
    double price;
    double qty;
    OrderType type;
    Side side;
    string sym;

    friend ostream& operator<<(std::ostream& os, const Order& order);
};

struct PriceLevel {
    double price;
    shared_ptr<Order> firstOrder;
    shared_ptr<Order> lastOrder;
    vector<shared_ptr<Order>> orders;

    inline PriceLevel(double price);
    void update(shared_ptr<Order> order);
};

struct OrderBook {
    vector<shared_ptr<PriceLevel>> bidLevels;
    vector<shared_ptr<PriceLevel>> askLevels;

    optional<double> bestBid;
    optional<double> bestAsk;

    void updateBestPrice(const Order& order);

    shared_ptr<PriceLevel> findInsertionPosition(const Order& order) const;
    std::shared_ptr<PriceLevel> findOrCreatePriceLevel(const Order&);
};

struct OrderMetaData {
    shared_ptr<PriceLevel> priceLevel;
    shared_ptr<Order> nextOrder;
    shared_ptr<Order> prevOrder;

    OrderMetaData() = default;

    OrderMetaData(shared_ptr<PriceLevel> priceLevel, shared_ptr<Order> order);
};

unordered_map<string, OrderMetaData> OrderMetaDataByOrderIDMap; // Export the variables
unordered_map<string, OrderBook> OrderBookByOrderIDMap;

/** PriceLevel */
shared_ptr<PriceLevel> findInsertionPosition(const Order& order, OrderBook& orderBook);
shared_ptr<PriceLevel> getPriceLevel(const Order& order, OrderBook& orderBook);

/** OrderBook operations*/
void insert(shared_ptr<Order> order, OrderBook& orderBook);

} // OrderBook
