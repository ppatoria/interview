#include "orderbook.h"
#include "../utils.h"
#include "algorithm"
#include "spdlog/spdlog.h"
#include <cassert>
#include <string>

using namespace std;

namespace OrderBook {

OrderBook::OrderBook(string sym)
    : symbol(std::move(sym))
{
}

string
OrderBook::toString() const
{
    return string();
}

const Order&
OrderBook::newOrder(Order&& order)
{
    auto& priceLevels = order.side == Side::BUY ? bidLevels : askLevels;
    auto& priceLevel = priceLevels.add(order.price);
    return priceLevel.enqueue(std::move(order));
}


bool
OrderBook::cancelOrder(const Order& order)
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


} // namespace OrderBook
