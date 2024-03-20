#include "build/thirdparty/SPDLOG/include/spdlog/spdlog.h"
#include "orderbook/orderbook.h"
#include "orderbook/orderbookhandler.h"
#include "spdlog/spdlog.h"

using namespace std;
using namespace OrderBook;

int main()
{
    // spdlog::set_level(spdlog::level::debug);

    // spdlog::debug("Order 1");
    // addOrderToBook(make_shared<Order>("1", 11.00, 11, OrderType::GFD, Side::BUY, "IBM"));

    // spdlog::debug("Order 2");
    // addOrderToBook(make_shared<Order>("2", 11.00, 11, OrderType::GFD, Side::BUY, "IBM"));

    // spdlog::debug("Order 3");
    // addOrderToBook(make_shared<Order>("3", 21.00, 21, OrderType::GFD, Side::BUY, "IBM"));
    // string orderid = "1";
    // removeOrderFromBook(orderid);
    return 0;
}
