#include <chrono>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

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

    friend std::ostream& operator<<(std::ostream& os, const Order& order)
    {
        os << order.id << "\t"
           << order.price << "\t"
           << order.qty << "\t"
           << (order.type == OrderType::GFD ? "GFD" : "IOC") << "\t"
           << (order.side == Side::BUY ? "BUY" : "SELL") << "\n";
        return os;
    }
};

struct PriceLevel {
    double price;
    std::shared_ptr<Order> firstOrder;
    std::shared_ptr<Order> lastOrder;
    std::vector<std::shared_ptr<Order>> orders;

    PriceLevel(double price)
        : price(price)
        , firstOrder(nullptr)
        , lastOrder(nullptr)
    {
    }
};

struct OrderBook { // per instrument
    std::vector<std::shared_ptr<PriceLevel>> bidLevels;
    std::vector<std::shared_ptr<PriceLevel>> askLevels;
};

struct OrderMetaData {
    std::shared_ptr<OrderBook> orderBook;
    std::shared_ptr<PriceLevel> priceLevel;
    std::shared_ptr<Order> nextOrder;
    std::shared_ptr<Order> prevOrder;

    OrderMetaData(std::shared_ptr<OrderBook> orderBook,
        std::shared_ptr<PriceLevel> priceLevel,
        std::shared_ptr<Order> nextOrder = nullptr,
        std::shared_ptr<Order> prevOrder = nullptr)
        : orderBook(orderBook)
        , priceLevel(priceLevel)
        , nextOrder(nextOrder)
        , prevOrder(prevOrder)
    {
    }
};

std::unordered_map<std::string, OrderMetaData> orderMap;
std::unordered_map<std::string, std::shared_ptr<OrderBook>> orderBookMap;

std::shared_ptr<PriceLevel> findPriceLevel(std::shared_ptr<Order> order, std::shared_ptr<OrderBook> orderBook)
{
    std::vector<std::shared_ptr<PriceLevel>>& levels = (order->side == Side::BUY) ? orderBook->bidLevels : orderBook->askLevels;
    // Iterate over the price levels from the end of the vector
    for (auto it = levels.rbegin(); it != levels.rend(); ++it) {
        if ((*it)->price == order->price) {
            return *it;
        }
    }
    return nullptr;
}

void insert(std::shared_ptr<Order> order)
{
    auto it = orderBookMap.find(order->id);
    if (it == orderBookMap.end()) {
        throw runtime_error("OrderBook not found for order " + order->id);
    }
    std::shared_ptr<OrderBook> orderBook = it->second;

    std::shared_ptr<PriceLevel> priceLevel = findPriceLevel(order, orderBook);

    if (!priceLevel) {
        priceLevel = std::make_shared<PriceLevel>(order->price);
        if (order->side == Side::BUY) {
            orderBook->bidLevels.push_back(priceLevel);
        } else {
            orderBook->askLevels.push_back(priceLevel);
        }
    }

    OrderMetaData metaData(orderBook, priceLevel);
    if (!priceLevel->orders.empty()) {
        metaData.prevOrder = priceLevel->orders.back();
        orderMap[priceLevel->orders.back()->id].nextOrder = order;
    }

    priceLevel->orders.push_back(order);
    priceLevel->lastOrder = order;
    if (priceLevel->orders.size() == 1) {
        priceLevel->firstOrder = order;
    }
    priceLevel->totalQuantity += order->qty;
    priceLevel->numberOfOrders++;

    orderMap[order->id] = metaData;
}

void deleteOrder(int orderId)
{
    // Find the metadata for the order
    auto it = orderMap.find(orderId);
    if (it != orderMap.end()) {
        // Get the metadata
        OrderMetaData metaData = it->second;

        // Get the price level for the order
        std::shared_ptr<PriceLevel> priceLevel = metaData.priceLevel;

        // Find the order in the price level
        auto orderIt = std::find(priceLevel->orders.begin(), priceLevel->orders.end(), metaData.prevOrder);
        if (orderIt != priceLevel->orders.end()) {
            // Remove the order from the price level
            priceLevel->orders.erase(orderIt);

            // Update the total quantity and number of orders at this price level
            priceLevel->totalQuantity -= (*orderIt)->qty;
            priceLevel->numberOfOrders--;

            // If this was the first or last order at this price level, update the firstOrder and lastOrder pointers
            if (priceLevel->firstOrder->id == orderId) {
                priceLevel->firstOrder = metaData.nextOrder;
            }
            if (priceLevel->lastOrder->id == orderId) {
                priceLevel->lastOrder = metaData.prevOrder;
            }

            // If there are orders before or after this order, update their nextOrder and prevOrder pointers
            if (metaData.prevOrder) {
                orderMap[metaData.prevOrder->id].nextOrder = metaData.nextOrder;
            }
            if (metaData.nextOrder) {
                orderMap[metaData.nextOrder->id].prevOrder = metaData.prevOrder;
            }
        }

        // Remove the metadata from the order map
        orderMap.erase(it);
    }
}

OrderMetaData* lookupOrder(int orderId)
{
    // Find the metadata for the order
    auto it = orderMap.find(orderId);
    if (it != orderMap.end()) {
        // Return a pointer to the metadata
        return &(it->second);
    }
    // If the order is not found, return nullptr
    return nullptr;
}

} // OrderBook

int main() { return 0; }
