#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>

class Execution {
public:
  void requestOrderAdd(uint32_t id, const std::string &feedcode, char orderSide,
                       double orderPrice, uint32_t orderVolume) {
    std::cout << "OrderAdd: ID=" << id << ", Feedcode=" << feedcode
              << ", Side=" << orderSide << ", Price=" << orderPrice
              << ", Volume=" << orderVolume << "\n";
  }

  void requestOrderRemove(uint32_t id) {
    std::cout << "OrderRemove: ID=" << id << "\n";
  }
};

class InstrumentQuoter {
public:
  InstrumentQuoter(const std::string &feedcode, double quoteOffset,
                   uint32_t quoteVolume, double tickWidth, Execution &execution)
      : feedcode(feedcode), quoteOffset(quoteOffset), quoteVolume(quoteVolume),
        tickWidth(tickWidth), execution(execution), nextOrderId(1),
        bidOrderActive(false), askOrderActive(false) {}

  void OnTheoreticalPrice(double theoreticalPrice) {
    this->theoreticalPrice = theoreticalPrice;
    updateQuotes();
  }

  void OnBestBidOffer(double bidPrice, double offerPrice) {
    this->bidPrice = bidPrice;
    this->offerPrice = offerPrice;
    updateQuotes();
  }

  void OnOrderAddConfirm(uint32_t id) {
    activeOrders.insert(id);
    processPendingRemovals();
  }

  void OnOrderRemoveConfirm(uint32_t id) {
    activeOrders.erase(id);
    processPendingRemovals();
  }

private:
  std::string feedcode;
  double quoteOffset;
  uint32_t quoteVolume;
  double tickWidth;
  Execution &execution;

  double theoreticalPrice = 0.0;
  double bidPrice = 0.0;
  double offerPrice = 0.0;

  uint32_t nextOrderId;
  std::unordered_set<uint32_t> activeOrders;
  std::queue<uint32_t> pendingRemovals;

  bool bidOrderActive;
  bool askOrderActive;
  uint32_t bidOrderId;
  uint32_t askOrderId;

  void updateQuotes() {
    // Calculate the new bid and ask prices
    double newBidPrice = alignToTick(theoreticalPrice - quoteOffset);
    double newAskPrice = alignToTick(theoreticalPrice + quoteOffset);

    if (bidPrice > 0.0) {
      newBidPrice = std::min(newBidPrice, bidPrice - tickWidth);
    }
    if (offerPrice > 0.0) {
      newAskPrice = std::max(newAskPrice, offerPrice + tickWidth);
    }

    // Handle bid order
    if (bidOrderActive) {
      pendingRemovals.push(bidOrderId);
      bidOrderActive = false;
    }
    bidOrderId = nextOrderId++;
    execution.requestOrderAdd(bidOrderId, feedcode, 'B', newBidPrice,
                              quoteVolume);
    bidOrderActive = true;

    // Handle ask order
    if (askOrderActive) {
      pendingRemovals.push(askOrderId);
      askOrderActive = false;
    }
    askOrderId = nextOrderId++;
    execution.requestOrderAdd(askOrderId, feedcode, 'S', newAskPrice,
                              quoteVolume);
    askOrderActive = true;
  }

  double alignToTick(double price) {
    return std::round(price / tickWidth) * tickWidth;
  }

  void processPendingRemovals() {
    while (!pendingRemovals.empty()) {
      uint32_t id = pendingRemovals.front();
      if (activeOrders.find(id) != activeOrders.end())
        break;

      pendingRemovals.pop();
      execution.requestOrderRemove(id);
    }
  }
};

int main() {
  Execution execution;
  InstrumentQuoter quoter("INSTRUMENT1", 0.5, 100, 0.5, execution);

  std::cout << "Scenario 1: Initialize and update with theoretical price.\n";
  quoter.OnTheoreticalPrice(10.0);

  std::cout << "\nScenario 2: Update with new best bid/offer.\n";
  quoter.OnBestBidOffer(9.5, 10.5);

  std::cout << "\nScenario 3: Confirm orders added.\n";
  quoter.OnOrderAddConfirm(1);
  quoter.OnOrderAddConfirm(2);

  std::cout << "\nScenario 4: Confirm orders removed and update quotes.\n";
  quoter.OnOrderRemoveConfirm(1);
  quoter.OnOrderRemoveConfirm(2);
  quoter.OnTheoreticalPrice(11.0);

  return 0;
}
