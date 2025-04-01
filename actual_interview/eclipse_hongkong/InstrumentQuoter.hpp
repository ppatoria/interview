#ifndef INSTRUMENT_QUOTER_HPP
#define INSTRUMENT_QUOTER_HPP

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

class Execution {
public:
    virtual void requestOrderAdd(uint32_t id, const std::string& feedcode,
                               char side, double price, uint32_t volume) = 0;
    virtual void requestOrderRemove(uint32_t id) = 0;
    virtual ~Execution() = default;
};

class InstrumentQuoter {
 public:
    struct Quote {
        char side;
        double price;
        uint32_t volume;
        bool isPending;

        Quote(char s, double p, uint32_t v, bool pending = false)
            : side(s), price(p), volume(v), isPending(pending) {}
    };

    friend class InstrumentQuoterTestAccess;

    const std::string feedcode_;
    const double quoteOffset_;
    const uint32_t quoteVolume_;
    const double tickWidth_;
    Execution& execution_;

    double currentTheoreticalPrice_ = 0.0;
    double exchangeBid_ = 0.0;
    double exchangeAsk_ = 0.0;
    uint32_t nextQuoteId_ = 1;
    std::unordered_map<uint32_t, Quote> quotes_;

    double roundToTick(double price) const {
        if (tickWidth_ <= 0.0) return price;
        return std::round(price / tickWidth_) * tickWidth_;
    }

    bool validOffset(const Quote& order) const {
        if (order.side == 'B') {
            return (currentTheoreticalPrice_ - order.price) >= (quoteOffset_ - 1e-9);
        } else {
            return (order.price - currentTheoreticalPrice_) >= (quoteOffset_ - 1e-9);
        }
    }

    bool notCrossing(const Quote& order) const {
        if (order.side == 'B') {
            return (exchangeAsk_ <= 0) || (order.price < (exchangeAsk_ - 1e-9));
        } else {
            return (exchangeBid_ <= 0) || (order.price > (exchangeBid_ + 1e-9));
        }
    }

    std::vector<uint32_t> identifyInvalidOrdersotes() const {
        std::vector<uint32_t> invalidOrders;
        for (const auto& [id, order] : quotes_) {
            if (!order.isPending && (!validOffset(order) || !notCrossing(order))) {
                invalidOrders.push_back(id);
            }
        }
        return invalidOrders;
    }

    void cancelQuotes(const std::vector<uint32_t>& orderIds) {
        for (uint32_t id : orderIds) {
            quotes_.erase(id);
            execution_.requestOrderRemove(id);
        }
    }

    double calculatePrice(char side) const {
        double price = (side == 'B')
            ? roundToTick(currentTheoreticalPrice_ - quoteOffset_)
            : roundToTick(currentTheoreticalPrice_ + quoteOffset_);

        if (side == 'B' && exchangeBid_ > 0) {
            price = std::min(price, roundToTick(exchangeBid_ - tickWidth_));
        } else if (side == 'S' && exchangeAsk_ > 0) {
            price = std::max(price, roundToTick(exchangeAsk_ + tickWidth_));
        }

        return price;
    }

    bool hasAnyQuote(char side, bool checkPending) const {
        return std::any_of(quotes_.begin(), quotes_.end(),
            [side, checkPending](const auto& entry) {
                return entry.second.side == side && 
                       entry.second.isPending == checkPending;
            });
    }

    bool shouldSendQuote(char side) const {
        return !hasAnyQuote(side, false) && !hasAnyQuote(side, true);
    }

    void sendQuote(char side) {
        double price = calculatePrice(side);
        uint32_t id = nextQuoteId_++;
        quotes_.emplace(id, Quote(side, price, quoteVolume_, true));
        execution_.requestOrderAdd(id, feedcode_, side, price, quoteVolume_);
    }

    void processMarketUpdate() {
        std::vector<uint32_t> toCancel = identifyInvalidOrdersotes();
        cancelQuotes(toCancel);

        if (currentTheoreticalPrice_ != 0.0) {
            if (shouldSendQuote('B')) sendQuote('B');
            if (shouldSendQuote('S')) sendQuote('S');
        }
    }

public:
    InstrumentQuoter(const std::string& feedcode, double quoteOffset,
                   uint32_t quoteVolume, double tickWidth, Execution& execution)
        : feedcode_(feedcode), quoteOffset_(quoteOffset),
          quoteVolume_(quoteVolume), tickWidth_(tickWidth), execution_(execution) {
        if (tickWidth <= 0.0) throw std::invalid_argument("Tick width must be positive");
        if (quoteOffset <= 0.0) throw std::invalid_argument("Quote offset must be positive");
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
        if (auto it = quotes_.find(id); it != quotes_.end()) {
            it->second.isPending = false;
        }
    }

    void OnOrderRemoveConfirm(uint32_t id) {
        quotes_.erase(id);
    }
};

class InstrumentQuoterTestAccess {
public:
    static double calculatePrice(const InstrumentQuoter& quoter, char side) {
        return quoter.calculatePrice(side);
    }
    
    static bool shouldSendQuote(const InstrumentQuoter& quoter, char side) {
        return quoter.shouldSendQuote(side);
    }
    
    static bool validOffset(const InstrumentQuoter& quoter, const InstrumentQuoter::Quote& order) {
        return quoter.validOffset(order);
    }
    
    static bool notCrossing(const InstrumentQuoter& quoter, const InstrumentQuoter::Quote& order) {
        return quoter.notCrossing(order);
    }
};

#endif // INSTRUMENT_QUOTER_HPP
