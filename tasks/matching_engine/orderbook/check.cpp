#include <string>
#include <vector>

OrderBook::OrderBook(string sym)
    : symbol(sym)
{
}
shared_ptr<PriceLevel> OrderBook::findInsertionPosition(const Order& order) const
{
    const auto& levels = (order.side == Side::BUY) ? bidLevels : askLevels;
    return *find_if(levels.crbegin(), levels.crend(), [&order](const auto& level) {
        return order.price >= level->price;
    });
}
shared_ptr<PriceLevel> OrderBook::findOrCreatePriceLevel(const Order& order)
{
    auto pos = findInsertionPosition(order); // Optional optimization
    if (!pos) {
        auto& levels = (order.side == Side::BUY) ? bidLevels : askLevels;
        return levels.emplace_back(make_shared<PriceLevel>(order.price));
    }
    return pos;
}
enum class OrderType {
    GFD,
    IOC
};
enum class Side {
    BUY,
    SELL
};
struct Order {
    string id;
    double price;
    double qty;
    OrderType type;
    Side side;
    string sym;
    friend ostream& operator<<(std::ostream& os, const Order& order);
};
struct PriceLevel {
    double price;
    shared_ptr<Order> firstOrder;
    shared_ptr<Order> lastOrder;
    vector<shared_ptr<Order>> orders;
    PriceLevel(double price);
    void addOrder(shared_ptr<Order>);
};
struct OrderBook {
    string symbol;
    vector<shared_ptr<PriceLevel>> bidLevels;
    vector<shared_ptr<PriceLevel>> askLevels;
    OrderBook(string sym);
    shared_ptr<PriceLevel> findInsertionPosition(const Order&) const;
    shared_ptr<PriceLevel> findOrCreatePriceLevel(const Order&);
};
struct OrderMetaData {
    shared_ptr<PriceLevel> priceLevel;
    shared_ptr<Order> nextOrder;
    shared_ptr<Order> prevOrder;
    OrderMetaData() = default;
    OrderMetaData(shared_ptr<PriceLevel>);
};
using OrderMetaDataByOrderIDMap = unordered_map<string, OrderMetaData>;
using OrderBookByOrderIDMap = unordered_map<string, OrderBook>;
OrderMetaDataByOrderIDMap orderMetaDataByOrderIDMap;
OrderBookByOrderIDMap orderBookByOrderIDMap;
void addOrderToBook(shared_ptr<Order> newOrder)
{
    auto& orderBook = orderBookByOrderIDMap.try_emplace(newOrder->id, OrderBook(newOrder->sym)).first->second;
    auto priceLevelPtr = orderBook.findOrCreatePriceLevel(*newOrder);
    if (!priceLevelPtr->orders.empty()) {
        auto lastOrderAtThisPrice = priceLevelPtr->orders.back();
        orderMetaDataByOrderIDMap[lastOrderAtThisPrice->id].nextOrder = newOrder;
    }
    priceLevelPtr->addOrder(newOrder);
    OrderMetaData newOrderMetaData(priceLevelPtr);
    newOrderMetaData.prevOrder = priceLevelPtr->orders.size() > 1 ? *(priceLevelPtr->orders.end() - 2) : nullptr;
    orderMetaDataByOrderIDMap[newOrder->id] = newOrderMetaData;
}
shared_ptr<PriceLevel> findInsertionPosition(const Order& order, OrderBook& orderBook);
shared_ptr<PriceLevel> getPriceLevel(const Order& order, OrderBook& orderBook);
void addOrderToBook(shared_ptr<Order> order, OrderBook& orderBook);
OrderMetaData::OrderMetaData(shared_ptr<PriceLevel> priceLevel)
    : priceLevel(priceLevel)
    , nextOrder(nullptr)
    , prevOrder(nullptr)
{
}
PriceLevel::PriceLevel(double price)
    : price(price)
    , firstOrder(nullptr)
    , lastOrder(nullptr)
{
}
void PriceLevel::addOrder(shared_ptr<Order> order)
{
    orders.emplace_back(order);

    lastOrder = order;
    if (orders.size() == 1) {
        firstOrder = order;
    }
}
