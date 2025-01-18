#include <benchmark/benchmark.h>
#include <vector>
#include <algorithm>
#include <array>
#include <random>
#include <chrono>
#include <cstdlib> // For posix_memalign
#include <type_traits>
#include <span>

/**
 * Non-Aligned Data.
 */
struct MarketData {
    int symbol_id;
    double price;
    int volume;

    MarketData() = default;

    MarketData(int symbolID, double price_, int vol)
        : symbol_id(symbolID), price(price_), volume(vol) {}
};

/**
 * Aligned Data.
 */
struct alignas(32) MarketDataAlligned {
    int symbol_id; // 4 bytes
    double price;  // 8 bytes
    int volume;    // 4 bytes

    MarketDataAlligned() = default;

    MarketDataAlligned(int symbolID, double price_, int vol)
        : symbol_id(symbolID), price(price_), volume(vol) {}
};

/**
 * Custom Allocator for custom alignment for STL containers.
 */
template <typename T, std::size_t Alignment>
struct aligned_allocator {
    using value_type = T;

    aligned_allocator() = default;

    template <typename U>
    aligned_allocator(const aligned_allocator<U, Alignment>&) noexcept {}

    T* allocate(std::size_t n) {
        if (n == 0) {
            return nullptr;
        }
        if (n > static_cast<std::size_t>(-1) / sizeof(T)) {
            throw std::bad_alloc();
        }

        void* ptr = nullptr;
        if (posix_memalign(&ptr, Alignment, n * sizeof(T)) != 0) {
            throw std::bad_alloc();
        }
        return static_cast<T*>(ptr);
    }

    void deallocate(T* ptr, std::size_t) {
        free(ptr);
    }

    template <typename U>
    struct rebind {
        using other = aligned_allocator<U, Alignment>;
    };

    bool operator==(const aligned_allocator&) const noexcept { return true; }
    bool operator!=(const aligned_allocator&) const noexcept { return false; }
};

/**
 * Cache line size.
 */
static constexpr std::size_t BlockSize = std::hardware_destructive_interference_size;

/**
 * Generate Simulated Data
 */
template <typename Container, size_t Volume>
class SimulatedInput {
public:
    explicit SimulatedInput(Container container) : data_(std::move(container)) {
        populateData();
    }

    const Container& get() const {
        return data_;
    }

private:
    Container data_;

    void populateData() {
        if constexpr (requires { data_.reserve(Volume); }) {
            data_.reserve(Volume);
        }

        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> symbolDist(1, 1000);
        std::uniform_real_distribution<double> priceDist(1.0, 1000.0);
        std::uniform_int_distribution<int> volumeDist(1, 10000);

        for (std::size_t i = 0; i < Volume; ++i) {
            typename Container::value_type newValue(symbolDist(rng), priceDist(rng), volumeDist(rng));
            data_.emplace_back(std::move(newValue));
        }
    }
};

constexpr size_t volume = 1000;
using MDVector = std::vector<MarketData>;
static SimulatedInput<MDVector, volume> inputGenerator(MDVector{});
const auto& SimulatedInputData = inputGenerator.get();

using AlignedMDVector = std::vector<MarketDataAlligned, aligned_allocator<MarketDataAlligned, 64>>;
static SimulatedInput<AlignedMDVector, volume> alignedInputGenerator(AlignedMDVector{});
const auto& SimulatedAlignedInputData = alignedInputGenerator.get();

/**
 * Cache optimization using buffering.
 */
void processUsingBufferBenchmark(benchmark::State& state) {
    std::array<MarketData, BlockSize> buffer;

    for (auto _ : state) {
        for (std::size_t i = 0; i < SimulatedInputData.size(); i += BlockSize) {
            std::size_t chunk_size = std::min(BlockSize, SimulatedInputData.size() - i);
            std::copy_n(SimulatedInputData.begin() + i, chunk_size, buffer.begin());

            for (std::size_t j = 0; j < chunk_size; ++j) {
                const auto& update = buffer[j];
                benchmark::DoNotOptimize(update.symbol_id);
                benchmark::DoNotOptimize(update.price);
                benchmark::DoNotOptimize(update.volume);
            }
        }
    }
}

BENCHMARK(processUsingBufferBenchmark);

/**
 * Cache optimization using prefetching.
 */
void ProcessByPrefetching(benchmark::State& state) {
    for (auto _ : state) {
        auto it = SimulatedInputData.begin();
        auto end = SimulatedInputData.end();

        while (it != end) {
            std::size_t count = std::min(static_cast<std::size_t>(std::distance(it, end)), BlockSize);
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

BENCHMARK(ProcessByPrefetching);

/**
 * Cache optimization with aligned data.
 */
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

BENCHMARK_MAIN();
