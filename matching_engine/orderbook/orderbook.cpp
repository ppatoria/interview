#include "orderbook.h"

using namespace std;

namespace OrderBook {

ostream& operator<<(ostream& os, const Order& order)
{
    os << order.id << "\t"
       << order.price << "\t"
       << order.qty << "\t"
       << (order.type == OrderType::GFD ? "GFD" : "IOC") << "\t"
       << (order.side == Side::BUY ? "BUY" : "SELL") << "\t"
       << order.sym << "\n";
    return os;
}

shared_ptr<PriceLevel> findInsertionPosition(const Order& order, OrderBook& orderBook)
{
    if (orderBook.bestBid && order.side == Side::BUY && order.price >= *orderBook.bestBid) {
        return orderBook.bidLevels.back(); // Insert at the end of bid levels
    } else if (orderBook.bestAsk && order.side == Side::SELL && order.price <= *orderBook.bestAsk) {
        return orderBook.askLevels.front(); // Insert at the beginning of ask levels
    }
    // Otherwise, use linear search (can be optimized further)
    auto& levels = (order.side == Side::BUY) ? orderBook.bidLevels : orderBook.askLevels;
    for (auto it = levels.rbegin(); it != levels.rend(); ++it) {
        if (order.price >= (*it)->price) {
            return *it;
        }
    }
    return nullptr;
}

shared_ptr<PriceLevel> getPriceLevel(const Order& order, OrderBook& orderBook)
{
    auto it = findInsertionPosition(order, orderBook); // Optional optimization
    if (!it) {
        auto newPriceLevel = make_shared<PriceLevel>(order.price);
        if (order.side == Side::BUY) {
            orderBook.bidLevels.push_back(newPriceLevel);
        } else {
            orderBook.askLevels.push_back(newPriceLevel);
        }
        orderBook.updateBestPrice(order); // Update best bid/ask (optional)
        return newPriceLevel;
    }
    return it;
}

void insert(shared_ptr<Order> order, OrderBook& orderBook)
{
    auto priceLevel = getPriceLevel(*order, orderBook);

    OrderMetaData metaData(priceLevel);
    if (!priceLevel->orders.empty()) {
        metaData.prevOrder = priceLevel->orders.back();
        OrderMap[priceLevel->orders.back()->id].nextOrder = order;
    }

    priceLevel->orders.push_back(std::move(order));

    priceLevel->lastOrder = order;
    if (priceLevel->orders.size() == 1) {
        priceLevel->firstOrder = order;
    }
    OrderMap[order->id] = metaData;
}

// void delete(int orderId)
// {
//     // Find the metadata for the order
//     auto it = orderMap.find(orderId);
//     if (it != orderMap.end()) {
//         // Get the metadata
//         OrderMetaData metaData = it->second;

//         // Get the price level for the order
//         std::shared_ptr<PriceLevel> priceLevel = metaData.priceLevel;

//         // Find the order in the price level
//         auto orderIt = std::find(priceLevel->orders.begin(), priceLevel->orders.end(), metaData.prevOrder);
//         if (orderIt != priceLevel->orders.end()) {
//             // Remove the order from the price level
//             priceLevel->orders.erase(orderIt);

//             // Update the total quantity and number of orders at this price level
//             priceLevel->totalQuantity -= (*orderIt)->qty;
//             priceLevel->numberOfOrders--;

//             // If this was the first or last order at this price level, update the firstOrder and lastOrder pointers
//             if (priceLevel->firstOrder->id == orderId) {
//                 priceLevel->firstOrder = metaData.nextOrder;
//             }
//             if (priceLevel->lastOrder->id == orderId) {
//                 priceLevel->lastOrder = metaData.prevOrder;
//             }

//             // If there are orders before or after this order, update their nextOrder and prevOrder pointers
//             if (metaData.prevOrder) {
//                 orderMap[metaData.prevOrder->id].nextOrder = metaData.nextOrder;
//             }
//             if (metaData.nextOrder) {
//                 orderMap[metaData.nextOrder->id].prevOrder = metaData.prevOrder;
//             }
//         }

//         // Remove the metadata from the order map
//         orderMap.erase(it);
//     }
// }

// OrderMetaData* lookupOrder(int orderId)
// {
//     // Find the metadata for the order
//     auto it = orderMap.find(orderId);
//     if (it != orderMap.end()) {
//         // Return a pointer to the metadata
//         return &(it->second);
//     }
//     // If the order is not found, return nullptr
//     return nullptr;
// }

} // namespace OrderBook
