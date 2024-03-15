#include "algorithm"
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#pragma once

using namespace std;

/**
 * * Design:
 * ** Components and their relationship:
 * 1. For each **instrument** (like a stock or a future), there will be an `OrderBook`.
 * 2. Each `OrderBook` will contain an array of `PriceLevel`s for each side (buy/sell).
 * 3. Each `PriceLevel` will have an array of `Order`s of the same price.
 * 4. For each `Order`, there will be an `OrderMetaData` which is linked to the previous order and next order within the same `PriceLevel`.
 * 5. We have hash tables `OrderMetaDataByOrderIDMap` and `OrderBookByOrderIDMap`.
 */

namespace OrderBook {
enum class OrderType {
    GFD,
    IOC
};

enum class Side {
    BUY,
    SELL
};
template <typename T>
concept Container = requires(T t) {
    { t.begin() } -> std::same_as<typename T::iterator>;
    { t.end() } -> std::same_as<typename T::iterator>;
    requires !std::same_as<typename T::value_type, char>;
};

template <Container T>
std::ostream& operator<<(std::ostream& os, const T& container)
{
    os << "[";
    for_each(container.begin(), container.end(), [&os](const auto& element) {
        os << element << ", ";
    });

    if (!container.empty()) {
        os.seekp(-2, std::ios_base::end);
        os << "]";
    }
    return os;
}

struct Order {
    string id;
    double price;
    double qty;
    OrderType type;
    Side side;
    string sym;

    friend ostream& operator<<(std::ostream& os, const Order& order);
};

/**
 * The `firstOrder` and `lastOrder` pointers in `PriceLevel` struct is used
 * to quickly access the first and last orders at a particular price level. This
 * is useful in certain scenarios, such as:
 *
 * - Order matching: To match new incoming orders with existing orders. The
 *       `firstOrder` could represent the oldest (or "best") order at that price
 *       level, which would typically be the first one to get matched with an
 *       incoming order of the opposite side.
 *
 * - Order removal: When an order is cancelled or fully executed, it need to be
 *       removed from the order book. If the order to be removed is the
 *       `firstOrder` or `lastOrder`, having these pointers allows you to
 *       quickly update them to point to the next or previous order in the
 *       `orders` vector.
 *
 * - Market data dissemination: To send market data updates to traders or other
 *       systems. The `firstOrder` and `lastOrder` could be used to quickly
 *       provide information about the "best" and "worst" orders at a particular
 *       price level.
 */

struct PriceLevel {
    double price;
    shared_ptr<Order> firstOrder;
    shared_ptr<Order> lastOrder;
    vector<shared_ptr<Order>> orders;

    PriceLevel(double price);
    void addOrder(shared_ptr<Order>);

    friend std::ostream& operator<<(std::ostream& os, const PriceLevel& priceLevel)
    {
        os << "price: " << priceLevel.price
           << "firstOrder: ";
        if (priceLevel.firstOrder) {
            os << *(priceLevel.firstOrder); // Dereference the shared_ptr before printing
        }
        os << "lastOrder: ";
        if (priceLevel.lastOrder) {
            os << *(priceLevel.lastOrder); // Dereference the shared_ptr before printing
        }
        os << "Orders: ";
        for (const auto& order : priceLevel.orders) {
            if (order) {
                os << *order << ", "; // Dereference the shared_ptr before printing
            }
        }
        os << endl;
        return os;
    }
};

struct OrderBook {
    string symbol;
    vector<shared_ptr<PriceLevel>> bidLevels;
    vector<shared_ptr<PriceLevel>> askLevels;

    OrderBook(string sym);

    shared_ptr<PriceLevel> findInsertionPosition(const Order&) const;
    shared_ptr<PriceLevel> findOrCreatePriceLevel(const Order&);

    friend std::ostream& operator<<(std::ostream& os, const OrderBook& orderBook)
    {
        os << "Symbol: " << orderBook.symbol
           << "\nbidLevels: " << orderBook.bidLevels
           << "\naskLevels: " << orderBook.askLevels
           << endl;
        return os;
    }
};

/**
 * The `OrderMetaData` struct is a way to keep track of the relationship between
 * an `Order`, its `PriceLevel`, and its neighboring orders in the order book.

 * `nextOrder` and `prevOrder`:
 *     These are pointers to the next and previous orders in the same price level.
 *     They essentially form a doubly-linked list of orders within each price level.
 *     This can be useful for quickly navigating through orders at the same price level.
 */

struct OrderMetaData {
    shared_ptr<PriceLevel> priceLevel;
    shared_ptr<Order> nextOrder;
    shared_ptr<Order> prevOrder;

    OrderMetaData() = default;
    OrderMetaData(shared_ptr<PriceLevel>);

    friend std::ostream& operator<<(std::ostream& os, const OrderMetaData& orderMetaData)
    {
        os << "PriceLevel: " << orderMetaData.priceLevel
           << "nextOrder: " << orderMetaData.nextOrder
           << "prevOrder: " << orderMetaData.prevOrder
           << endl;
        return os;
    }
};

using OrderMetaDataByOrderIDMap = unordered_map<string, OrderMetaData>;
using OrderBookByOrderIDMap = unordered_map<string, OrderBook>;
using OrderBookBySymbolMap = unordered_map<string, OrderBook>;

} // OrderBook
