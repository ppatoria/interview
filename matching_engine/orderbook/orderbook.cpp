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
PriceLevel::PriceLevel(double price)
    : price(price)
    , firstOrder(nullptr)
    , lastOrder(nullptr)
{
}

/**
 * This code  adds the order to the orders of the price level and updates the lastOrder of the price level.
 * If this is the first order at this price level, it also updates the firstOrder.
 */
void PriceLevel::update(shared_ptr<Order> order)
{
    orders.push_back(std::move(order));

    lastOrder = order;
    if (orders.size() == 1) {
        firstOrder = order;
    }
}

shared_ptr<PriceLevel> OrderBook::findInsertionPosition(const Order& order)
{
    if (bestBid // if best bid return the last/back position.
        && order.side == Side::BUY
        && order.price >= *bestBid) {
        return bidLevels.back();
    } else if (bestAsk // if best ask return the front position.
        && order.side == Side::SELL
        && order.price <= *bestAsk) {
        return askLevels.front();
    }

    // Otherwise, use linear search (can be optimized further)
    auto& levels = (order.side == Side::BUY)
        ? bidLevels
        : askLevels;
    for (auto it = levels.rbegin(); it != levels.rend(); ++it) {
        if (order.price >= (*it)->price) {
            return *it;
        }
    }
    return nullptr;
}

/**
 * This method gets the price level for the order.
 * If the price level does not exist,
 * it creates a new one and adds it to the bid or ask levels of the order book.
 */
shared_ptr<PriceLevel> OrderBook::getPriceLevel(const Order& order)
{
    auto it = findInsertionPosition(order); // Optional optimization
    if (!it) {
        auto newPriceLevel = make_shared<PriceLevel>(order.price);
        if (order.side == Side::BUY) {
            bidLevels.push_back(newPriceLevel);
        } else {
            askLevels.push_back(newPriceLevel);
        }
        updateBestPrice(order);
        return newPriceLevel;
    }
    return it;
}
void OrderBook::updateBestPrice(const Order& order)
{
    if (order.side == Side::BUY) {
        bestBid = order.price;
    } else {
        bestAsk = order.price;
    }
}

/** This code creates an OrderMetaData object for the order.
 * If there are other orders at the same price level,
 * it sets the prevOrder of the new order to the last order at that price level,
 * and the nextOrder of the last order to the new order.
 */
OrderMetaData::OrderMetaData(shared_ptr<PriceLevel> priceLevel, shared_ptr<Order> order)
    : priceLevel(priceLevel)
    , nextOrder(nullptr)
    , prevOrder(nullptr)
{
    if (!priceLevel->orders.empty()) {
        prevOrder = priceLevel->orders.back();
        OrderMetaDataByOrderIDMap[priceLevel->orders.back()->id].nextOrder = order;
    }
}

void add(shared_ptr<Order> order, OrderBook& orderBook)
{
    auto priceLevel = orderBook.getPriceLevel(*order);
    priceLevel->update(order);
    OrderMetaData metaData(priceLevel, order);
    OrderMetaDataByOrderIDMap[order->id] = metaData;
}

// void remove(int orderId)
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
