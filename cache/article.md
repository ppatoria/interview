# Code
```c++
#include <algorithm>
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

template <typename T, size_t N>
class SimulatedAlignedArrayGenerator {
public:
    explicit SimulatedAlignedArrayGenerator(){
    populateData();
  }

    const AlignedArray<T,N> &get() const { return data_; }

private:
    AlignedArray<T,N> data_;

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
constexpr size_t BlockSize = 64;  // Fallback for systems without std::hardware_destructive_interference_size

using MDVector = std::vector<MarketData>;
static SimulatedInput<MDVector, Volume> inputGenerator(MDVector{});
const auto &SimulatedInputData = inputGenerator.get();

using AlignedMDVector =
    std::vector<MarketData32Aligned, aligned_allocator<MarketData32Aligned, 64>>;
static SimulatedInput<AlignedMDVector, Volume>
    alignedInputGenerator(AlignedMDVector{});
const auto &SimulatedAlignedInputData = alignedInputGenerator.get();

using RearrangedMDVector = std::vector<MarketDataArranged>;
static SimulatedInput<RearrangedMDVector, Volume>
    rearrangedInputGenerator(RearrangedMDVector{});
const auto &SimulatedRearrangedInputData = rearrangedInputGenerator.get();

static SimulatedAlignedArrayGenerator<MarketData, Volume>
    alignedArrayInputGenerator;
const auto &SimulatedAlignedArrayInputData = alignedArrayInputGenerator.get();

static SimulatedAlignedArrayGenerator<MarketDataArranged, Volume>
    alignedArrayArrangedDataInputGenerator;
const auto &SimulatedAlignedArrayArrangedInputData =
    alignedArrayArrangedDataInputGenerator.get();

static SimulatedAlignedArrayGenerator<MarketData32Aligned, Volume>
    alignedArrayAlignedDataInputGenerator;
const auto &SimulatedAlignedArrayAlignedInputData = alignedArrayAlignedDataInputGenerator.get();



/**
 * Benchmarking Functions
 */
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

void ProcessAlignedMarketData(benchmark::State& state) {
  for (auto _ : state) {
    for (const auto &update : SimulatedAlignedInputData) {
      benchmark::DoNotOptimize(&update.price);
      benchmark::DoNotOptimize(&update.symbol_id);
      benchmark::DoNotOptimize(&update.volume);
    }
  }
}
BENCHMARK(ProcessAlignedMarketData);

void ProcessRearrangedMarketData(benchmark::State &state) {
  for (auto _ : state) {
    for (const auto &update : SimulatedRearrangedInputData) {
      benchmark::DoNotOptimize(&update.price);
      benchmark::DoNotOptimize(&update.symbol_id);
      benchmark::DoNotOptimize(&update.volume);
    }
  }
}
BENCHMARK(ProcessRearrangedMarketData);

void ProcessAlignedNonArrangedMarketData(benchmark::State &state) {
  for (auto _ : state) {
    for (const auto &update : SimulatedAlignedArrayInputData.data) {

      benchmark::DoNotOptimize(&update.price);
      benchmark::DoNotOptimize(&update.symbol_id);
      benchmark::DoNotOptimize(&update.volume);
    }
  }
}
BENCHMARK(ProcessAlignedNonArrangedMarketData);

void ProcessAlignedArrangedMarketData(benchmark::State &state) {
  for (auto _ : state) {
    for (const auto &update : SimulatedAlignedArrayArrangedInputData.data) {

      benchmark::DoNotOptimize(&update.price);
      benchmark::DoNotOptimize(&update.symbol_id);
      benchmark::DoNotOptimize(&update.volume);
    }
  }
}
BENCHMARK(ProcessAlignedArrangedMarketData);

void ProcessAlignedArrangedMarketDataUnrollingLoop(benchmark::State &state) {
  for (auto _ : state) {

#pragma GCC unroll 4
for (const auto &update : SimulatedAlignedArrayArrangedInputData.data) {

  benchmark::DoNotOptimize(&update.price);
  benchmark::DoNotOptimize(&update.symbol_id);
  benchmark::DoNotOptimize(&update.volume);
}
  }
}
BENCHMARK(ProcessAlignedArrangedMarketDataUnrollingLoop);

void ProcessAlignedArrayAlignedMarketData(benchmark::State &state) {
  for (auto _ : state) {
    for (const auto &update : SimulatedAlignedArrayAlignedInputData.data) {

      benchmark::DoNotOptimize(&update.price);
      benchmark::DoNotOptimize(&update.symbol_id);
      benchmark::DoNotOptimize(&update.volume);
    }
  }
}
BENCHMARK(ProcessAlignedArrayAlignedMarketData);

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
                benchmark::DoNotOptimize(&update.symbol_id);
                benchmark::DoNotOptimize(&update.price);
                benchmark::DoNotOptimize(&update.volume);
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
                benchmark::DoNotOptimize(&update.symbol_id);
                benchmark::DoNotOptimize(&update.price);
                benchmark::DoNotOptimize(&update.volume);
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

            std::span<const MarketData32Aligned> data_span(&*it, count);

            for (const auto& update : data_span) {
                benchmark::DoNotOptimize(&update.symbol_id);
                benchmark::DoNotOptimize(&update.price);
                benchmark::DoNotOptimize(&update.volume);
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

            std::span<const MarketDataArranged> data_span(&*it, count);

            for (const auto& update : data_span) {
                benchmark::DoNotOptimize(&update.symbol_id);
                benchmark::DoNotOptimize(&update.price);
                benchmark::DoNotOptimize(&update.volume);
            }
            it = next_it;
        }
    }
}
BENCHMARK(ProcessRearrangedWithPrefetching);
BENCHMARK_MAIN();
```

# Debug Build Results:
-*- mode: compilation; default-directory: "~/dev/interview/cache/benchmark/" -*-
Compilation started at Mon Jan 20 03:35:42

./build/cache_benchmark
2025-01-20T03:35:44-05:00
Running ./build/cache_benchmark
Run on (4 X 3300 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 3072 KiB (x1)
Load Average: 1.12, 0.94, 0.85
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------------------------
Benchmark                                              Time             CPU   Iterations
----------------------------------------------------------------------------------------
ProcessMarketData                               14938964 ns     14939683 ns           47
ProcessAlignedMarketData                        15238244 ns     15238440 ns           47
ProcessRearrangedMarketData                     14958700 ns     14956311 ns           45
ProcessAlignedNonArrangedMarketData              3543928 ns      3541595 ns          202
ProcessAlignedArrangedMarketData                 3419874 ns      3419930 ns          206
ProcessAlignedArrangedMarketDataUnrollingLoop    3518871 ns      3519036 ns          200
ProcessAlignedArrayAlignedMarketData             3457505 ns      3457597 ns          183
ProcessDefaultDataWithBuffering                 10729308 ns     10730141 ns           67
ProcessAlignedDataWithBuffering                  9804863 ns      9805720 ns           73
ProcessRearrangedDataWithBuffering               8789698 ns      8789422 ns           79
ProcessWithPrefetching                          26007952 ns     26008943 ns           27
ProcessAlignedWithPrefetching                   30532696 ns     30532469 ns           26
ProcessRearrangedWithPrefetching                26798053 ns     26799086 ns           19

Compilation finished at Mon Jan 20 03:35:56
# Release Build Results:
-*- mode: compilation; default-directory: "~/dev/interview/cache/benchmark/" -*-
Compilation started at Mon Jan 20 03:39:26

./build/cache_benchmark
2025-01-20T03:39:27-05:00
Running ./build/cache_benchmark
Run on (4 X 3300 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 3072 KiB (x1)
Load Average: 0.77, 0.84, 0.83
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------------------------
Benchmark                                              Time             CPU   Iterations
----------------------------------------------------------------------------------------
ProcessMarketData                                 452816 ns       452837 ns         1541
ProcessAlignedMarketData                          446854 ns       446830 ns         1562
ProcessRearrangedMarketData                       452473 ns       452503 ns         1547
ProcessAlignedNonArrangedMarketData               447599 ns       447611 ns         1569
ProcessAlignedArrangedMarketData                  452344 ns       452135 ns         1567
ProcessAlignedArrangedMarketDataUnrollingLoop     430286 ns       430303 ns         1639
ProcessAlignedArrayAlignedMarketData              454448 ns       454466 ns         1568
ProcessDefaultDataWithBuffering                  2615618 ns      2615837 ns          252
ProcessAlignedDataWithBuffering                  3068382 ns      3068322 ns          236
ProcessRearrangedDataWithBuffering               1763803 ns      1763939 ns          384
ProcessWithPrefetching                           1713034 ns      1713031 ns          398
ProcessAlignedWithPrefetching                    2226339 ns      2226369 ns          315
ProcessRearrangedWithPrefetching                 1149216 ns      1149284 ns          607

Compilation finished at Mon Jan 20 03:39:41
