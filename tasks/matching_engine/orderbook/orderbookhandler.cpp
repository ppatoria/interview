#include "../logger.h"
#include "../utils.h"
#include "algorithm"
#include "orderbook.h"
#include "spdlog/spdlog.h"
#include <format>
#include <stdexcept>
using namespace std;

namespace OrderBook {

const Order&
OrderBookHandler::newOrder(Order&& order)
{
    const auto& orderBook = orderBooks.getOrAdd(order.sym);
    return orderBook.newOrder(order);
}

const Order&
OrderBookHandler::cancelOrder(const Order& order)
{
  auto orderBook = orderBooks.get(order.sym));
  if (!orderBook) {
      throw std::runtime_error(std::format("No OrderBook found for symbol [{}]", order.sym));
  }
  return orderBook.cancelOrder(order);
}

} // namespace OrderBook
