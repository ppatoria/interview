#include <chrono>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

namespace OrderBook {

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

    friend std::ostream& operator<<(std::ostream& os, const Order& order)
    {
        os << order.id << "\t"
           << order.price << "\t"
           << order.qty << "\t"
           << (order.type == OrderType::GFD ? "GFD" : "IOC") << "\t"
           << (order.side == Side::BUY ? "BUY" : "SELL") << "\n";
        return os;
    }
};

struct PriceLevel {
    double price;
    int totalQuantity;
    int numberOfOrders;
    std::shared_ptr<Order> firstOrder;
    std::shared_ptr<Order> lastOrder;
    std::chrono::system_clock::time_point timestamp;
    std::vector<std::shared_ptr<Order>> orders;

    PriceLevel(double price)
        : price(price)
        , totalQuantity(0)
        , numberOfOrders(0)
        , firstOrder(nullptr)
        , lastOrder(nullptr)
        , timestamp(std::chrono::system_clock::now())
    {
    }
};

struct OrderBook {
    std::vector<std::shared_ptr<PriceLevel>> bidLevels;
    std::vector<std::shared_ptr<PriceLevel>> askLevels;
    bool isMarketOpen;
    int tradingVolume;
    double lastTradePrice;
    double highestBid;
    double lowestAsk;
    std::vector<std::pair<std::shared_ptr<Order>, std::chrono::system_clock::time_point>> tradeHistory;
    // Order matching algorithm details...

    OrderBook()
        : isMarketOpen(false)
        , tradingVolume(0)
        , lastTradePrice(0.0)
        , highestBid(0.0)
        , lowestAsk(0.0)
    {
    }
};

struct OrderMetaData {
    std::shared_ptr<OrderBook> orderBook;
    std::shared_ptr<PriceLevel> priceLevel;
    std::shared_ptr<Order> nextOrder;
    std::shared_ptr<Order> prevOrder;
};

std::unordered_map<int, OrderMetaData> orderMap;

} // OrderBook

int main() { return 0; }
