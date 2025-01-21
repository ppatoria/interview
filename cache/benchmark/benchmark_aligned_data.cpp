#include <array>
#include <benchmark/benchmark.h>
#include <concepts>
#include <cstdlib> // For posix_memalign
#include <random>
#include <span> // Requires C++20
#include <vector>

/**
 * Non-Aligned Data Structure
 */
struct MarketData {
  int symbol_id;
  double price;
  int volume;

  MarketData() = default;

  MarketData(double price_, int symbolID, int vol)
      : price(price_), symbol_id(symbolID), volume(vol) {}
};

/*
 * Aligned Data Structure
 */
struct alignas(32) MarketData32Aligned {
  int symbol_id;
  double price;
  int volume;

  MarketData32Aligned() = default;

  MarketData32Aligned(double price_, int symbolID, int vol)
      : symbol_id(symbolID), price(price_), volume(vol) {}
};

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

/**
 * Custom Allocator for STL Containers with Specific Alignment
 */
template <typename T, std::size_t Alignment> struct aligned_allocator {
  using value_type = T;

  aligned_allocator() = default;

  template <typename U>
  aligned_allocator(const aligned_allocator<U, Alignment> &) noexcept {}

  T *allocate(std::size_t n) {
    if (n == 0)
      return nullptr;
    if (n > static_cast<std::size_t>(-1) / sizeof(T))
      throw std::bad_alloc();

    void *ptr = nullptr;
    if (posix_memalign(&ptr, Alignment, n * sizeof(T)) != 0)
      throw std::bad_alloc();
    return static_cast<T *>(ptr);
  }

  void deallocate(T *ptr, std::size_t) { free(ptr); }

  template <typename U> struct rebind {
    using other = aligned_allocator<U, Alignment>;
  };
};

/**
 * Simulated Data Input Generator
 */

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

template <typename Container, size_t Volume> class SimulatedInput {
public:
  explicit SimulatedInput(Container container) : data_(std::move(container)) {
    populateData();
  }

  const Container &get() const { return data_; }

private:
  Container data_;

  void populateData() {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> symbolDist(1, 1000);
    std::uniform_real_distribution<double> priceDist(1.0, 1000.0);
    std::uniform_int_distribution<int> volumeDist(1, 10000);
    for (std::size_t i = 0; i < Volume; ++i) {
      data_.emplace_back(priceDist(rng), symbolDist(rng), volumeDist(rng));
    }
  }
};

// Constants
constexpr size_t Volume = 1000000;
constexpr size_t BlockSize = 64; // Fallback for systems without
                                 // std::hardware_destructive_interference_size

/**
 * Benchmarking Functions
 */

using MDVector = std::vector<MarketData>;
static SimulatedInput<MDVector, Volume> inputGenerator(MDVector{});
const auto &SimulatedInputData = inputGenerator.get();

void ProcessMarketData(benchmark::State &state) {

  for (auto _ : state) {
    for (const auto &update : SimulatedInputData) {
      benchmark::DoNotOptimize(&update.symbol_id);
      benchmark::DoNotOptimize(&update.price);
      benchmark::DoNotOptimize(&update.volume);
    }
  }
}
BENCHMARK(ProcessMarketData);

using AlignedDataAlignedMDVector =
    std::vector<MarketData32Aligned,
                aligned_allocator<MarketData32Aligned, 64>>;
static SimulatedInput<AlignedDataAlignedMDVector, Volume>
    alignedDataAlignedVectorInputGenerator(AlignedDataAlignedMDVector{});
const auto &SimulatedAlignedDataAlignedVectorInputData =
    alignedDataAlignedVectorInputGenerator.get();

void ProcessAlignedDataAlignedVectorMarketData(benchmark::State &state) {
  for (auto _ : state) {
    for (const auto &update : SimulatedAlignedDataAlignedVectorInputData) {
      benchmark::DoNotOptimize(&update.price);
      benchmark::DoNotOptimize(&update.symbol_id);
      benchmark::DoNotOptimize(&update.volume);
    }
  }
}
BENCHMARK(ProcessAlignedDataAlignedVectorMarketData);

static SimulatedAlignedArrayGenerator<MarketData32Aligned, Volume>
    alignedDataAlignedArrayInputGenerator;
const auto &SimulatedAlignedDataAlignedArrayInputData =
    alignedDataAlignedArrayInputGenerator.get();

void ProcessAlignedDataAlignedArrayMarketData(benchmark::State &state) {
  for (auto _ : state) {
    for (const auto &update : SimulatedAlignedDataAlignedArrayInputData.data) {
      benchmark::DoNotOptimize(&update.price);
      benchmark::DoNotOptimize(&update.symbol_id);
      benchmark::DoNotOptimize(&update.volume);
    }
  }
}
BENCHMARK(ProcessAlignedDataAlignedArrayMarketData);

using ArrangedMDVector = std::vector<MarketDataArranged>;
static SimulatedInput<ArrangedMDVector, Volume>
    arrangedDataAlignedVectorInputGenerator(ArrangedMDVector{});
const auto &SimulatedArrangedDataAlignedVectorInputData =
    arrangedDataAlignedVectorInputGenerator.get();
void ProcessArrangedDataAlignedVectorMarketData(benchmark::State &state) {
  for (auto _ : state) {
    for (const auto &update : SimulatedArrangedDataAlignedVectorInputData) {
      benchmark::DoNotOptimize(&update.price);
      benchmark::DoNotOptimize(&update.symbol_id);
      benchmark::DoNotOptimize(&update.volume);
    }
  }
}
BENCHMARK(ProcessArrangedDataAlignedVectorMarketData);

static SimulatedAlignedArrayGenerator<MarketDataArranged, Volume>
    arrangedDataAlignedArrayInputGenerator;
const auto &SimulatedArrangedDataAlignedArrayInputData =
    arrangedDataAlignedArrayInputGenerator.get();
void ProcessArrangedDataAlignedArrayMarketData(benchmark::State &state) {
  for (auto _ : state) {
    for (const auto &update : SimulatedArrangedDataAlignedArrayInputData.data) {

      benchmark::DoNotOptimize(&update.price);
      benchmark::DoNotOptimize(&update.symbol_id);
      benchmark::DoNotOptimize(&update.volume);
    }
  }
}
BENCHMARK(ProcessArrangedDataAlignedArrayMarketData);

static SimulatedAlignedArrayGenerator<MarketData, Volume>
    nonAlignedNonArrangedDataAlignedArrayInputGenerator;
const auto &SimulatedNonAlignedNonArrangedDataAlignedArrayInputData =
    nonAlignedNonArrangedDataAlignedArrayInputGenerator.get();

void ProcessNonAlignedNonArrangedDataAlignedArrayMarketData(
    benchmark::State &state) {
  for (auto _ : state) {
    for (const auto &update :
         SimulatedNonAlignedNonArrangedDataAlignedArrayInputData.data) {

      benchmark::DoNotOptimize(&update.price);
      benchmark::DoNotOptimize(&update.symbol_id);
      benchmark::DoNotOptimize(&update.volume);
    }
  }
}
BENCHMARK(ProcessNonAlignedNonArrangedDataAlignedArrayMarketData);

void ProcessArrangedDataAlignedArrayMarketDataUnrollingLoop(
    benchmark::State &state) {
  for (auto _ : state) {

#pragma GCC unroll 4
    for(int i=0; i < SimulatedArrangedDataAlignedArrayInputData.data.size(); i++) {

      benchmark::DoNotOptimize(&SimulatedArrangedDataAlignedArrayInputData.data[i].price);
      benchmark::DoNotOptimize(&SimulatedArrangedDataAlignedArrayInputData.data[i].symbol_id);
      benchmark::DoNotOptimize(&SimulatedArrangedDataAlignedArrayInputData.data[i].volume);
    }
  }
}
BENCHMARK(ProcessArrangedDataAlignedArrayMarketDataUnrollingLoop);

void ProcessArrangedDataAlignedVectorMarketDataUnrollingLoop(
    benchmark::State &state) {
  for (auto _ : state) {

#pragma GCC unroll 4
    for (int i=0;i<SimulatedArrangedDataAlignedVectorInputData.size(); i++) {

      benchmark::DoNotOptimize(&SimulatedArrangedDataAlignedVectorInputData[i].price);
      benchmark::DoNotOptimize(&SimulatedArrangedDataAlignedVectorInputData[i].symbol_id);
      benchmark::DoNotOptimize(&SimulatedArrangedDataAlignedVectorInputData[i].volume);
    }
  }
}
BENCHMARK(ProcessArrangedDataAlignedVectorMarketDataUnrollingLoop);

BENCHMARK_MAIN();
