#include "orderbook.h"

using namespace std;

namespace OrderBook {

PriceLevel::PriceLevel(double price)
    : price(price)
    , firstOrder(nullptr)
    , lastOrder(nullptr)
{
}

/**
 * - adds the order to the list of orders of that price level.
 * - updates the lastOrder of that price level.
 * -- If this is the first order at this price level, it also updates the firstOrder.
 */
void PriceLevel::addOrder(shared_ptr<Order> order)
{
    orders.emplace_back(order);

    lastOrder = order;
    if (orders.size() == 1) {
        firstOrder = order;
    }
}
}
