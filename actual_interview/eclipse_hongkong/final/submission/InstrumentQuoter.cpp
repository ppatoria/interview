#include <algorithm>
#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

// Provides methods to send order requests to the exchange.
// ASSUME THIS CLASS IS ALREADY DEFINED. DO NOT IMPLEMENT.
class Execution {
public:
  void requestOrderAdd(            // Requests an order to be added at the exchange
      uint32_t id,                 // A caller-selected request identifier for this order
      std::string const &feedcode, // The exchange identifier of the instrument
      char orderSide,              // The side of the order ('B' for buy,'S' for sell)
      double orderPrice,           // The price of the order
      uint32_t orderVolume);        // The volume of the order

  void requestOrderRemove(        // Requests an order to be removed at the exchange
      uint32_t id);               // The request identifier used in 'requestOrderAdd'
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

