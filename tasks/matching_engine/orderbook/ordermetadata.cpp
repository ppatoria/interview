#include "orderbook.h"
#include <format>
#include <string>
using namespace std;

namespace OrderBook {

/** This code creates an OrderMetaData object for the order.
 * If there are other orders at the same price level,
 * it sets the prevOrder of the new order to the last order at that price level,
 * and the nextOrder of the last order to the new order.
 */
OrderMetaData::OrderMetaData(shared_ptr<PriceLevel> priceLevel)
    : priceLevel(priceLevel)
    , nextOrder(nullptr)
    , prevOrder(nullptr)
{
}
string OrderMetaData::toString()
{
    string str = "[";
    if (priceLevel)
        str += format("PriceLevel: {}", priceLevel->toString());
    if (nextOrder)
        str += format("\tnextOrder: {}", nextOrder->toString());
    if (prevOrder)
        str += format("\tprevOrder: {}", prevOrder->toString());
    str += "]";

    return str;
}
}
