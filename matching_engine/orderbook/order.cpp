#include "orderbook.h"

using namespace std;

namespace OrderBook {

ostream& operator<<(ostream& os, const Order& order)
{
    os << order.id << "\t"
       << order.price << "\t"
       << order.qty << "\t"
       << (order.type == OrderType::GFD ? "GFD" : "IOC") << "\t"
       << (order.side == Side::BUY ? "BUY" : "SELL") << "\t"
       << order.sym << "\n";
    return os;
}
}
