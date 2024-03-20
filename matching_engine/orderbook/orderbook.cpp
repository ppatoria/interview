#include "orderbook.h"
#include "../utils.h"
#include "algorithm"
#include "orderbookhandler.h"
#include <format>
#include <stdexcept>
#include <string>

using namespace std;

namespace OrderBook {

bool OrderBooks::contains(const Symbol& sym) const
{
    return orderBookBySymbolMap.contains(sym);
}

optional<reference_wrapper<const OrderBook>>
OrderBooks::get(const Symbol& sym) const
{
    const auto& constOrderBooks = orderBookBySymbolMap;
    auto iter = constOrderBooks.find(sym);
    if (iter == constOrderBooks.cend())
        return nullopt;
    return iter->second;
}

const OrderBook& OrderBooks::getOrAdd(const Symbol& symbol)
{
    return getOrAdd(OrderBook(symbol));
}

const OrderBook& OrderBooks::getOrAdd(OrderBook&& orderBook)
{
    return orderBookBySymbolMap.try_emplace(orderBook.symbol, std::move(orderBook)).first->second;
}

const OrderBook OrderBooks::remove(const Symbol& sym)
{
    auto iter = orderBookBySymbolMap.find(sym);

    if (iter == orderBookBySymbolMap.end()) {
        throw runtime_error(std::format("OrderBook with symbol [{}] not found.", sym));
    }

    auto removed = std::move(iter->second);
    orderBookBySymbolMap.erase(iter);
    return removed;
}

OrderBook::OrderBook(string sym)
    : symbol(std::move(sym))
{
}

optional<reference_wrapper<PriceLevel>> OrderBook::findInsertionPosition(const Order& order)
{
    auto& levels = (order.side == Side::BUY) ? bidLevels : askLevels;
    auto iter = find_if(levels.rbegin(), levels.rend(), [&order](const auto& level) {
        return order.price >= level.price;
    });
    if (iter == levels.rend())
        return nullopt;
    else
        return ref(*iter);
}

/**
 * This method gets the price level for the order.
 * If the price level does not exist,
 * it creates a new one and adds it to the bid or ask levels of the order book.
 */
PriceLevel& OrderBook::findOrCreatePriceLevel(const Order& order)
{
    auto pos = findInsertionPosition(order); // Optional optimization
    if (pos) {
        auto& levels = (order.side == Side::BUY) ? bidLevels : askLevels;
        pos = levels.emplace_back(order.price);
    }
    return *pos;
}
string OrderBook::toString() const
{
    return string();
}

const Order& OrderBook::newOrder(Order&& order)
{
    auto& priceLevel = findOrCreatePriceLevel(order);
    return priceLevel.orders.emplace_back(std::move(order));
}
const Order& OrderBook::cancelOrder(const Order& order)
{
    return Order();

    // auto& levels = (order.side == Side::BUY) ? bidLevels : askLevels;
    // auto iter = find_if(levels.rbegin(), levels.rend(), [&order](const auto& level) {
    //     return order.price >= level.price;
    // });
    // if (iter == levels.rend()) {
    //     const auto& error = std::format(
    //         "No PriveLevel found for cancel request with ID: [{}], Symbol: [{}] and Price: [{}]",
    //         order.id, order.sym, order.price);
    //     throw runtime_error(error);
    // } else {

    //     return *removed_iter;
    // }
}

const Order& modifyOrder(const Order& order)
{
    return Order();
}

} // namespace OrderBook
