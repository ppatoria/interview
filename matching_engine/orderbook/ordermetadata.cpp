#include "orderbook.h"

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
}
