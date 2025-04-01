/**
This test consists of 1 problem.

Problem 1.
In C++, implement a quoting algorithm for a single instrument for a simple
market that only supports order insertion and deletion. You are welcome to use
any STL or Boost functionality, but please do not use other third-party
libraries. Your code should implement the InstrumentQuoter class.

Requirements:
• Orders must not be closer to the theoretical price than the offset at the time
they're sent (e.g., theoreticalPrice - orderPrice >= quoteOffset for buy
orders).

• Orders must be sent with price aligned to the exchange tick (e.g., for
tickWidth = 0.5, valid order prices would be ..., 9.5, 10.0,10.5, ...).

• Orders must not cross the exchange best bid/offer at the time they're sent
(e.g., for tickWidth = 0.5, theoreticalPrice = 10.0, quoteOffset = 0.5, and
offerPrice = 9.0, the buy order should have order price 8.5 rather than 9.5).

• Previous orders on a side must be confirmed removed before a new add request
is sent on the same side.

Assumptions:
• You can assume all code wiring is done for you (i.e., you do not need to worry
about how the InstrumentQuoter::On* methods are called). • You can assume that
all InstrumentQuoter methods are called from the same thread (i.e., you do not
need to worry about concurrency and synchronisation). • You can assume order
requests will eventually succeed (i.e., you do not need to handle exchange
rejections). • You can ignore the effect of trades (i.e., you do not need to
"refill" quotes). • You can ignore exchange volume rules (i.e., the quote volume
provided to you accounts for any exchange volume rules).

If you prefer, you may — but are not required to — assume that negative or zero
order prices are valid for this instrument.

// Represents the quoting algorithm for a single instrument.
// THIS IS THE CLASS YOU NEED TO IMPLEMENT.
class InstrumentQuoter
{
public:
  InstrumentQuoter(            // Instantiates the quoter for a single instrument
  std::string const &feedcode, // The exchange identifier of the instrument
  double quoteOffset,          // The minimum desired distance between our theoretical price and each order we send
  uint32_t quoteVolume,        // The desired volume for each order we send
  double tickWidth,            // The distance between valid exchange price levels
  Execution &execution);       // An execution service to add and remove orders (interface described below)

  void OnTheoreticalPrice(     // Notifies the quoter of a new theoretical price for the instrument
  double theoreticalPrice);    // The new theoretical price

  void OnBestBidOffer(         // Notifies the quoter of a new best bid/offer from the exchange
  double bidPrice,             // The best price bid on the exchange
  double offerPrice);          // The best price offer on the exchange

  void OnOrderAddConfirm(      // Notifies the quoter that an "order add" request has been confirmed
  uint32_t id);                // The request identifier of the order

  void OnOrderRemoveConfirm(   // Notifies the quoter that an "order remove" request has been confirmed
  uint32_t id);                // The request identifier of the order
};


// Provides methods to send order requests to the exchange.
// ASSUME THIS CLASS IS ALREADY DEFINED. DO NOT IMPLEMENT.
class Execution
{
public:
  void requestOrderAdd(              // Requests an order to be added at the
       exchange uint32_t id,         // A caller-selected request identifier for this order
       std::string const &feedcode,  // The exchange identifier of the instrument
       char orderSide,               // The side of the order ('B' for buy, 'S' for sell)
       double orderPrice,            // The price of the order
       uint32_t orderVolume);        // The volume of the order

  void requestOrderRemove(           // Requests an order to be removed at the
      exchange uint32_t id);         // The request identifier used in 'requestOrderAdd'
};

*/
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

// Provides methods to send order requests to the exchange.
// ASSUME THIS CLASS IS ALREADY DEFINED. DO NOT IMPLEMENT.
class Execution {
public:
  void requestOrderAdd(            // Requests an order to be added at the exchange
      uint32_t id,                 // A caller-selected request identifier for this order
      std::string const &feedcode, // The exchange identifier of the instrument
      char orderSide,              // The side of the order ('B' for buy,'S' for sell)
      double orderPrice,           // The price of the order
      uint32_t orderVolume)        // The volume of the order
  {
    std::cout << "[Execution] ADD Order ID=" << id << " " << feedcode << " "
              << orderSide << " " << orderPrice  << " " << orderVolume << "\n";
  }
  void requestOrderRemove(        // Requests an order to be removed at the exchange
      uint32_t id)                // The request identifier used in 'requestOrderAdd'
  {
    std::cout << "[Execution] REMOVE Order ID=" << id << "\n";
  }
};

class InstrumentQuoter {
private:
  struct Order {
    char side;
    double price;
    uint32_t volume;
    bool isPending;

    Order(char s, double p, uint32_t v, bool pending = false)
        : side(s), price(p), volume(v), isPending(pending) {}

    std::string status() const { return isPending ? "pending" : "live"; }
  };

  const std::string feedcode_;
  const double quoteOffset_;
  const uint32_t quoteVolume_;
  const double tickWidth_;
  Execution &execution_;

  double currentTheoreticalPrice_;
  double exchangeBid_;
  double exchangeAsk_;
  uint32_t nextOrderId_;

  std::unordered_map<uint32_t, Order> orders_;

  bool validOffset(const Order &order) const {
    return (order.side == 'B')
               ? (currentTheoreticalPrice_ - order.price >= quoteOffset_)
               : (order.price - currentTheoreticalPrice_ >= quoteOffset_);
  }

  bool notCrossing(const Order &order) const {
    return (order.side == 'B')
               ? (exchangeAsk_ <= 0 || order.price < exchangeAsk_)
               : (exchangeBid_ <= 0 || order.price > exchangeBid_);
  }

  std::vector<uint32_t> identifyInvalidOrders() const {
    std::vector<uint32_t> invalidOrders;
    for (const auto &[id, order] : orders_) {
      if (!order.isPending
          &&
          (!validOffset(order) || !notCrossing(order)))
      {
          invalidOrders.push_back(id);
      }
    }
    return invalidOrders;
  }


  void cancelOrders(const std::vector<uint32_t> &orderIds) {
    for (uint32_t id : orderIds) {
      orders_.erase(id); // Immediate removal
      execution_.requestOrderRemove(id);
    }
  }

  double calculatePrice(char side) const {
    const double theoreticalPrice =
        (side == 'B') ? roundToTick(currentTheoreticalPrice_ - quoteOffset_)
                      : roundToTick(currentTheoreticalPrice_ + quoteOffset_);

    if (side == 'B') {
      return (exchangeAsk_ > 0)
                 ? std::min(theoreticalPrice, (exchangeAsk_ - tickWidth_))
                 : theoreticalPrice;
    } else {
      return (exchangeBid_ > 0)
                 ? std::max(theoreticalPrice, (exchangeBid_ + tickWidth_))
                 : theoreticalPrice;
    }
  }
  bool hasAnyQuote(char side, bool checkPending) const{
    return std::any_of(
        orders_.begin(), orders_.end(),
        [side, checkPending](const auto& quoteEntry){
          return (quoteEntry.second.side == side
                  &&
                  quoteEntry.second.isPending == checkPending);
        });
  }

  bool hasPendingQuote(char side) const {
    return hasAnyQuote(side, true /*is pending */);
  }

  bool hasLiveQuote(char side) const {
    return hasAnyQuote(side, false /*is live */);
  }

  /**
   * Requirement:
   * Previous orders on a side must be confirmed removed
   * before a new add request is sent on the same side.
   */

  bool shouldSendQuote(char side) const {
    return !hasLiveQuote(side) && !hasPendingQuote(side);
  }

  void sendQuote(char side) {
    double price = calculatePrice(side);
    if (price > 0) {
      uint32_t id = nextOrderId_++;
      orders_.emplace(id, Order(side, price, quoteVolume_, true));
      execution_.requestOrderAdd(id, feedcode_, side, price, quoteVolume_);
    }
  }

  void processMarketUpdate() {

    std::vector<uint32_t> toCancel = identifyInvalidOrders();
    cancelOrders(toCancel);

    if (currentTheoreticalPrice_ > 0) {
      if (shouldSendQuote('B'))
        sendQuote('B');
      if (shouldSendQuote('S'))
        sendQuote('S');
    }
  }

  double roundToTick(double price) const {
    return std::round(price / tickWidth_) * tickWidth_;
  }
public:

  InstrumentQuoter(const std::string &feedcode, double quoteOffset,
                   uint32_t quoteVolume, double tickWidth, Execution &execution)
      : feedcode_(feedcode), quoteOffset_(quoteOffset),
        quoteVolume_(quoteVolume), tickWidth_(tickWidth), execution_(execution),
        currentTheoreticalPrice_(0.0), exchangeBid_(0.0), exchangeAsk_(0.0),
        nextOrderId_(1) {
    if (tickWidth <= 0.0)
      throw std::invalid_argument("Tick width must be positive");
    if (quoteOffset <= 0.0)
      throw std::invalid_argument("Quote offset must be positive");
  }

  void OnTheoreticalPrice(double theoreticalPrice) {
    currentTheoreticalPrice_ = theoreticalPrice;
    processMarketUpdate();
  }

  void OnBestBidOffer(double bidPrice, double offerPrice) {
    exchangeBid_ = bidPrice;
    exchangeAsk_ = offerPrice;
    processMarketUpdate();
  }

  void OnOrderAddConfirm(uint32_t id) {
    auto it = orders_.find(id);
    if (it != orders_.end()) {
      it->second.isPending = false;
    }
  }

  void OnOrderRemoveConfirm(uint32_t id) {
    // Safe to call - does nothing if already erased during cancelOrders()
    orders_.erase(id);
  }
};


int main() {
    Execution execution;
    
    InstrumentQuoter quoter("AAPL", 0.5, 100, 0.1, execution);
    std::cout << "[Main] Instrument: AAPL | Offset: 0.5 | Volume: 100 | Tick: 0.1\n";

    std::cout << "\n[Main] Set Theoretical Price = 100.0 (Expectation: B@99.5, S@100.5)\n";
    quoter.OnTheoreticalPrice(100.0);

    std::cout << "\n[Main] Set BBO=99.5/100.5 (Expectation: No crossing and shouldn't affect existing quotes)\n";
    quoter.OnBestBidOffer(99.5, 100.5);

    quoter.OnOrderAddConfirm(1);
    quoter.OnOrderAddConfirm(2);

    std::cout << "\n[Main] Set Theoretical=101.0 (Expection: should cancel sell@100.5 and add new sell@101.5) while Buy@99.5 remains (101.0-99.5=1.5 >= 0.5 offset)\n";
    quoter.OnTheoreticalPrice(101.0);

    quoter.OnOrderAddConfirm(3); // add confirmation for s@101.5 quote

    std::cout << "\n[Main] Set BBO=101.5/101.8 ( Expectation: Buy@99.5 still valid, Sell@101.5 crosses bid@101.5 and must be cancelled)\n";
    quoter.OnBestBidOffer(101.5, 101.8); 

    quoter.OnOrderRemoveConfirm(1);
    quoter.OnOrderRemoveConfirm(2);
    quoter.OnOrderRemoveConfirm(3);
    quoter.OnOrderRemoveConfirm(4);

    std::cout << "\n[Main] Set Theoretical=100 (Expectation: Fresh quotes)\n";
    quoter.OnTheoreticalPrice(100);

    quoter.OnOrderAddConfirm(5); 
    quoter.OnOrderAddConfirm(6); 

    std::cout << "\n[Main] Set BBO=99.2/99.3 ; existing echange bid will cross the new exchange ask; triggers removal and new quote B@99.2 sent\n";
    quoter.OnBestBidOffer(99.2, 99.3); 


    return 0;
}
