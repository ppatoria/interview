#include "orderbook.h"
#include "../utils.h"
#include "algorithm"
#include "spdlog/spdlog.h"
#include <cassert>
#include <exception>
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


    std::expected<PriceLevel::OrderIterator, FailedSeachInfo>
    OrderBook::findOrder(const Order& order)
    {
        auto errorMessage = [&order](const string& object) {
            return std::format(
                "No [{}] found for cancel request with ID: [{}], Symbol: [{}] and Price: [{}]",
                object, order.id, order.sym, order.price);
        };

        auto& priceLevels = order.side == Side::BUY ? bidLevels : askLevels;
        auto priceLevelSearchResult = priceLevels.find(order.price);

        assert(priceLevelSearchResult);
        if(!priceLevelSearchResult){
            return std::unexpected(FailedSearchInfo { message = errorMessage("price level"); });
        }

        auto priceLevel = priceLevelSearchResult.second;
        auto orderSearchResult = priceLevel->findOrder(order);

        assert(orderSearchResult);
        if (!orderSearchResult) {
            return std::unexpected(FailedSearchInfo { message = errorMessage("order"); });
        }

        return orderSearchResult;
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
    auto priceLevelSearchResult = priceLevels.find(order.price);

    assert(priceLevelSearchResult.first == true);

    if (priceLevelSearchResult.first == false) {
        spdlog::error(errorMessage("price level"));
        return false;
    }

    auto priceLevel = priceLevelSearchResult.second;

    auto removed_count = priceLevel->removeOrder(order);

    assert(removed_count > 0);

    if (removed_count <= 0) {
        spdlog::error(errorMessage("order"));
        return false;
    }
    return true;
}

    const Order& OrderBook::modifyOrder(const Order& order){
        
    }
} // namespace OrderBook
