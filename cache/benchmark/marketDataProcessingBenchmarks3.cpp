#include <benchmark/benchmark.h>
#include <vector>
#include <array>
#include <algorithm>
#include <random>
#include <span>  // Requires C++20
#include <cstdlib>  // For posix_memalign

/**
 * Non-Aligned Data Structure
 */
struct MarketData {
    int symbol_id;
    double price;
    int volume;

    MarketData() = default;

    MarketData(int symbolID, double price_, int vol)
        : symbol_id(symbolID), price(price_), volume(vol) {}
};

/*
 * Aligned Data Structure
 */
struct alignas(32) MarketDataAligned {
    int symbol_id;
    double price;
    int volume;

    MarketDataAligned() = default;

    MarketDataAligned(int symbolID, double price_, int vol)
        : symbol_id(symbolID), price(price_), volume(vol) {}
 };

struct MarketDataRearranged {
  double price;
  int symbol_id;
  int volume;

  MarketDataRearranged() = default;

  MarketDataRearranged(double price_, int symbolID, int vol)
      : price(price_), symbol_id(symbolID), volume(vol) {}
};

/**
 * Custom Allocator for STL Containers with Specific Alignment
 */
template <typename T, std::size_t Alignment>
struct aligned_allocator {
    using value_type = T;

    aligned_allocator() = default;

    template <typename U>
    aligned_allocator(const aligned_allocator<U, Alignment>&) noexcept {}

    T* allocate(std::size_t n) {
        if (n == 0) return nullptr;
        if (n > static_cast<std::size_t>(-1) / sizeof(T)) throw std::bad_alloc();

        void* ptr = nullptr;
        if (posix_memalign(&ptr, Alignment, n * sizeof(T)) != 0) throw std::bad_alloc();
        return static_cast<T*>(ptr);
    }

    void deallocate(T* ptr, std::size_t) { free(ptr); }

    template <typename U>
    struct rebind {
        using other = aligned_allocator<U, Alignment>;
    };
};

/**
 * Simulated Data Input Generator
 */
template <typename Container, size_t Volume>
class SimulatedInput {
public:
    explicit SimulatedInput(Container container) 
    : data_(std::move(container)) 
    {
        populateData();
    }

    const Container& get() const { return data_; }

private:
    Container data_;

    void populateData() {
        if constexpr (requires { data_.reserve(Volume); }) data_.reserve(Volume);

        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> symbolDist(1, 1000);
        std::uniform_real_distribution<double> priceDist(1.0, 1000.0);
        std::uniform_int_distribution<int> volumeDist(1, 10000);

        for (std::size_t i = 0; i < Volume; ++i) {
            data_.emplace_back(symbolDist(rng), priceDist(rng), volumeDist(rng));
        }
    }
};

// Constants
constexpr size_t Volume = 1000;
constexpr size_t BlockSize = 64;  // Fallback for systems without std::hardware_destructive_interference_size

using MDVector = std::vector<MarketData>;
using AlignedMDVector = std::vector<MarketDataAligned, aligned_allocator<MarketDataAligned, 64>>;
using RearrangedMDVector = std::vector<MarketDataRearranged>;

static SimulatedInput<MDVector, Volume> inputGenerator(MDVector{});
static SimulatedInput<AlignedMDVector, Volume> alignedInputGenerator(AlignedMDVector{});
static SimulatedInput<RearrangedMDVector, Volume> rearrangedInputGenerator(RearrangedMDVector{});

const auto& SimulatedInputData = inputGenerator.get();
const auto& SimulatedAlignedInputData = alignedInputGenerator.get();
const auto& SimulatedRearrangedInputData = rearrangedInputGenerator.get();

/**
 * Benchmarking Functions
 */
void ProcessMarketData(benchmark::State& state) {
    for (auto _ : state) {
        for (const auto& update : SimulatedInputData) {
            benchmark::DoNotOptimize(update.symbol_id);
            benchmark::DoNotOptimize(update.price);
            benchmark::DoNotOptimize(update.volume);
        }
    }
}
BENCHMARK(ProcessMarketData);

void ProcessAlignedMarketData(benchmark::State& state) {
    for (auto _ : state) {
        for (const auto& update : SimulatedAlignedInputData) {
            benchmark::DoNotOptimize(update.symbol_id);
            benchmark::DoNotOptimize(update.price);
            benchmark::DoNotOptimize(update.volume);
        }
    }
}
BENCHMARK(ProcessAlignedMarketData);

void ProcessRearrangedMarketData(benchmark::State &state) {
  for (auto _ : state) {
    for (const auto &update : SimulatedRearrangedInputData) {
      benchmark::DoNotOptimize(update.symbol_id);
      benchmark::DoNotOptimize(update.price);
      benchmark::DoNotOptimize(update.volume);
    }
  }
}
BENCHMARK(ProcessRearrangedMarketData);

template <typename DataType, template <typename, typename> class Container, typename Allocator>
void ProcessDataWithBuffering(
          const Container<DataType, Allocator>& updates, 
          benchmark::State& state) 
{
    std::array<DataType, BlockSize> buffer;

    for (auto _ : state) {
        for (std::size_t i = 0; i < updates.size(); i += BlockSize) {
            std::size_t chunk_size = std::min(BlockSize, updates.size() - i);
            std::copy_n(updates.begin() + i, chunk_size, buffer.begin());

            // Process the buffered data
            for (std::size_t j = 0; j < chunk_size; ++j) {
                const auto& update = buffer[j];
                benchmark::DoNotOptimize(update.symbol_id);
                benchmark::DoNotOptimize(update.price);
                benchmark::DoNotOptimize(update.volume);
            }
        }
    }
}


// Benchmark for default data with buffering
void ProcessDefaultDataWithBuffering(benchmark::State& state) {
    ProcessDataWithBuffering(SimulatedInputData, state);
}
BENCHMARK(ProcessDefaultDataWithBuffering);

// Benchmark for aligned data with buffering
void ProcessAlignedDataWithBuffering(benchmark::State& state) {
    ProcessDataWithBuffering(SimulatedAlignedInputData, state);
}
BENCHMARK(ProcessAlignedDataWithBuffering);

// Benchmark for rearranged data with buffering
void ProcessRearrangedDataWithBuffering(benchmark::State& state) {
    ProcessDataWithBuffering(SimulatedRearrangedInputData, state);
}
BENCHMARK(ProcessRearrangedDataWithBuffering);

void ProcessWithPrefetching(benchmark::State& state) {
    const auto& updates = SimulatedInputData;

    for (auto _ : state) {
        auto it = updates.begin();
        auto end = updates.end();

        while (it != end) {
            std::size_t count = std::min(static_cast<size_t>(std::distance(it, end)), BlockSize);
            auto next_it = std::next(it, count);
            for (auto prefetch_it = it; prefetch_it != next_it; ++prefetch_it) {
                __builtin_prefetch(&(*prefetch_it), 0, 3);
            }

            std::span<const MarketData> data_span(&*it, count);

            for (const auto& update : data_span) {
                benchmark::DoNotOptimize(update.symbol_id);
                benchmark::DoNotOptimize(update.price);
                benchmark::DoNotOptimize(update.volume);
            }
            it = next_it;
        }
    }
}
BENCHMARK(ProcessWithPrefetching);

void ProcessAlignedWithPrefetching(benchmark::State& state) {
    const auto& updates = SimulatedAlignedInputData;

    for (auto _ : state) {
        auto it = updates.begin();
        auto end = updates.end();

        while (it != end) {
            std::size_t count = std::min(static_cast<size_t>(std::distance(it, end)), BlockSize);
            auto next_it = std::next(it, count);
            for (auto prefetch_it = it; prefetch_it != next_it; ++prefetch_it) {
                __builtin_prefetch(&(*prefetch_it), 0, 3);
            }

            std::span<const MarketDataAligned> data_span(&*it, count);

            for (const auto& update : data_span) {
                benchmark::DoNotOptimize(update.symbol_id);
                benchmark::DoNotOptimize(update.price);
                benchmark::DoNotOptimize(update.volume);
            }
            it = next_it;
        }
    }
}
BENCHMARK(ProcessAlignedWithPrefetching);

void ProcessRearrangedWithPrefetching(benchmark::State& state) {
    const auto& updates = SimulatedRearrangedInputData;

    for (auto _ : state) {
        auto it = updates.begin();
        auto end = updates.end();

        while (it != end) {
            std::size_t count = std::min(static_cast<size_t>(std::distance(it, end)), BlockSize);
            auto next_it = std::next(it, count);
            for (auto prefetch_it = it; prefetch_it != next_it; ++prefetch_it) {
                __builtin_prefetch(&(*prefetch_it), 0, 3);
            }

            std::span<const MarketDataRearranged> data_span(&*it, count);

            for (const auto& update : data_span) {
                benchmark::DoNotOptimize(update.symbol_id);
                benchmark::DoNotOptimize(update.price);
                benchmark::DoNotOptimize(update.volume);
            }
            it = next_it;
        }
    }
}
BENCHMARK(ProcessRearrangedWithPrefetching);
