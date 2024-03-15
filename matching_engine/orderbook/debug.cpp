#include "algorithm"
#include "orderbook.h"
#include <ostream>
namespace OrderBook {

template <typename T>
concept Container = requires(T t) {
    { t.begin() } -> std::same_as<typename T::iterator>;
    { t.end() } -> std::same_as<typename T::iterator>;
    requires !std::same_as<typename T::value_type, char>;
};

template <Container T>
std::ostream& operator<<(std::ostream& os, const T& container)
{
    os << "[";
    for_each(container.begin(), container.end(), [&os](const auto& element) {
        os << element << ", ";
    });

    if (!container.empty()) {
        os.seekp(-2, std::ios_base::end);
        os << "]";
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const PriceLevel& priceLevel)
{
    os << "price: " << priceLevel.price
       << "firstOrder: ";
    if (priceLevel.firstOrder) {
        os << *(priceLevel.firstOrder); // Dereference the shared_ptr before printing
    }
    os << "lastOrder: ";
    if (priceLevel.lastOrder) {
        os << *(priceLevel.lastOrder); // Dereference the shared_ptr before printing
    }
    os << "Orders: ";
    for (const auto& order : priceLevel.orders) {
        if (order) {
            os << *order << ", "; // Dereference the shared_ptr before printing
        }
    }
    os << endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const OrderBook& orderBook)
{
    os << "Symbol: " << orderBook.symbol
       << "\nbidLevels: " << orderBook.bidLevels
       << "\naskLevels: " << orderBook.askLevels
       << endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const OrderMetaData& orderMetaData)
{
    os << "PriceLevel: " << orderMetaData.priceLevel
       << "nextOrder: " << orderMetaData.nextOrder
       << "prevOrder: " << orderMetaData.prevOrder
       << endl;
    return os;
}

} // namespace OrderBook
