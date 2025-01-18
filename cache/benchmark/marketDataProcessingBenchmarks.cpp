#include <benchmark/benchmark.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <random>
#include <chrono>
#include <cstdlib> // For posix_memalign

#include <vector>
#include <random>
#include <type_traits>


struct MarketData {
    int symbol_id;
    double price;
    int volume;

    MarketData() = default;

    MarketData(int symbolID, double price_, int vol)
    :symbol_id(symbolID),
    price(price_),
    volume(vol){}

};


struct alignas(32) MarketDataAlligned {
    int symbol_id;    // 4 bytes
    double price;     // 8 bytes
    int volume;       // 4 bytes

    MarketDataAlligned() = default;

    MarketDataAlligned(int symbolID, double price_, int vol)
    :symbol_id(symbolID),
    price(price_),
    volume(vol){}


};


// Generic function to populate a container with simulated market data
template <typename Container>
void populateSimulatedInput(Container& updates) {
    constexpr std::size_t largeVolume = 10'000'000; // 10 million records
    
    // Reserve space if the container supports it
    if constexpr (requires { updates.reserve(largeVolume); }) {
        updates.reserve(largeVolume);
    }

    // Random number generation for symbol_id, price, and volume
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> symbolDist(1, 1000);
    std::uniform_real_distribution<double> priceDist(1.0, 1000.0);
    std::uniform_int_distribution<int> volumeDist(1, 10000);

    for (std::size_t i = 0; i < largeVolume; ++i) {
      typename Container::value_type newValue(symbolDist(rng), priceDist(rng), volumeDist(rng));
      updates.emplace_back(std::move(newValue));

    }
}




// Benchmarkable process function
void processUsingBufferBenchmark(benchmark::State& state) {

    std::vector<MarketData> updates;
    populateSimulatedInput(updates);    
    static constexpr std::size_t BlockSize = std::hardware_destructive_interference_size;
    std::array<MarketData, BlockSize> buffer;

    // Benchmark loop
    for (auto _ : state) {
        double total_volume = 0.0; // Example aggregate computation
        double weighted_price_sum = 0.0; // Weighted price computation

        for (std::size_t i = 0; i < updates.size(); i += BlockSize) {
            std::size_t chunk_size = std::min(BlockSize, updates.size() - i);
            std::copy_n(updates.begin() + i, chunk_size, buffer.begin());

            // Process the chunk
            for (std::size_t j = 0; j < chunk_size; ++j) {
                const auto& update = buffer[j];
                total_volume += update.volume;
                weighted_price_sum += update.price * update.volume;
            }
        }

        benchmark::DoNotOptimize(total_volume);
        benchmark::DoNotOptimize(weighted_price_sum);
    }
}

// Register the benchmark
BENCHMARK(processUsingBufferBenchmark);

static void ProcessByPrefetching(benchmark::State& state) {

    std::vector<MarketData> updates;
    populateSimulatedInput(updates);
  
    // Process market data with prefetching
    for (auto _ : state) {
        auto it = updates.begin();
        auto end = updates.end();
        static constexpr std::size_t BlockSize = std::hardware_destructive_interference_size;

        while (it != end) {
            // Prefetch the next block into the cache
            std::size_t count = std::min(static_cast<std::size_t>(std::distance(it, end)), BlockSize);
            auto next_it = std::next(it, count);
            for (auto prefetch_it = it; prefetch_it != next_it; ++prefetch_it) {
                __builtin_prefetch(&(*prefetch_it), 0, 3);
            }

            // Create a span for the current range
            std::span<const MarketData> data_span(&*it, count);

            // Process the current range
            for (const auto& update : data_span) {
                benchmark::DoNotOptimize(update.symbol_id);
                benchmark::DoNotOptimize(update.price);
                benchmark::DoNotOptimize(update.volume);
            }

            it = next_it; // Advance iterator
        }
    }
}

BENCHMARK(ProcessByPrefetching);

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

    // Comparison operators for compatibility
    bool operator==(const aligned_allocator&) const noexcept { return true; }
    bool operator!=(const aligned_allocator&) const noexcept { return false; }
};




// Single function to process market data with aligned vectors and benchmarking
static void ProcessAlignedMarketData(benchmark::State& state) {
    // Create an aligned vector of market data updates
    std::vector<MarketDataAlligned, 
                aligned_allocator<MarketDataAlligned, 64>> updates;
    populateSimulatedInput(updates);
    
    // Benchmark the processing of the aligned vector
    for (auto _ : state) {
        for (const auto& update : updates) {
            // Simulate processing each market data update
            benchmark::DoNotOptimize(update.symbol_id);
            benchmark::DoNotOptimize(update.price);
            benchmark::DoNotOptimize(update.volume);
        }
    }

    // Output some results for validation (can be removed in actual benchmarks)
    std::cout << "Processed " << updates.size() << " market data updates." << std::endl;
}

// Register the benchmark
BENCHMARK(ProcessAlignedMarketData);
