#include <unordered_map>
#include <stdexcept>

enum Side { Buy, Sell, ShortSell };
struct Order {
    int orderid;
    double price;
    int quantity;
    Side side;
};

struct NewOrder : public Order {};
struct CancelOrder : public Order {};
struct ModifyOrder : public Order {};

struct Quote {
    double price;
    int quantity;

    bool operator<(const Quote& other) const{
        return price < other.price;
    };

    bool operator==(const Quote& other) const{
        return price == other.price;
    }
};

using BuySide = std::unordered_map<double,Quote>;
using SellSide = std::unordered_map<double,Quote>;

class Orderbook {
   private:
    BuySide  buyside;
    SellSide sellside;

   public:
    template <typename CONTAINER>
    void add(const Order& order, CONTAINER& container) {
        Quote newQuote = {order.price, order.quantity};
        auto iter = container.find(newQuote);
        if (iter != container.end()) {
             
            newQuote.quantity += order.quantity;
            container.erase(iter);
        } 
        container.insert(newQuote);
        
    }

    template <typename CONTAINER>
    void remove(const Order& order, CONTAINER& container) {
        Quote cancelledQuote{order.price, order.quantity};
        auto iter = container.find(cancelledQuote);
        if (iter != container.end()) {
            if (iter->quantity > order.quantity){
                cancelledQuote.quantity  -=  order.quantity;
                container.erase(iter);
                container.insert(cancelledQuote);               
            }
            else if (iter->quantity == order.quantity){
                container.erase(iter);
            }else{
                throw std::runtime_error("Order quantity greater than the existing order");
            }
        } 
        else {
            throw std::runtime_error("Order does not exists");
        }
    }

    template <typename CONTAINER>
    void modify(const Order& order, CONTAINER& container) {
        Quote modifiedQuote{order.price, order.quantity};
        auto iter = container.find(modifiedQuote);
        if (iter != container.end()) {
            modifiedQuote.quantity += order.quantity;
            container.erase(iter);
            container.insert(modifiedQuote);
        } else {
            throw std::runtime_error("Order does not exists");
        }
    }

    void add(const NewOrder& order) {
        if (order.side == Side::Buy)
            add(order, buyside);
        else if (order.side == Side::Sell || order.side == Side::ShortSell)
            add(order, sellside);
        else
            throw std::runtime_error("Invalid Side in the New Order");
    }

    void remove(const CancelOrder& order) {
        if (order.side == Side::Buy)
            remove(order, buyside);
        else if (order.side == Side::Sell || order.side == Side::ShortSell)
            remove(order, sellside);
        else
            throw std::runtime_error("Invalid Side in the New Order");
    }

    void modify(const ModifyOrder& order) {
        if (order.side == Side::Buy)
            modify(order, buyside);
        else if (order.side == Side::Sell || order.side == Side::ShortSell)
            modify(order, sellside);
        else
            throw std::runtime_error("Invalid Side in the New Order");
    }
};
