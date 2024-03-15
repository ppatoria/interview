#include "orderbookhandler.h"
#include "algorithm"
#include "orderbook.h"
#include <iostream>

using namespace std;

namespace OrderBook {

OrderMetaDataByOrderIDMap orderMetaDataByOrderIDMap;
OrderBookByOrderIDMap orderBookByOrderIDMap;
OrderBookBySymbolMap orderBookBySymbolMap;

/**


Now, let's look at how `OrderMetaData` is used in the `addOrder` function:

1. **Create `OrderMetaData` for new order**: When a new order is added, a new `OrderMetaData` object is created for it. The `priceLevel` pointer is set to the price level of the order.

2. **Link new order to previous order**: If there are already other orders at the same price level, the new order is linked to the previous order (`priceLevel->orders.back()`) by setting `metaData.prevOrder` to the previous order and setting the `nextOrder` of the previous order's metadata to the new order.

3. **Store `OrderMetaData` in map**: The `OrderMetaData` for the new order is then stored in `orderMetaDataByOrderIDMap` with the order's ID as the key. This allows for quick access to the order's metadata later on.

So, the purpose of `OrderMetaData` is to keep track of the relationship between an order and its price level and neighboring orders, and it's used in the `addOrder` function to update these relationships when a new order is added. I hope this helps! Let me know if you have any other questions.
*/
void addOrderToBook(shared_ptr<Order> newOrder)
{
    auto& orderBook = orderBookBySymbolMap.try_emplace(newOrder->sym, OrderBook(newOrder->sym)).first->second;

    auto priceLevelPtr = orderBook.findOrCreatePriceLevel(*newOrder);

    if (!priceLevelPtr->orders.empty()) {
        auto lastOrderAtThisPrice = priceLevelPtr->orders.back();
        orderMetaDataByOrderIDMap[lastOrderAtThisPrice->id].nextOrder = newOrder;
    }

    priceLevelPtr->addOrder(newOrder);

    OrderMetaData newOrderMetaData(priceLevelPtr);
    newOrderMetaData.prevOrder = priceLevelPtr->orders.size() > 1 ? *(priceLevelPtr->orders.end() - 2) : nullptr;

    orderMetaDataByOrderIDMap[newOrder->id] = newOrderMetaData;
    cout << "Order          :" << *newOrder << endl;
    cout << "PriceLevel     :" << *priceLevelPtr << endl;
    // cout << "OrderMetaData  :" << newOrderMetaData << endl;
    // cout << "OrderBook      :" << orderBook << endl;
}

void removeOrder(const Order& order)
{
    // Find the metadata for the order
    auto it = orderMetaDataByOrderIDMap.find(order.id);
    if (it != orderMetaDataByOrderIDMap.end()) {
        // Get the metadata
        OrderMetaData metaData = it->second;

        // Get the price level for the order
        auto priceLevel = metaData.priceLevel;

        // Find the order in the price level
        auto orderIt = std::find(priceLevel->orders.begin(), priceLevel->orders.end(), metaData.prevOrder);
        if (orderIt != priceLevel->orders.end()) {
            // Remove the order from the price level
            priceLevel->orders.erase(orderIt);

            // If this was the first or last order at this price level, update the firstOrder and lastOrder pointers
            if (priceLevel->firstOrder->id == order.id) {
                priceLevel->firstOrder = metaData.nextOrder;
            }
            if (priceLevel->lastOrder->id == order.id) {
                priceLevel->lastOrder = metaData.prevOrder;
            }

            // If there are orders before or after this order, update their nextOrder and prevOrder pointers
            if (metaData.prevOrder) {
                orderMetaDataByOrderIDMap[metaData.prevOrder->id].nextOrder = metaData.nextOrder;
            }
            if (metaData.nextOrder) {
                orderMetaDataByOrderIDMap[metaData.nextOrder->id].prevOrder = metaData.prevOrder;
            }
        }

        // Remove the metadata from the order map
        orderMetaDataByOrderIDMap.erase(it);
    }
}

} // namespace OrderBook
