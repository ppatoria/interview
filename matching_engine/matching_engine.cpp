#include <algorithm>
#include <climits>
#include <cstdint>
#include <functional>
#include <iostream>
#include <optional>
#include <set>
#include <stack>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>

using namespace std;

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
    uint64_t price;
    uint64_t qty;
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

struct CancelOrder {
    string id;
};

struct ModifyOrder {
    string id;
    uint64_t price;
    uint64_t qty;
    Side side;
};

/**
 *NOTE:
 * If the Order ID does not exist simply do nothing.
 *
 * Any input with price <= 0 or quantity <= 0 or empty order ID is invalid and
 * should be ignored by the matching engine.
 */
template <typename T>
concept HasPriceAndQty = requires(T order)
{
    order.price;
    order.qty;
};

template <typename T>
concept HasID = requires(T order)
{
    order.id;
};

bool isModifiable(const Order& order)
{
    return order.type == OrderType::IOC;
};

template <typename T>
concept HasOnlyID = HasID<T> && !HasPriceAndQty<T>;

template <HasPriceAndQty T>
bool isValid(const T& order)
{
    return !(order.id.empty() || order.price <= 0 || order.qty <= 0);
};

template <HasOnlyID T>
bool isValid(const T& order)
{
    return !order.id.empty();
}

using OrderRequest = variant<Order, CancelOrder, ModifyOrder>;
/**
 *NOTE:
 *
 * The IOC order means if it cannot be traded immediately,
 * it will be cancelled right away.
 * If it is only partially traded, the non-traded part is cancelled.
 *
 * Simply put orders that are equally priced are traded out in the order they
 * are received at the matching engine.
 *
 * There one more "property" to note of the MODIFY operation.
 * When are order is modified will lose its original priority (place).
 *
 */
struct Orders {
public:
    /**
     * Order ID will always be unique for all active orders in the matching
     * engine, otherwise the given operation (Buy, Sell, Modify Cancel etc) should
     * be ignored.
     */
    template <typename OrderSet>
    void safeInsert(OrderSet& orderSet, const Order& order)
    {
        auto result = orderSet.insert(order);
        if (result.second) {
            visit([&](auto it) { it = result.first; }, orderLookup[order.id]);
        } else {
            throw runtime_error("Order with id: [" + result.first->id + "] already exists.");
        }
    }

    template <typename T>
    void insert(const T& order)
    {
        if (orderLookup.count(order.id)) {
            cout << "Order id: [" + order.id + "] already exists. Ignoring the order," << endl;
            return;
        }

        if (order.side == Side::BUY) {
            safeInsert(buyOrders, order);
        } else if (order.side == Side::SELL) {
            safeInsert(sellOrders, order);
        } else {
            cout << "Ignoring invalid Order: " << order << endl; // log as error and continue.
        }
    }

    void erase(const std::string& orderid)
    {
        if (contains(orderid)) {
            std::visit(
                [&](auto it) {
                    if constexpr (std::is_same_v<decltype(it), SellOrderIterator>) {
                        sellOrders.erase(*it);
                    } else if constexpr (std::is_same_v<decltype(it), BuyOrderIterator>) {
                        buyOrders.erase(*it);
                    }
                },
                orderLookup[orderid]);
            orderLookup.erase(orderid);
        }
    }

    std::optional<Order> get(const std::string& orderid)
    {
        if (contains(orderid)) {
            return visit(
                [&](auto it) -> Order {
                    return *it;
                },
                orderLookup[orderid]);
        }
        return nullopt;
    }

    bool contains(const std::string& orderid)
    {
        return orderLookup.find(orderid) != orderLookup.end();
    }

    /**
     * The sequence for order1 and order2 is decided by which order was
     * processed by the matching engine first.
     *
     * The "TRADE order1 price1 qty1 order2 price2 qty2"
     * message should be output every time a trade
     * occurs within the the matching engine.**
     */
    friend std::ostream& operator<<(std::ostream& os, const Orders& orders)
    {
        os << "SELL:\n";
        for (auto it = orders.sellOrders.rbegin(); it != orders.sellOrders.rend(); ++it) {
            os << it->price << " " << it->qty << '\n';
        }

        os << "BUY:\n";
        for (auto it = orders.buyOrders.rbegin(); it != orders.buyOrders.rend(); ++it) {
            os << it->price << " " << it->qty << '\n';
        }

        return os;
    }

private:
    /**
     *NOTE:
     * The price for SELL section must be decreasing (descending) order and
     * correspondingly the price for BUY section must also be decreasing order.
     */

    function<bool(const Order&, const Order&)> sellOrderComparer = [](const Order& lhs, const Order& rhs) {
        return lhs.price < rhs.price;
    };
    function<bool(const Order&, const Order&)> buyOrderComparer = [](const Order& lhs, const Order& rhs) {
        return lhs.price > rhs.price;
    };

    using SellOrderSet = std::set<Order, decltype(sellOrderComparer)>;
    using BuyOrderSet = std::set<Order, decltype(buyOrderComparer)>;

    using SellOrderIterator = SellOrderSet::iterator;
    using BuyOrderIterator = BuyOrderSet::iterator;

    using OrderIterator = std::variant<SellOrderIterator, BuyOrderIterator>;

    SellOrderSet sellOrders { sellOrderComparer };
    BuyOrderSet buyOrders { buyOrderComparer };
    std::unordered_map<std::string, OrderIterator> orderLookup;
};

Orders orders;
struct OrderProcessor {

    void operator()(Order& order)
    {
        if (isValid(order)) {
            orders.insert(order);
        }
    }

    void operator()(CancelOrder& cancelOrder)
    {
        if (isValid(cancelOrder)) {
            orders.erase(cancelOrder.id);
        }
    }

    void operator()(ModifyOrder& modifyOrder)
    {
        if (!isValid(modifyOrder)) {
            return;
        };

        auto mayBeOrder = orders.get(modifyOrder.id);

        if (!mayBeOrder.has_value()) {
            throw std::runtime_error("Not able to get order with orderid: " + modifyOrder.id + " from Orders.");
        }

        auto& existingOrder = mayBeOrder.value();
        if (!isModifiable(existingOrder)) {
            return;
        };

        orders.erase(modifyOrder.id);
        existingOrder.price = modifyOrder.price;
        existingOrder.qty = modifyOrder.qty;
        existingOrder.side = modifyOrder.side;
        orders.insert(existingOrder);
    }
};

int main()
{
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    string request;
    while (cin >> request) {
        OrderRequest orderRequest;
        if (request == "BUY" || request == "SELL") {
            Order order;
            string orderType;
            cin >> orderType >> order.price >> order.qty >> order.id;
            order.type = (orderType == "GFD") ? OrderType::GFD : OrderType::IOC;
            order.side = (request == "BUY") ? Side::BUY : Side::SELL;
            orderRequest = order;
        } else if (request == "CANCEL") {
            CancelOrder cancelOrder;
            cin >> cancelOrder.id;
            orderRequest = cancelOrder;
        } else if (request == "MODIFY") {
            ModifyOrder modifyOrder;
            cin >> modifyOrder.id >> request >> modifyOrder.price >> modifyOrder.qty;
            modifyOrder.side = (request == "BUY") ? Side::BUY : Side::SELL;
            orderRequest = modifyOrder;
        }
        std::visit(OrderProcessor(), orderRequest);
        cout << orders << endl;
    }
    cout << orders << endl;
    return 0;
}
