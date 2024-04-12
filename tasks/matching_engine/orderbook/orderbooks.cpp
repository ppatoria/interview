#include "../utils.h"
#include "orderbook.h"
#include <format>

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

const OrderBook&
OrderBooks::getOrAdd(const Symbol& symbol)
{
    return getOrAdd(OrderBook(symbol));
}

const OrderBook&
OrderBooks::getOrAdd(OrderBook&& orderBook)
{
    return orderBookBySymbolMap.try_emplace(orderBook.symbol, std::move(orderBook)).first->second;
}

const OrderBook
OrderBooks::remove(const Symbol& sym)
{
    auto iter = orderBookBySymbolMap.find(sym);

    if (iter == orderBookBySymbolMap.end()) {
        throw runtime_error(std::format("OrderBook with symbol [{}] not found.", sym));
    }

    auto removed = std::move(iter->second);
    orderBookBySymbolMap.erase(iter);
    return removed;
}

} // namespace OrderBook
