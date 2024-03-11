shared_ptr<PriceLevel> findInsertionPosition(const Order& order, OrderBook& orderBook)
{
    if (orderBook.bestBid && order.side == Side::BUY && order.price >= *orderBook.bestBid) {
        return orderBook.bidLevels.back(); // Insert at the end of bid levels
    } else if (orderBook.bestAsk && order.side == Side::SELL && order.price <= *orderBook.bestAsk) {
        return orderBook.askLevels.front(); // Insert at the beginning of ask levels
    }
    // Otherwise, use linear search (can be optimized further)
    auto& levels = (order.side == Side::BUY) ? orderBook.bidLevels : orderBook.askLevels;
    for (auto it = levels.rbegin(); it != levels.rend(); ++it) {
        if (order.price >= (*it)->price) {
            return *it;
        }
    }
    return nullptr;
}

shared_ptr<PriceLevel> getPriceLevel(const Order& order, OrderBook& orderBook)
{
    auto it = findInsertionPosition(order, orderBook); // Optional optimization
    if (!it) {
        auto newPriceLevel = make_shared<PriceLevel>(order.price);
        if (order.side == Side::BUY) {
            orderBook.bidLevels.push_back(newPriceLevel);
        } else {
            orderBook.askLevels.push_back(newPriceLevel);
        }
        orderBook.updateBestPrice(order); // Update best bid/ask (optional)
        return newPriceLevel;
    }
    return it;
}
