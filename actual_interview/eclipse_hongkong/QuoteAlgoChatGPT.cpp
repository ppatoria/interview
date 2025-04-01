#include <cstdint>
#include <iostream>
#include <optional>
#include <unordered_map>
#include <cmath>
#include <string>

class Execution {
public:
    void requestOrderAdd(uint32_t id, const std::string& feedcode, char orderSide, double orderPrice, uint32_t orderVolume) {
        std::cout << "Order Added: ID=" << id << " Side=" << orderSide << " Price=" << orderPrice << " Volume=" << orderVolume << "\n";
    }

    void requestOrderRemove(uint32_t id) {
        std::cout << "Order Removed: ID=" << id << "\n";
    }
};

class InstrumentQuoter {
private:
    std::string feedcode;
    double quoteOffset;
    uint32_t quoteVolume;
    double tickWidth;
    Execution& execution;

    std::optional<uint32_t> currentBuyOrder;
    std::optional<uint32_t> currentSellOrder;
    std::unordered_map<uint32_t, double> orderPrices;
    double theoreticalPrice = 0.0;
    double bestBid = 0.0;
    double bestOffer = 0.0;
    uint32_t orderIdCounter = 0;

    uint32_t getNextOrderId() {
        return ++orderIdCounter;
    }

    double roundToTick(double price) {
        return std::round(price / tickWidth) * tickWidth;
    }

    bool isValidPrice(double price) {
        return price > 0; // Assuming negative or zero prices are invalid.
    }

    void updateQuotes() {
        double buyPrice = roundToTick(theoreticalPrice - quoteOffset);
        double sellPrice = roundToTick(theoreticalPrice + quoteOffset);

        // Ensure quotes do not cross the best bid/offer.
        if (buyPrice >= bestOffer) buyPrice = roundToTick(bestOffer - tickWidth);
        if (sellPrice <= bestBid) sellPrice = roundToTick(bestBid + tickWidth);

        if (isValidPrice(buyPrice)) handleBuyOrder(buyPrice);
        if (isValidPrice(sellPrice)) handleSellOrder(sellPrice);
    }

    void handleBuyOrder(double buyPrice) {
        if (currentBuyOrder && orderPrices[*currentBuyOrder] == buyPrice) {
            return; // Order is already at correct price, no need to update.
        }
        
        if (currentBuyOrder) {
            execution.requestOrderRemove(*currentBuyOrder);
            currentBuyOrder.reset();
        }
        
        uint32_t newBuyOrderId = getNextOrderId();
        execution.requestOrderAdd(newBuyOrderId, feedcode, 'B', buyPrice, quoteVolume);
        currentBuyOrder = newBuyOrderId;
        orderPrices[newBuyOrderId] = buyPrice;
    }

    void handleSellOrder(double sellPrice) {
        if (currentSellOrder && orderPrices[*currentSellOrder] == sellPrice) {
            return; // Order is already at correct price, no need to update.
        }
        
        if (currentSellOrder) {
            execution.requestOrderRemove(*currentSellOrder);
            currentSellOrder.reset();
        }
        
        uint32_t newSellOrderId = getNextOrderId();
        execution.requestOrderAdd(newSellOrderId, feedcode, 'S', sellPrice, quoteVolume);
        currentSellOrder = newSellOrderId;
        orderPrices[newSellOrderId] = sellPrice;
    }

public:
    InstrumentQuoter(std::string const& feedcode, double quoteOffset, uint32_t quoteVolume, double tickWidth, Execution& execution)
        : feedcode(feedcode), quoteOffset(quoteOffset), quoteVolume(quoteVolume), tickWidth(tickWidth), execution(execution) {}

    void OnTheoreticalPrice(double newTheoreticalPrice) {
        theoreticalPrice = newTheoreticalPrice;
        updateQuotes();
    }

    void OnBestBidOffer(double bidPrice, double offerPrice) {
        bestBid = bidPrice;
        bestOffer = offerPrice;
        updateQuotes();
    }

    void OnOrderAddConfirm(uint32_t id) {
        // No action needed in this case, as orders are tracked during placement.
    }

    void OnOrderRemoveConfirm(uint32_t id) {
        if (currentBuyOrder && *currentBuyOrder == id) currentBuyOrder.reset();
        if (currentSellOrder && *currentSellOrder == id) currentSellOrder.reset();
        orderPrices.erase(id);
    }
};

int main() {
    Execution execution;
    InstrumentQuoter quoter("AAPL", 0.5, 100, 0.5, execution);

    // Simulate incoming market updates
    quoter.OnTheoreticalPrice(10.0);
    quoter.OnBestBidOffer(9.5, 10.5);
    quoter.OnOrderAddConfirm(1);
    quoter.OnOrderAddConfirm(2);

    // Changing market conditions
    quoter.OnTheoreticalPrice(10.5);
    quoter.OnBestBidOffer(10.0, 11.0);

    // Order confirmations
    quoter.OnOrderRemoveConfirm(1);
    quoter.OnOrderRemoveConfirm(2);

    return 0;
}
