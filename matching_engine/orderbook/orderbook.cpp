#include "orderbook.h"
#include "algorithm"
#include <string>

using namespace std;

namespace OrderBook {

OrderBook::OrderBook(string sym)
    : symbol(sym)
{
}

shared_ptr<PriceLevel> OrderBook::findInsertionPosition(const Order& order) const
{
    const auto& levels = (order.side == Side::BUY) ? bidLevels : askLevels;
    return *find_if(levels.crbegin(), levels.crend(), [&order](const auto& level) {
        return order.price >= level->price;
    });
}

/**
 * This method gets the price level for the order.
 * If the price level does not exist,
 * it creates a new one and adds it to the bid or ask levels of the order book.
 */
shared_ptr<PriceLevel> OrderBook::findOrCreatePriceLevel(const Order& order)
{
    auto pos = findInsertionPosition(order); // Optional optimization
    if (!pos) {
        auto& levels = (order.side == Side::BUY) ? bidLevels : askLevels;
        return levels.emplace_back(make_shared<PriceLevel>(order.price));
    }
    return pos;
}

} // namespace OrderBook
