#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <stdexcept>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <variant>
#include <functional>
#include <cstdint>
#include <type_traits>
#include <iostream>
#include <optional>

using namespace std;

enum class OrderType{
    GFD,
    IOC
};

enum class Side{
    BUY,
    SELL
};

struct Order{
    string id;
    uint64_t price;
    uint64_t qty;
    OrderType type;
    Side side;
};

struct CancelOrder{
    string id;
};

struct ModifyOrder{
    string id;
    uint64_t price;
    uint64_t qty;
    Side side;
};

using OrderRequest = variant<Order, CancelOrder, ModifyOrder>;

struct Orders {
public:
    template<typename T>
    void insert(const T& order) {
        if (orderLookup.count(order.id)) {
            throw std::runtime_error("Order id already exists: " + order.id);
        }
        if (order.side == Side::BUY) {
            orderLookup[order.id] = buyOrders.insert(order);
        } else {
            orderLookup[order.id] = sellOrders.insert(order);
        }
    }

    void erase(const std::string& orderid) {
        if (orderLookup.count(orderid)) {
          std::visit([&] (auto it){ it->second.erase(it->first); }, orderLookup[orderid]);
            orderLookup.erase(orderid);
        }
    }

    std::optional<Order> get(const std::string& orderid) {
        if(orderLookup.count(orderid))
            return *std::visit( { return &(*it); }, orderLookup[orderid]);
        return std::nullopt;
    }

    bool contains(const std::string& orderid) {
        return orderLookup.find(orderid) != orderLookup.end();
    }

    friend std::ostream &operator<<(std::ostream &os, const Orders &orders) {
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
    function<bool(const Order &, const Order &)> sellOrderComparer =
        [](const Order &lhs, const Order &rhs) {
          return lhs.price < rhs.price;
        };
    function<bool(const Order &, const Order &)> buyOrderComparer =
        [](const Order &lhs, const Order &rhs) {
          return lhs.price > rhs.price;
        };

    using SellOrderSet = std::multiset<Order, decltype(sellOrderComparer)>;
    using BuyOrderSet  = std::multiset<Order, decltype(buyOrderComparer)>;

    using SellOrderIterator = SellOrderSet::iterator;
    using BuyOrderIterator  = BuyOrderSet::iterator;

    using OrderIterator = std::variant<SellOrderIterator, BuyOrderIterator>;

    SellOrderSet sellOrders{sellOrderComparer};
    BuyOrderSet buyOrders{buyOrderComparer};
    std::unordered_map<std::string, OrderIterator> orderLookup;
};


Orders orders;
struct OrderProcessor {
    void operator()(Order& order) {
        orders.insert(order);
    }

    void operator()(CancelOrder& cancelOrder) {
        orders.erase(cancelOrder.id);
    }

    void operator()(ModifyOrder& modifyOrder) {
        if (orders.contains(modifyOrder.id)) {
            Order existingOrder = orders.get(modifyOrder.id);
            orders.erase(modifyOrder.id);
            existingOrder.price = modifyOrder.price;
            existingOrder.qty = modifyOrder.qty;
            existingOrder.side = modifyOrder.side;
            orders.insert(existingOrder);
        }
    }
};

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    string request;
    while(cin >> request){
        OrderRequest orderRequest;
        if(request == "BUY" || request == "SELL"){
            Order order;
            string orderType;
            cin >> orderType >> order.price >> order.qty >> order.id ;
            order.type = (orderType == "GFD") ? OrderType::GFD : OrderType::IOC;
            order.side = (request == "BUY") ? Side::BUY : Side::SELL;
            orderRequest = order;
        } else if (request == "CANCEL"){
            CancelOrder cancelOrder;
            cin >> cancelOrder.id;
            orderRequest = cancelOrder;
        } else if ( request == "MODIFY"){
            ModifyOrder modifyOrder;
            cin >> modifyOrder.id >> request >> modifyOrder.price >> modifyOrder.qty;
            modifyOrder.side = (request == "BUY") ? Side::BUY : Side::SELL;
            orderRequest = modifyOrder;
        }
        std::visit( OrderProcessor(), orderRequest );

    }
    return 0;
}
