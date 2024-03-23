#include "../utils.h"
#include "algorithm"
#include "orderbook.h"
#include <utility>

using namespace std;

namespace OrderBook {

PriceLevel&
PriceLevels::add(const Price& price)
{
    auto result = find(price);
    if (result.first)
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
        return std::make_pair(false, priceLevels.end());
    else
        return std::make_pair(true, iter.base());
}

} // namespace OrderBook
