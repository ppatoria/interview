#include "orderbook.h"
#include <deque>
#include <format>
#include <string>
#include <utility>

using namespace std;

namespace OrderBook {

PriceLevel::PriceLevel(Price price)
    : price(price)
{
}

PriceLevel::PriceLevel(PriceLevel&& other) noexcept
{
    moveResourceFrom(std::move(other));
    other.setToDefaultState();
}

PriceLevel&
PriceLevel::operator=(PriceLevel&& other) noexcept
{
    if (this != &other) {
        moveResourceFrom(std::move(other));
        other.setToDefaultState();
    }
    return *this;
}

void PriceLevel::moveResourceFrom(PriceLevel&& other)
{
    price = std::move(other.price);
    orders = std::move(other.orders);
}

void PriceLevel::setToDefaultState()
{
    price = Price();
    orders = deque<Order>();
}

string
PriceLevel::toString() const
{
    return std::string();

    // string str = std::format("price: {} ", price);
    // str += "[";
    // for (const auto& order : orders) {
    //     str += order.id + ",";
    // }
    // str += "]";
    // return str;
}

const Order&
PriceLevel::enqueue(Order&& order)
{
    return orders.emplace_back(std::move(order));
}

std::pair<bool, PriceLevel::OrderIterator>
PriceLevel::findOrder(const Order& order)
{
    /* Search from the back(last element) towards the front(first element) */
    auto iter = find_if(orders.rbegin(), orders.rend(), [&order](const auto& ord) {
        return order.id == ord.id;
    });

    if (iter == orders.rend())
        return std::make_pair(false, orders.end());
    else
        return std::make_pair(true, iter.base());
}

int PriceLevel::removeOrder(const Order& order)
{
    return std::erase_if(orders,
        [&order](const auto& ord) {
            return order.id == ord.id;
        });
}

}
