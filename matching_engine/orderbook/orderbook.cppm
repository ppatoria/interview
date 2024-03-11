#include <chrono>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

/**
 * Order         : This struct represents an individual order. It contains the order ID, price, quantity, type, and side (buy or sell).
 * PriceLevel    : This struct represents a specific price level in the order book. It contains the price, pointers to the first and last orders at this price level, and a vector of pointers to all orders at this price level.
 * OrderBook     : This struct represents the entire order book for a specific instrument. It contains vectors of pointers to all price levels on the bid side (buy orders) and the ask sides (sell orders).
 * OrderMetaData : This struct contains metadata about an order, including pointers to the price level that the order belongs to, and the next and previous orders at the same price level.
 * OrderMap      : This is an unordered map (hash map) that allows you to quickly look up the metadata for any order using its ID.
 * OrderBookMap  : This is an unordered map (hash map) that allows you to quickly look up the OrderBook for any instrument using symbol.
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

struct Order {
    string id;
    double price;
    double qty;
    OrderType type;
    Side side;
    string sym;

    friend std::ostream& operator<<(std::ostream& os, const Order& order)
    {
        os << order.id << "\t"
           << order.price << "\t"
           << order.qty << "\t"
           << (order.type == OrderType::GFD ? "GFD" : "IOC") << "\t"
           << (order.side == Side::BUY ? "BUY" : "SELL") << "\t"
           << order.sym << "\n";
        return os;
    }
};

struct PriceLevel {
    double price;
    shared_ptr<Order> firstOrder;
    shared_ptr<Order> lastOrder;
    vector<shared_ptr<Order>> orders;

    PriceLevel(double price)
        : price(price)
        , firstOrder(nullptr)
        , lastOrder(nullptr)
    {
    }
};

struct OrderBook { // per instrument
    vector<shared_ptr<PriceLevel>> bidLevels;
    vector<shared_ptr<PriceLevel>> askLevels;

    optional<double> bestBid;
    optional<double> bestAsk;

    void updateBestPrice(const Order& order)
    {
        if (order.side == Side::BUY) {
            bestBid = order.price;
        } else {
            bestAsk = order.price;
        }
    }
};

struct OrderMetaData {
    shared_ptr<PriceLevel> priceLevel;
    shared_ptr<Order> nextOrder;
    shared_ptr<Order> prevOrder;

    OrderMetaData() = default;

    OrderMetaData(
        std::shared_ptr<PriceLevel> priceLevel,
        std::shared_ptr<Order> nextOrder = nullptr,
        std::shared_ptr<Order> prevOrder = nullptr)
        : priceLevel(priceLevel)
        , nextOrder(nextOrder)
        , prevOrder(prevOrder)
    {
    }
};

std::unordered_map<std::string, OrderMetaData> OrderMap;
std::unordered_map<std::string, OrderBook> OrderBookMap;
} // OrderBook namespace
