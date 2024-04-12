#pragma once
#include "orderbook.h"
#include <string>

using namespace std;

namespace OrderBook {
shared_ptr<PriceLevel> findInsertionPosition(const Order& order, OrderBook& orderBook);
shared_ptr<PriceLevel> getPriceLevel(const Order& order, OrderBook& orderBook);
void addOrderToBook(shared_ptr<Order> order);
void removeOrderFromBook(const string& orderid);

}
