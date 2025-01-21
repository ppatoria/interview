#include <array>
#include <benchmark/benchmark.h>
#include <concepts>
#include <random>
#include <span> // Requires C++20

struct MarketDataArranged {
  double price;
  int symbol_id;
  int volume;

  MarketDataArranged() = default;

  MarketDataArranged(double price_, int symbolID, int vol)
      : price(price_), symbol_id(symbolID), volume(vol) {}
};

template <typename T, std::size_t N> struct AlignedArray {
  alignas(64) std::array<T, N> data;
  using value_type = T;
  T &operator[](size_t index) { return data[index]; }
  const T &operator[](size_t index) const { return data[index]; }
};


template <typename T, size_t N> class SimulatedAlignedArrayGenerator {
public:
  explicit SimulatedAlignedArrayGenerator() { populateData(); }

  const AlignedArray<T, N> &get() const { return data_; }

private:
  AlignedArray<T, N> data_;

  void populateData() {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> symbolDist(1, 1000);
    std::uniform_real_distribution<double> priceDist(1.0, 1000.0);
    std::uniform_int_distribution<int> volumeDist(1, 10000);
    for (std::size_t i = 0; i < N; ++i) {
      data_[i] = T{priceDist(rng), symbolDist(rng), volumeDist(rng)};
    }
  }
};


// Constants
constexpr size_t Volume = 1000000;
constexpr size_t BlockSize = 64; // Fallback for systems without
                                 // std::hardware_destructive_interference_size

