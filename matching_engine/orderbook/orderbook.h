#include "algorithm"
#include <deque>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#pragma once

using namespace std;

/**
 * Design: Components and their relationship:
 ********************************************
 * 1. For each **instrument** (like a stock or a future), there will be an `OrderBook`.
 * 2. Each `OrderBook` will contain an array of `PriceLevel`s for each side (buy/sell).
 * 3. Each `PriceLevel` will have an queue(deque)  of `Order`s of the same price.
 * 4. We have hash tables `PriceLevelByOrderIDMap` and `OrderBookBySymbolMap`.
 */

namespace OrderBook {
class OrderBook;
struct PriceLevel;

using OrderID = string;
using Symbol = string;
using Price = double;
using Quantity = int;
using PriceLevelByOrderIDMap = unordered_map<OrderID, shared_ptr<PriceLevel>>;
using OrderBookBySymbolMap = unordered_map<Symbol, OrderBook>;

enum class OrderType {
    GFD,
    IOC
};

enum class Side {
    BUY,
    SELL
};

struct Order {

    OrderID id;
    Price price;
    Quantity qty;
    OrderType type;
    Side side;
    Symbol sym;

    Order() = default;
    Order(OrderID, Price, Quantity, OrderType, Side, Symbol);

    Order(const Order&) = delete;
    Order& operator=(const Order&) = delete;

    Order(Order&& rhs) noexcept;
    Order& operator=(Order&& rhs) noexcept;

    unique_ptr<Order> clone() const;
    string toString() const;

private:
    void moveResourceFrom(Order&& order);
    void setToDefaultState();
};

struct PriceLevel {

    double price;
    deque<Order> orders; // FIFO queue for order execution

    PriceLevel() = default;
    PriceLevel(Price);

    PriceLevel(const PriceLevel&) = delete;
    PriceLevel& operator=(const PriceLevel&) = delete;

    PriceLevel(PriceLevel&&) noexcept;
    PriceLevel& operator=(PriceLevel&&) noexcept;

    string toString() const;

private:
    void moveResourceFrom(PriceLevel&& order);
    void setToDefaultState();
};

struct OrderBook {
    string symbol;
    vector<PriceLevel> bidLevels;
    vector<PriceLevel> askLevels;

    PriceLevelByOrderIDMap priceLevelByOrderIDMap;

private:
    optional<reference_wrapper<PriceLevel>> findInsertionPosition(const Order&);

    PriceLevel& findOrCreatePriceLevel(const Order&);

public:
    OrderBook(Symbol);
    const Order& newOrder(Order&&);
    const Order& cancelOrder(const Order&);
    const Order& modifyOrder(const Order&);

    string toString() const;
};

class OrderBooks {
    OrderBookBySymbolMap orderBookBySymbolMap;

public:
    OrderBooks() = default;
    OrderBooks(const OrderBooks&) = delete;
    OrderBooks& operator=(const OrderBooks&) = delete;

    bool contains(const Symbol&) const;
    optional<reference_wrapper<const OrderBook>> get(const Symbol&) const;
    const OrderBook& getOrAdd(const Symbol&);
    const OrderBook& getOrAdd(OrderBook&&);
    const OrderBook remove(const Symbol&);
};

class OrderBookHandler {
public:
    const Order& newOrder(Order&&);
    const Order& cancelOrder(const Order&);
    const Order& modifyOrder(const Order&);

private:
    OrderBooks orderBooks;
};

} // OrderBook
