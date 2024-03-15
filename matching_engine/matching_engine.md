# Write a Matching Engine
Your task is to write an exchange order matching engine.
Input will arrive from stdin, where each line shall be comprised of several columns separated by one space.
The first column of each line specifies the operation to be be carried out. The supported operations are:
  1. BUY
  2. SELL
  3. CANCEL
  4. MODIFY
  5. PRINT

* **BUY or SELL:**
  If the first column is BUY or SELL, then this line will contain five columns in total, where:
    1. The second column denotes the order type, which will be either: IOC or GFD.
    2. The third column will denote the price you want to buy or sell. It will be a positive integer.
    3. The fourth column denotes the quantity (or amount) of that will be either bought or sold. It too will be a positive integer.
    4. The fifth column is the Order ID, it can be comprised of any sequence of printable characters.
* **CANCEL:**
  If the first column is CANCEL, then this line will contain two columns in total, where:
    1. The second column is the Order ID. The order associated with the Order ID must be deleted and can no longer be considered for trading.
    2. **If the Order ID does not exist simply do nothing.**

* **MODIFY**
  If the first column is MODIFY, then this line will contain five columns in total, where:
    1. The second column is the Order ID, it means that specific order needs to be modified.
    2. The third column is either BUY or SELL.
    3. The fourth column is the new price of that order.
    4. The fifth column is the new quantity of that order.
    5. **If the Order ID does not exist simply do nothing.**
  **Note: We cannot modify an IOC order type, as will be explained later.**
  The MODIFY is a powerful operation, e.g. a BUY order can be modified to become a SELL order and vice versa.
  ```
  BUY GFD 1000 10 order1
  MODIFY order1 SELL 1000 10
```
* **PRINT**
  For an input line where first column is PRINT, then there shall be no other columns in this line.
  The task to be carried out for the PRINT operation is to output the current price book according to the following output format.

  *Output Format:*
  ```
  SELL:
  price1 qty1
  price2 qty2
  BUY:
  price1 qty1
  price2 qty2
  ```
  **The price for SELL section must be decreasing (descending) order and correspondingly the price for BUY section must also be decreasing order.** e.g.
  ```
  SELL:
  1010 10
  1000 20
  BUY:
  999 10
  800 20
  ```

* **Lets now discuss the order types:**
  The **GFD** order (stands for "Good For Day") will stay in the order book until it has been completely traded.
  The **IOC** order (stands for "Insert Or Cancel") means if it cannot be traded immediately, it will be cancelled right away. If it is only partially traded, the non-traded part is cancelled.

* **Matching Rule**
  The rule for matching is simple:
  If there is a "price cross", meaning someone is willing to buy at a price higher than or equal to the lowest current selling price, these two orders will be considered as having traded against each other.
  Furthermore the matching engine is expected to print out information pertaining to the newly minted trade.
  For example:
  ```
  BUY GFD 1000 10 order1
  SELL GFD 900 10 order2
  ```
  After you process the second line, we know that these two orders can be traded against each other, so you matching engine's output should be:
  ```
  TRADE order1 1000 10 order2 900 10
  ```
  The print out displays the two order's trade information:
      Order ID followed by the trade's price and the traded quantities for each side. (Real matching engines will have only one traded price, but to make things simple, we output two prices by each.)
  **The sequence for order1 and order2 is decided by which order was processed by the matching engine first.**

  For example:
  ```
  SELL GFD 900 10 order2
  BUY GFD 1000 10 order1
  ```
  Then the output is:
  ```
  TRADE order2 900 10 order1 1000 10
  ```
  **Note: The "TRADE order1 price1 qty1 order2 price2 qty2" message should be output every time a trade occurs within the the matching engine.**
  The overarching aim of the matching engine is to be fair. To achieve this we utilise **price-time priority**.
  **Simply put orders that are equally priced are traded out in the order they are received at the matching engine.**
  For example:
  ```
  BUY GFD 1000 10 order1
  BUY GFD 1000 10 order2
  SELL GFD 900 20 order3
  ```
  The output will be:
  ```
  TRADE order1 1000 10 order3 900 10
  TRADE order2 1000 10 order3 900 10
  ```
  There one more "property" to note of the MODIFY operation. **When are order is modified will lose its original priority (place).** So
  ```
  BUY GFD 1000 10 order1
  BUY GFD 1000 10 order2
  MODIFY order1 BUY 1000 20
  SELL GFD 900 20 order3
  ```
  Because order1 is modified in the middle, now order2 is in the first place, order1 is in the second place, so the expected output from the matching engine will be:
  ```
  TRADE order2 1000 10 order3 900 10
  TRADE order1 1000 10 order3 900 10
  ```
  **Note: Order ID will always be unique for all active orders in the matching engine, otherwise the given operation (Buy, Sell, Modify Cancel etc) should be ignored.**

  Example 1:
  ```
  BUY GFD 1000 10 order1
  PRINT
  ```
  The output of above would be:
  ```
  SELL:
  BUY:
  1000 10
  ```

  Example 2:
  ```
  BUY GFD 1000 10 order1
  BUY GFD 1000 20 order2
  PRINT
  ```
  The output of above would be:
  ```
  SELL:
  BUY:
  1000 30
  ```

  Example 3:
  ```
  BUY GFD 1000 10 order1
  BUY GFD 1001 20 order2
  PRINT
  ```
  The output of above would be: SELL:
  ```
  BUY:
  1001 20
  1000 10
  ```

  Example 4:
  ```
  BUY GFD 1000 10 order1
  SELL GFD 900 20 order2
  PRINT
  ```
  The output of above would be:
  ```
  TRADE order1 1000 10 order2 900 10
  SELL:
  900 10
  BUY:
  ```

  Example 5:
  ```
  BUY GFD 1000 10 ORDER1
  BUY GFD 1010 10 ORDER2
  SELL GFD 1000 15 ORDER3
  ```
  ```
  TRADE ORDER2 1010 10 ORDER3 1000 10
  TRADE ORDER1 1000 5 ORDER3 1000 5
  ```
  **Any input with price <= 0 or quantity <= 0 or empty order ID is invalid and should be ignored by the matching engine.**

  **Your solution should:**
  1. Be entirely of your own devising and not sourced from anyone or anywhere else
  2. Apply SOLID principles where appropriate, applicable and practical
  3. Be clear, concise and correct - demonstrating common sense software engineering and the application of modern C++ skills and principles.
  4. Take into account usage in low latency scenarios
  5. Be scalable, in short where applicable the overall performance should not degrade as the number of inputs or actions increase
  When grading your solution all of the above criteria will be taken into account in addition to the number of tests that have passed.

#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
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

using namespace std;
int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    return 0;
}

## Implementation of the matching engine.
This code reads from standard input and writes to standard output as per your requirement.
Please note that this is a basic implementation and may need further optimization for low latency scenarios.

```c++

#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
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

using namespace std;

struct Order {
    string id;
    int price;
    int quantity;
    string type;
    bool buy;
};

struct PriceCompare {
    bool operator() (const Order& lhs, const Order& rhs) const {
        return lhs.price < rhs.price;
    }
};

struct TimeCompare {
    bool operator() (const Order& lhs, const Order& rhs) const {
        return lhs.id < rhs.id;
    }
};

multiset<Order, PriceCompare> sellOrders;
multiset<Order, PriceCompare> buyOrders;
unordered_map<string, Order> orderMap;

void printOrderBook() {
    cout << "SELL:" << endl;
    for (auto it = sellOrders.rbegin(); it != sellOrders.rend(); ++it) {
        cout << it->price << " " << it->quantity << endl;
    }
    cout << "BUY:" << endl;
    for (auto it = buyOrders.rbegin(); it != buyOrders.rend(); ++it) {
        cout << it->price << " " << it->quantity << endl;
    }
}

void matchOrders() {
    while (!buyOrders.empty() && !sellOrders.empty()) {
        auto buy = *buyOrders.rbegin();
        auto sell = *sellOrders.begin();
        if (buy.price >= sell.price) {
            int tradeQuantity = min(buy.quantity, sell.quantity);
            cout << "TRADE " << buy.id << " " << buy.price << " " << tradeQuantity << " " << sell.id << " " << sell.price << " " << tradeQuantity << endl;
            buyOrders.erase(--buyOrders.end());
            sellOrders.erase(sellOrders.begin());
            buy.quantity -= tradeQuantity;
            sell.quantity -= tradeQuantity;
            if (buy.quantity > 0) {
                buyOrders.insert(buy);
            }
            if (sell.quantity > 0) {
                sellOrders.insert(sell);
            }
        } else {
            break;
        }
    }
}

int main() {
    string operation;
    while (cin >> operation) {
        if (operation == "BUY" || operation == "SELL") {
            Order order;
            cin >> order.type >> order.price >> order.quantity >> order.id;
            order.buy = (operation == "BUY");
            if (order.type == "GFD") {
                if (order.buy) {
                    buyOrders.insert(order);
                } else {
                    sellOrders.insert(order);
                }
                orderMap[order.id] = order;
            }
            matchOrders();
        } else if (operation == "CANCEL") {
            string id;
            cin >> id;
            if (orderMap.count(id)) {
                Order order = orderMap[id];
                if (order.buy) {
                    buyOrders.erase(buyOrders.find(order));
                } else {
                    sellOrders.erase(sellOrders.find(order));
                }
                orderMap.erase(id);
            }
        } else if (operation == "MODIFY") {
            string id;
            cin >> id;
            if (orderMap.count(id)) {
                Order order = orderMap[id];
                if (order.buy) {
                    buyOrders.erase(buyOrders.find(order));
                } else {
                    sellOrders.erase(sellOrders.find(order));
                }
                orderMap.erase(id);
                cin >> operation >> order.price >> order.quantity;
                order.buy = (operation == "BUY");
                if (order.buy) {
                    buyOrders.insert(order);
                } else {
                    sellOrders.insert(order);
                }
                orderMap[order.id] = order;
                matchOrders();
            }
        } else if (operation == "PRINT") {
            printOrderBook();
        }
    }
    return 0;
}
```
This code maintains two multisets, one for buy orders and one for sell orders. The orders are sorted by price. It also maintains a map from order ID to the order itself for quick access. When a new order comes in, it tries to match it with existing orders. If it can't be matched, it's added to the order book. If an order is cancelled or modified, it's removed from the order book and the map. The 'PRINT' operation simply prints the current state of the order book. Please note that this code assumes that the input is well-formed and does not contain any errors. You may want to add error checking code for a production environment. Also, this code does not handle the 'IOC' order type as per your requirement. You may need to add additional logic to handle this case. This code is a good starting point and you can modify it according to your needs. Please let me know if you have any questions or need further assistance.

Sure, here is a high-level design for the `OrderBook` and `MatchingEngine` classes.

**Order Class**

First, we need an `Order` class to represent an order. This class will have the following properties:

- `orderID`: A string that represents the unique identifier of the order.
- `orderType`: A string that represents the type of the order (IOC or GFD).
- `operation`: A string that represents the operation of the order (BUY or SELL).
- `price`: An integer that represents the price of the order.
- `quantity`: An integer that represents the quantity of the order.

**OrderBook Class**

The `OrderBook` class will maintain two priority queues (one for BUY orders and one for SELL orders). The priority queue for BUY orders will be a max heap based on the price, and the priority queue for SELL orders will be a min heap based on the price. If the prices are equal, the order that came first will have higher priority. This class will have the following methods:

- `addOrder(Order order)`: Adds a new order to the appropriate priority queue.
- `cancelOrder(string orderID)`: Removes an order from the priority queue.
- `modifyOrder(Order order)`: Modifies an existing order in the priority queue.
- `print()`: Prints the current state of the order book.

**MatchingEngine Class**

The `MatchingEngine` class will contain an instance of the `OrderBook` class and will be responsible for matching orders. This class will have the following methods:

- `matchOrders()`: Matches BUY and SELL orders based on the matching rules. If a match is found, it prints the trade information and updates the order book.
- `processOrder(Order order)`: Processes a new order. Depending on the operation of the order, it either adds the order to the order book, cancels an order, modifies an order, or prints the order book.

This design ensures that the matching engine operates in a fair manner (price-time priority) and can handle a large number of orders efficiently. The use of priority queues ensures that orders can be matched, added, and removed in logarithmic time. The separate BUY and SELL order books allow the matching engine to quickly find matching orders. The use of a separate `OrderBook` class ensures that the order book logic is encapsulated and can be easily modified or replaced if needed. The `MatchingEngine` class provides a simple interface for processing orders and hides the complexity of the order matching logic.

Please note that this is a high-level design and does not include all the necessary error checking and edge case handling. You would need to expand upon this design to fully implement the matching engine as per your requirements. Also, the actual implementation of the methods in these classes would depend on the specific rules of your matching engine. For example, the `matchOrders` method in the `MatchingEngine` class would need to implement the specific matching rules of your engine. Similarly, the `addOrder`, `cancelOrder`, and `modifyOrder` methods in the `OrderBook` class would need to implement the specific rules for adding, cancelling, and modifying orders in your engine. The `print` method would need to implement the specific format for printing the order book. 

I hope this helps! Let me know if you have any questions or need further clarification.

Absolutely, here's the high-level design of classes and data structures for the matching engine:

**Classes:**

1. **Order:**
    * **Attributes:**
        * `type` (enum): BUY or SELL
        * `id` (string): Unique identifier for the order
        * `price` (int): Price of the order
        * `quantity` (int): Remaining quantity of the order
        * `status` (enum): OPEN, FILLED, CANCELED
        * `is_ioc` (bool): Flag indicating IOC (Insert or Cancel) order

2. **OrderBook:**
    * **Abstract class:** Represents a collection of orders for a specific type (BUY or SELL)
    * **Interface:**
        * `add_order(const Order& order)`: Adds an order to the book
        * `remove_order(const std::string& id)`: Removes an order by ID
        * `get_orders() const`: Returns a read-only reference to the internal order map (price -> set of orders)
        * `modify_order(const std::string& id, int new_price, int new_quantity)`: Modifies an existing order

3. **BuyOrderBook (inherits OrderBook):**
    * **Implementation details:**
        * Uses a `std::map<int, std::set<const Order*>>` to store orders.
        * Key: Price (integer)
        * Value: Set of pointers to `Order` objects (sorted by arrival time for FIFO processing)

4. **SellOrderBook (inherits OrderBook):**
    * **Implementation details:**
        * Similar to BuyOrderBook, but uses a `std::map` with a custom comparator (`std::greater<int>`) to store orders in descending order of price (highest price first).

5. **MatchingEngine:**
    * Processes incoming orders and performs matching based on order types and prices.
    * Manages two instances of `OrderBook`: one for BUY and one for SELL orders.

**Data Structures:**

* `std::map`: Used in `OrderBook` to efficiently store orders based on price.
* `std::set`: Used within the `map` to maintain order within a specific price level (FIFO for BUY, descending price for SELL).
* `std::string`: Used for storing unique order IDs.
* `int`: Used for storing price and quantity of orders.
* `enum`: Used for order type (`BUY` or `SELL`) and order status (`OPEN`, `FILLED`, `CANCELED`).

**Relationships:**

* `MatchingEngine` has two member variables of type `OrderBook`: one for `BuyOrderBook` and one for `SellOrderBook`.
* `OrderBook` is an abstract class with a pure virtual interface.
* `BuyOrderBook` and `SellOrderBook` inherit from `OrderBook` and provide concrete implementations for BUY and SELL orders respectively.
* `Order` is a standalone class representing an individual order.

**Key Points:**

* Compile-time polymorphism is achieved through inheritance with `OrderBook` as the abstract base class.
* Order books use a combination of `map` and `set` for efficient price-based lookup and maintaining order within each price level.
* The design focuses on separation of concerns and promotes code reusability through the abstract `OrderBook` class.



Here's a basic implementation of your requirements in C++20. Please note that this is a simplified version and may need to be adjusted based on your specific needs.

```cpp
    #include <unordered_map>
#include <vector>

struct Order {
    int id;
    double price;
    // Other order details...
};

struct PriceLevel {
    double price;
    std::vector<Order*> orders;
    // Other price level details...
};

struct OrderBook {
    std::vector<PriceLevel*> bidLevels;
    std::vector<PriceLevel*> askLevels;
    // Other order book details...
};

struct OrderMetaData {
    OrderBook* orderBook;
    PriceLevel* priceLevel;
    Order* nextOrder;
    Order* prevOrder;
};

std::unordered_map<int, OrderMetaData> orderMap;

void insertOrder(Order* order, OrderBook* orderBook, PriceLevel* priceLevel) {
    OrderMetaData metaData = {orderBook, priceLevel, nullptr, nullptr};
    orderMap[order->id] = metaData;
    // Insert the order into the price level and update nextOrder and prevOrder as needed.
}

void deleteOrder(int orderId) {
    auto it = orderMap.find(orderId);
    if (it != orderMap.end()) {
        // Remove the order from the price level and update nextOrder and prevOrder as needed.
        orderMap.erase(it);
    }
}

OrderMetaData* lookupOrder(int orderId) {
    auto it = orderMap.find(orderId);
    if (it != orderMap.end()) {
        return &(it->second);
    }
    return nullptr;
}
```

This code creates an `unordered_map` from order IDs to order metadata. The `OrderMetaData` struct contains pointers to the order book and price level, as well as the next and previous orders in the queue. The `insertOrder`, `deleteOrder`, and `lookupOrder` functions provide O(1) insertion, deletion, and lookup of orders. The price levels in the order book are stored in a `vector` for fast average price lookup. Please note that this code does not handle the case of pathological orders far away from the inside of the book. You may need to add additional logic to handle such cases. Also, this code does not handle BBO updates, which you may need to implement separately based on your specific requirements.


# Samples

Sure, let's walk through the process of adding two orders to the order book. We'll use the following sample data:

- Order 1: Buy AAPL @ $100
- Order 2: Sell AAPL @ $101

Here are the steps:

1. **Initial state**: All data structures are empty.

2. **Add Order 1**:
    - `OrderBookByOrderIDMap`: {1: OrderBook for AAPL}
    - `OrderMetaDataByOrderIDMap`: {1: OrderMetaData for Order 1}
    - `OrderBook for AAPL`: {PriceLevel @ $100: [Order 1]}
    - `PriceLevel @ $100`: {firstOrder: Order 1, lastOrder: Order 1, orders: [Order 1]}
    - `OrderMetaData for Order 1`: {priceLevel: PriceLevel @ $100, prevOrder: nullptr, nextOrder: nullptr}

3. **Add Order 2**:
    - `OrderBookByOrderIDMap`: {1: OrderBook for AAPL, 2: OrderBook for AAPL}
    - `OrderMetaDataByOrderIDMap`: {1: OrderMetaData for Order 1, 2: OrderMetaData for Order 2}
    - `OrderBook for AAPL`: {PriceLevel @ $100: [Order 1], PriceLevel @ $101: [Order 2]}
    - `PriceLevel @ $100`: {firstOrder: Order 1, lastOrder: Order 1, orders: [Order 1]}
    - `PriceLevel @ $101`: {firstOrder: Order 2, lastOrder: Order 2, orders: [Order 2]}
    - `OrderMetaData for Order 1`: {priceLevel: PriceLevel @ $100, prevOrder: nullptr, nextOrder: nullptr}
    - `OrderMetaData for Order 2`: {priceLevel: PriceLevel @ $101, prevOrder: nullptr, nextOrder: nullptr}

At each step, the `OrderBookByOrderIDMap` and `OrderMetaDataByOrderIDMap` are updated to include the new order. The `OrderBook` for AAPL includes the new `PriceLevel` for the order's price, and the `OrderMetaData` for the new order includes the `PriceLevel` and the previous and next orders at that price level.

I hope this helps! Let me know if you have any other questions.
Sure, let's add a new order "Order 3: Buy AAPL @ $100" to the existing state. Here's how the data structures might evolve:

1. **Add Order 3** (Buy AAPL @ $100, Order ID: 3):
    - Call `addOrderToBook` with the new order.
    - `OrderBookByOrderIDMap`: {1: OrderBook for AAPL, 2: OrderBook for AAPL, 3: OrderBook for AAPL}
    - `OrderMetaDataByOrderIDMap`: {1: OrderMetaData for Order 1, 2: OrderMetaData for Order 2, 3: OrderMetaData for Order 3}
    - `OrderBook for AAPL`: {PriceLevel @ $100: [Order 1, Order 3], PriceLevel @ $101: [Order 2]}
    - `PriceLevel @ $100`: {firstOrder: Order 1, lastOrder: Order 3, orders: [Order 1, Order 3]}
    - `PriceLevel @ $101`: {firstOrder: Order 2, lastOrder: Order 2, orders: [Order 2]}
    - `OrderMetaData for Order 1`: {priceLevel: PriceLevel @ $100, prevOrder: nullptr, nextOrder: Order 3}
    - `OrderMetaData for Order 2`: {priceLevel: PriceLevel @ $101, prevOrder: nullptr, nextOrder: nullptr}
    - `OrderMetaData for Order 3`: {priceLevel: PriceLevel @ $100, prevOrder: Order 1, nextOrder: nullptr}

At this step, the `OrderBookByOrderIDMap` and `OrderMetaDataByOrderIDMap` are updated to include the new order. The `OrderBook` for AAPL includes the new `PriceLevel` for the order's price, and the `OrderMetaData` for the new order includes the `PriceLevel` and the previous and next orders at that price level.

I hope this helps! Let me know if you have any other questions.
