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

void insertOrder(std::shared_ptr<Order> order, std::shared_ptr<OrderBook> orderBook, std::shared_ptr<PriceLevel> priceLevel)
{
    OrderMetaData metaData = { orderBook, priceLevel, nullptr, nullptr };
    orderMap[order->id] = metaData;
    // Insert the order into the price level and update nextOrder and prevOrder as needed.
}

void deleteOrder(int orderId)
{
    auto it = orderMap.find(orderId);
    if (it != orderMap.end()) {
        // Remove the order from the price level and update nextOrder and prevOrder as needed.
        orderMap.erase(it);
    }
}

OrderMetaData* lookupOrder(int orderId)
{
    auto it = orderMap.find(orderId);
    if (it != orderMap.end()) {
        return &(it->second);
    }
    return nullptr;
}

// // struct OrderProcessor {

// //     void operator()(Order& order)
// //     {
// //         if (isValid(order)) {
// //             orders.insert(order);
// //         }
// //     }

// //     void operator()(CancelOrder& cancelOrder)
// //     {
// //         if (isValid(cancelOrder)) {
// //             orders.erase(cancelOrder.id);
// //         }
// //     }

// //     void operator()(ModifyOrder& modifyRequest)
// //     {
// //         if (!isValid(modifyRequest)) {
// //             return;
// //         };

// //         if (!orders.contains(modifyRequest.id)) {
// //             std::cout << "WARN: Not able to find in the Order Book. Ignoring [" << modifyRequest << "]" << endl;
// //             return;
// //         }

// //         Order newOrderFromExisting = orders.at(modifyRequest.id);
// //         cout << "got existing order: " << newOrderFromExisting << endl;

// //         if (!isModifiable(newOrderFromExisting)) {
// //             std::cout << "WARN: Order can't be modified. Ignoring [" << modifyRequest << "]" << endl;
// //             return;
// //         };

// //         newOrderFromExisting.price = modifyRequest.price;
// //         newOrderFromExisting.qty = modifyRequest.qty;
// //         newOrderFromExisting.side = modifyRequest.side;

// //         orders.erase(modifyRequest.id);
// //         orders.insert(newOrderFromExisting);
// //     }
// // };

// #include <climits>
// #include <cstdint>
// #include <functional>
// #include <optional>
// #include <stack>
// #include <stdexcept>
// #include <type_traits>
// #include <unordered_map>
// #include <variant>

// using namespace std;

// enum class OrderType {
//     GFD,
//     IOC
// };

// enum class Side {
//     BUY,
//     SELL
// };

// template <typename T>
// concept IsProcessable = requires(T order)
// {
//     order.process();
// };

// template <typename T>
// concept IsVerifiable = requires(T order)
// {
//     order.isValid()->same_as<bool>;
// };

// template <typename Derived>
// requires IsVerifiable<Derived>
// struct OrderProcessorInterface {
//     void process()
//     {
//         static_cast<Derived*>(this)->process();
//     }
// };

// struct Order {
//     string id;
//     uint64_t price;
//     uint64_t qty;
//     OrderType type;
//     Side side;

//     friend std::ostream& operator<<(std::ostream& os, const Order& order)
//     {
//         os << order.id << "\t"
//            << order.price << "\t"
//            << order.qty << "\t"
//            << (order.type == OrderType::GFD ? "GFD" : "IOC") << "\t"
//            << (order.side == Side::BUY ? "BUY" : "SELL") << "\n";
//         return os;
//     }

//     void process()
//     {
//         if (isValid()) {
//             OrderBook.insert(*this);
//         } else {
//             cout << "WARN: Ignoring invalid order: [" << *this << "]";
//         }
//     }

//     bool isValid()
//     {
//         return !(id.empty() || price <= 0 || qty <= 0);
//     }
// };

// struct CancelOrder : public Base<CancelOrder> {
//     string id;

//     void process()
//     {
//         if (isValid()) {
//             OrderBook.erase(id);
//         }
//     }

//     bool isValid()
//     {
//         return !(id.empty());
//     }

//     friend std::ostream& operator<<(std::ostream& os, const CancelOrder& order)
//     {
//         os << order.id << "\n";
//         return os;
//     }
// };

// struct ModifyOrder : public Base<ModifyOrder> {
//     string id;
//     uint64_t price;
//     uint64_t qty;
//     Side side;

//     bool isValid()
//     {
//         return !(id.empty() || price <= 0 || qty <= 0);
//     }

//     friend std::ostream& operator<<(std::ostream& os, const ModifyOrder& order)
//     {
//         os << order.id << "\t"
//            << order.price << "\t"
//            << order.qty << "\t"
//            << (order.side == Side::BUY ? "BUY" : "SELL") << "\n";
//         return os;
//     }
// };

// /**
//  *NOTE:
//  * If the Order ID does not exist simply do nothing.
//  *
//  * Any input with price <= 0 or quantity <= 0 or empty order ID is invalid and
//  * should be ignored by the matching engine.
//  */
// template <typename T>
// concept HasPriceAndQty = requires(T order)
// {
//     order.price;
//     order.qty;
// };

// template <typename T>
// concept HasID = requires(T order)
// {
//     order.id;
// };

// template <typename T>
// concept HasOnlyID = HasID<T> && !HasPriceAndQty<T>;

// template <HasPriceAndQty T>
// bool isValid(const T& order)
// {
//     return !(order.id.empty() || order.price <= 0 || order.qty <= 0);
// };

// template <HasOnlyID T>
// bool isValid(const T& order)
// {
//     return !order.id.empty();
// }

// /**
//  * NOTE : We cannot modify an IOC order type, as will be explained later.
//  */
// bool isModifiable(const Order& order)
// {
//     if (order.type == OrderType::GFD) {
//         cout << "WARN:  Order : " << order << " with OrderType:GFD cannot be modified." << endl;
//         return false;
//     }
//     return true;
// }

// using OrderRequest = variant<Order, CancelOrder, ModifyOrder>;
// /**
//  *NOTE:
//  *
//  * The IOC order means if it cannot be traded immediately,
//  * it will be cancelled right away.
//  * If it is only partially traded, the non-traded part is cancelled.
//  *
//  *
//  * There one more "property" to note of the MODIFY operation.
//  * When are order is modified will lose its original priority (place).
//  *
//  */
// struct Orders {
// public:
//     /**
//      * Order ID will always be unique for all active orders in the matching
//      * engine, otherwise the given operation (Buy, Sell, Modify Cancel etc) should
//      * be ignored.
//      */
//     template <typename OrderSet>
//     void actualInsert(OrderSet& orderSet, const Order& order)
//     {
//         auto iter = orderSet.insert(order);
//         visit([&](auto it) { it = iter; }, orderLookup[order.id]);
//     }

//     template <typename T>
//     void insert(const T& order)
//     {
//         if (orderLookup.count(order.id)) {
//             cout << "Order id: [" + order.id + "] already exists. Ignoring the order," << endl;
//             return;
//         }

//         if (order.side == Side::BUY) {
//             actualInsert(buyOrders, order);
//         } else if (order.side == Side::SELL) {
//             actualInsert(sellOrders, order);
//         } else {
//             cout << "Ignoring invalid Order: " << order << endl; // log as error and continue.
//         }
//     }

//     void erase(const std::string& orderid)
//     {
//         if (contains(orderid)) {
//             std::visit(
//                 [&](auto it) {
//                     if constexpr (std::is_same_v<decltype(it), SellOrderIterator>) {
//                         sellOrders.erase(*it);
//                     } else if constexpr (std::is_same_v<decltype(it), BuyOrderIterator>) {
//                         buyOrders.erase(*it);
//                     }
//                 },
//                 orderLookup[orderid]);
//             orderLookup.erase(orderid);
//         }
//     }

//     Order at(const std::string& orderid)
//     {
//         Order order;
//         auto orderIterator = orderLookup[orderid];
//         visit(
//             [&](auto iter) {
//                 order.id = iter->id;
//             },
//             orderIterator);
//         return order;
//     }

//     bool contains(const std::string& orderid)
//     {
//         return orderLookup.find(orderid) != orderLookup.end();
//     }

//     /**
//      */
//     friend std::ostream& operator<<(std::ostream& os, const Orders& orders)
//     {
//         os << "SELL:\n";
//         for (auto it = orders.sellOrders.rbegin(); it != orders.sellOrders.rend(); ++it) {
//             os << it->price << " " << it->qty << '\n';
//         }

//         os << "BUY:\n";
//         for (auto it = orders.buyOrders.rbegin(); it != orders.buyOrders.rend(); ++it) {
//             os << it->price << " " << it->qty << '\n';
//         }

//         return os;
//     }

// private:
//     /**
//      *NOTE Requirement:
//      * The price for SELL section must be decreasing (descending) order and
//      * correspondingly the price for BUY section must also be decreasing order.
//      *
//      * Comparers as per the above requirement.
//      */

//     function<bool(const Order&, const Order&)> sellOrderComparer = [](const Order& lhs, const Order& rhs) {
//         return lhs.price < rhs.price;
//     };
//     function<bool(const Order&, const Order&)> buyOrderComparer = [](const Order& lhs, const Order& rhs) {
//         return lhs.price > rhs.price;
//     };

//     /**
//      * NOTE Requirement:
//      * Simply put orders that are equally priced are traded out in the order they are received at the matching engine.
//      * The sequence for order1 and order2 is decided by which order was processed by the matching engine first.
//      * The "TRADE order1 price1 qty1 order2 price2 qty2 message should be output every time a trade occurs within the the matching engine.

//      * As per cpprefernce for multiset: "If the container has elements with equivalent key, inserts at the upper bound of that range."
//      * This feature takes care of the above requirement.
//      */

//     using SellOrderSet = std::multiset<Order, decltype(sellOrderComparer)>;
//     using BuyOrderSet = std::multiset<Order, decltype(buyOrderComparer)>;

//     using SellOrderIterator = SellOrderSet::iterator;
//     using BuyOrderIterator = BuyOrderSet::iterator;

//     using OrderIterator = std::variant<SellOrderIterator, BuyOrderIterator>;

//     SellOrderSet sellOrders { sellOrderComparer };
//     BuyOrderSet buyOrders { buyOrderComparer };
//     std::unordered_map<std::string, OrderIterator> orderLookup;
// };

// Orders orders;

// int main()
// {
//     /* Enter your code here. Read input from STDIN. Print output to STDOUT */
//     string request;
//     while (cin >> request) {
//         OrderRequest orderRequest;
//         if (request == "BUY" || request == "SELL") {
//             Order order;
//             string orderType;
//             cin >> orderType >> order.price >> order.qty >> order.id;
//             order.type = (orderType == "GFD") ? OrderType::GFD : OrderType::IOC;
//             order.side = (request == "BUY") ? Side::BUY : Side::SELL;
//             orderRequest = order;
//         } else if (request == "CANCEL") {
//             CancelOrder cancelOrder;
//             cin >> cancelOrder.id;
//             orderRequest = cancelOrder;
//         } else if (request == "MODIFY") {
//             ModifyOrder modifyOrder;
//             cin >> modifyOrder.id >> request >> modifyOrder.price >> modifyOrder.qty;
//             modifyOrder.side = (request == "BUY") ? Side::BUY : Side::SELL;
//             orderRequest = modifyOrder;
//         }
//         std::visit(OrderProcessor(), orderRequest);
//         cout << orders << endl;
//     }
//     cout << orders << endl;
//     return 0;
// }
