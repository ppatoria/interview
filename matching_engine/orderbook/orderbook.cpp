#include "orderbook.h"
#include "../utils.h"
#include "algorithm"
#include "orderbookhandler.h"
#include "spdlog/spdlog.h"
#include <cassert>
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

PriceLevel&
 PriceLevels::add(const Price& price) {
     auto result = find(price);
     if(result.first)
         return *result.second;
     return priceLevels.emplace_back(price);
 }


std::pair<bool, PriceLevels::iterator>
PriceLevels::find(const Price& price)
{
    auto iter = find_if(priceLevels.rbegin(), priceLevels.rend(), [&price](const auto& priceLevel) {
        return price == priceLevel.price;
    });
    if (iter == priceLevels.rend())
        return std::make_pair( false, priceLevels.end() );
    else
        return std::make_pair(true, iter.base());
}

const Order&
PriceLevel::enqueue(Order&& order){
    return orders.emplace_back(std::move(order));
}    

string OrderBook::toString() const
{
    return string();
}

const Order& OrderBook::newOrder(Order&& order)
{
    auto& priceLevels = order.side == Side::BUY ? bidLevels : askLevels;
    auto& priceLevel = priceLevels.add(order.price);
    return priceLevel.enqueue(std::move(order));
}

std::pair<bool, PriceLevel::OrderIterator>
PriceLevel::findOrder(const Order& order){
    /* Search from the back(last element) towards the front(first element) */
    auto iter = find_if(orders.rbegin(), orders.rend(), [&order](const auto& ord) {
        return order.id == ord.id;
    });

    if (iter == orders.rend())
        return std::make_pair(false, orders.end());
    else
        return std::make_pair(true, iter.base());
}

int
PriceLevel::removeOrder(const Order& order)
{
    return std::erase_if(orders,
                         [&order] (const auto& ord){
                           return order.id == ord.id;
                         });
}

bool OrderBook::cancelOrder(const Order& order)
{
    auto errorMessage = [&order](const string& object) {
        return std::format(
            "No [{}] found for cancel request with ID: [{}], Symbol: [{}] and Price: [{}]",
            object, order.id, order.sym, order.price);
    };

    auto& priceLevels = order.side == Side::BUY ? bidLevels : askLevels;
    auto result = priceLevels.find(order.price);

    assert(result.first == true);

    if (result.first == false) {
        spdlog::error(errorMessage("price level"));
        return false;
    }

    auto priceLevel = result.second;

    auto removed_count = priceLevel->removeOrder(order);

    assert(removed_count > 0);

    if (removed_count <= 0) {
        spdlog::error(errorMessage("order"));
        return false;
    }
    return true;
}

const Order& modifyOrder(const Order& order)
{
    return Order();
}

} // namespace OrderBook
