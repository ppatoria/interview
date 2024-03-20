#include "orderbook.h"
#include <deque>
#include <format>
#include <string>

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

PriceLevel& PriceLevel::operator=(PriceLevel&& other) noexcept
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

string PriceLevel::toString() const
{

    string str = std::format("price: {} ", price);
    str += "[";
    for (const auto& order : orders) {
        str += order.id + ",";
    }
    str += "]";
    return str;
}

}
