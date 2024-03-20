#include "orderbook.h"
#include <format>
#include <string>
using namespace std;

namespace OrderBook {

Order::Order(OrderID _id, Price _price, Quantity quantity, OrderType orderType, Side _side, Symbol symbol)
    : id(_id)
    , price(_price)
    , qty(quantity)
    , type(orderType)
    , side(_side)
    , sym(symbol)
{
}

Order::Order(Order&& other) noexcept
{
    moveResourceFrom(std::move(other));
    other.setToDefaultState();
}

Order& Order::operator=(Order&& rhs) noexcept
{
    if (&rhs != this) {
        moveResourceFrom(std::move(rhs));
        rhs.setToDefaultState();
    }
    return *this;
}

unique_ptr<Order> Order::clone() const
{
    return make_unique<Order>(id, price, qty, type, side, sym);
}

void Order::moveResourceFrom(Order&& order)
{
    id = std::move(order.id);
    price = std::move(order.price);
    qty = std::move(order.qty);
    type = std::move(order.type);
    side = std::move(order.side);
    sym = std::move(order.sym);
}

void Order::setToDefaultState()
{
    id = OrderID();
    price = Price();
    qty = Quantity();
    type = OrderType();
    side = Side();
    sym = Symbol();
}

std::string Order::toString() const
{
    return std::format("[ {} {} {} {} {} {} ]",
        id, price, qty, sym,
        (type == OrderType::GFD ? "GFD" : "IOC"),
        (side == Side::BUY ? "BUY" : "SELL"));
}
} // namespace OrderBook
