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

static SimulatedAlignedArrayGenerator<MarketData32Aligned, Volume>
    alignedArrayAlignedDataInputGenerator;
const auto &SimulatedAlignedArrayAlignedInputData = alignedArrayAlignedDataInputGenerator.get();

static SimulatedAlignedArrayGenerator<MarketData, Volume>
    alignedArrayInputGenerator;
const auto &SimulatedAlignedArrayInputData = alignedArrayInputGenerator.get();

static SimulatedAlignedArrayGenerator<MarketDataArranged, Volume>
    alignedArrayArrangedDataInputGenerator;
const auto &SimulatedAlignedArrayArrangedInputData =
    alignedArrayArrangedDataInputGenerator.get();

using ArrangedAlignedMDVector = std::vector<MarketDataArranged,
                                    aligned_allocator<MarketDataArranged, 64>>;
static SimulatedInput<ArrangedAlignedMDVector, Volume>
    arrangedAlignedVectorInputGenerator(ArrangedAlignedMDVector{});
const auto &SimulatedArrangedAlignedVectorInputData = arrangedAlignedVectorInputGenerator.get();

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

void ProcessAlignedVectorArrangedMarketData(benchmark::State &state) {
  for (auto _ : state) {
      for (const auto &update : SimulatedArrangedAlignedVectorInputData){
      benchmark::DoNotOptimize(&update.price);
      benchmark::DoNotOptimize(&update.symbol_id);
      benchmark::DoNotOptimize(&update.volume);
    }
  }
}
BENCHMARK(ProcessAlignedVectorArrangedMarketData);


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


// Debug mode
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// -*- mode: compilation; default-directory: "~/dev/interview/cache/benchmark/build/" -*-                                 //
// Compilation started at Sun Jan 19 04:01:08                                                                             //
//                                                                                                                        //
// cd build/; cmake ../; make ; ./cache_benchmark                                                                         //
// -- Configuring done                                                                                                    //
// -- Generating done                                                                                                     //
// -- Build files have been written to: /home/ppatoria/dev/interview/cache/benchmark/build                                //
// Consolidate compiler generated dependencies of target cache_benchmark                                                  //
// [ 50%] Building CXX object CMakeFiles/cache_benchmark.dir/benchmark.cpp.o                                              //
// [100%] Linking CXX executable cache_benchmark                                                                          //
// [100%] Built target cache_benchmark                                                                                    //
// 2025-01-19T04:01:11-05:00                                                                                              //
// Running ./cache_benchmark                                                                                              //
// Run on (4 X 3300 MHz CPU s)                                                                                            //
// CPU Caches:                                                                                                            //
//   L1 Data 32 KiB (x2)                                                                                                  //
//   L1 Instruction 32 KiB (x2)                                                                                           //
//   L2 Unified 256 KiB (x2)                                                                                              //
//   L3 Unified 3072 KiB (x1)                                                                                             //
// Load Average: 1.14, 1.44, 1.55                                                                                         //
// ***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead. //
// ------------------------------------------------------------------------------                                         //
// Benchmark                                    Time             CPU   Iterations                                         //
// ------------------------------------------------------------------------------                                         //
// ProcessMarketData                     12142007 ns     12130609 ns           62                                         //
// ProcessAlignedMarketData              11987555 ns     11976287 ns           59                                         //
// ProcessRearrangedMarketData           11421725 ns     11412121 ns           60                                         //
// ProcessAlignedNonArrangedMarketData    3550849 ns      3548313 ns          195                                         //
// ProcessAlignedArrangedMarketData       3518831 ns      3516521 ns          198                                         //
// ProcessDefaultDataWithBuffering        9973000 ns      9963683 ns           70                                         //
// ProcessAlignedDataWithBuffering        9144951 ns      9136090 ns           75                                         //
// ProcessRearrangedDataWithBuffering    11167273 ns     11136324 ns           86                                         //
// ProcessWithPrefetching                20539593 ns     20538612 ns           34                                         //
// ProcessAlignedWithPrefetching         20635539 ns     20635318 ns           34                                         //
// ProcessRearrangedWithPrefetching      20326708 ns     20325925 ns           35                                         //
//                                                                                                                        //
// Compilation finished at Sun Jan 19 04:01:21                                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
# Release Mode
2025-01-19T12:03:10-05:00
Running ./cache_benchmark
Run on (4 X 3300 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 3072 KiB (x1)
Load Average: 0.92, 0.85, 0.94
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------------
Benchmark                                     Time             CPU   Iterations
-------------------------------------------------------------------------------
ProcessMarketData                        404610 ns       405071 ns         1719
ProcessAlignedMarketData                 514260 ns       514844 ns         1615
ProcessRearrangedMarketData              407211 ns       407669 ns         1352
ProcessAlignedNonArrangedMarketData      428100 ns       428575 ns         1664
ProcessAlignedArrangedMarketData         436971 ns       437424 ns         1633
ProcessAlignedArrayAlignedMarketData     438603 ns       439051 ns         1596
ProcessDefaultDataWithBuffering         3058098 ns      3060837 ns          224
ProcessAlignedDataWithBuffering         3089844 ns      3092825 ns          234
ProcessRearrangedDataWithBuffering      2385500 ns      2387697 ns          286
ProcessWithPrefetching                  1718383 ns      1720087 ns          385
ProcessAlignedWithPrefetching           2253740 ns      2255871 ns          301
ProcessRearrangedWithPrefetching        1154778 ns      1155876 ns          553

*/

/*
 The behavior you're observing is due to the differences in optimizations applied in **Release** and **Debug** build modes:

### **Debug Mode (Unoptimized Code)**
1. **Lack of Compiler Optimizations:**
   - Debug mode prioritizes debugging information over performance.
   - The compiler doesn't apply aggressive optimizations like **inlining**, **loop unrolling**, or **vectorization**.
   - As a result, the performance reflects the raw cost of each instruction and memory access, exposing inefficiencies in non-aligned and non-arranged data.

2. **Alignment and Cache Effects:**
   - Debug mode doesn't optimize memory layout or data access patterns.
   - **Aligned and arranged data** exhibit better performance because the memory accesses are more efficient for the CPU's cache.

3. **Extra Debug Overheads:**
   - Debug mode adds instrumentation to help with debugging (e.g., bounds checking, function call stack maintenance).
   - These extra instructions amplify the differences between efficient (aligned/arranged) and inefficient (non-aligned) data layouts.

---

### **Release Mode (Optimized Code)**
1. **Aggressive Optimizations:**
   - Release mode enables optimizations like:
     - **Instruction scheduling** to reduce stalls.
     - **Loop unrolling** to improve throughput.
     - **Inlining** of small functions.
     - **Prefetching** and **SIMD vectorization** for better use of hardware resources.
   - These optimizations often make differences between aligned/arranged and non-aligned/non-arranged data layouts less noticeable.

2. **Branch Prediction Improvements:**
   - The CPU's branch predictor works more effectively with streamlined, optimized code.
   - Conditional operations and loops may be restructured to minimize pipeline stalls.

3. **Memory Access Patterns:**
   - Even non-aligned and non-arranged data can be processed quickly when the compiler optimizes memory accesses, prefetches data into the cache, or leverages SIMD instructions.
   - Modern CPUs are highly capable of handling misaligned accesses with minimal penalties when combined with compiler optimizations.

4. **Vectorization and Prefetching:**
   - Non-aligned vector processing benefits significantly from SIMD optimizations, especially for contiguous memory regions.
   - Optimized code may exhibit similar or better performance than arranged data due to reduced overhead from the optimizations.

---

### **Key Observations from Your Results**
- **In Debug Mode:**
  - The performance difference between arranged/aligned and non-arranged/non-aligned data is more pronounced because of raw access costs and lack of optimizations.
  - Aligned and arranged data dominate due to reduced cache misses and alignment benefits.

- **In Release Mode:**
  - The performance gap narrows or even reverses because optimizations mitigate the penalties of misalignment and inefficiency.
  - The additional overhead of arranging and aligning data might not provide as much benefit due to the CPU's ability to handle inefficiencies.

---

### **Why Non-Arranged Data Performs Well in Release Mode**
1. **Reduced Overhead for Simple Processing:**
   - Non-arranged data may skip unnecessary preprocessing overhead that aligned/arranged data might involve.
   - If the processing is straightforward, the CPU optimizations can handle non-aligned data efficiently.

2. **Compiler Optimizations:**
   - Optimizations might include loop fusion, reordering, or instruction-level parallelism, reducing penalties from misaligned accesses.
   - Prefetching and speculative execution can mitigate cache and alignment penalties.

3. **Aligned Data Overhead:**
   - Aligning data may incur setup or maintenance overhead that becomes unnecessary under optimal conditions.

---

### **How to Analyze and Confirm the Cause**
To better understand the behavior, consider the following:

1. **Profile Cache Performance:**
   - Use **Valgrind Cachegrind** or **Intel VTune** to analyze cache misses and memory access patterns for both builds.

2. **Inspect Assembly Code:**
   - Compare the generated assembly for Debug and Release builds using `objdump` or `Godbolt Compiler Explorer`:
     ```bash
     objdump -d cache_benchmark > debug_asm.txt
     objdump -d cache_benchmark_release > release_asm.txt
     ```

3. **Analyze Compiler Optimizations:**
   - Use `-fopt-info` with `gcc` or `clang` to understand applied optimizations:
     ```bash
     g++ -O3 -fopt-info-optimized benchmark.cpp
     ```

4. **Evaluate Specific Optimizations:**
   - Disable specific optimizations in the Release build (e.g., `-fno-unroll-loops`) to see their impact.

5. **Use Benchmarks with Larger Data Sets:**
   - Larger inputs might highlight the differences better, as alignment and arrangement benefits grow with input size.

By profiling and analyzing, you'll gain deeper insights into how specific compiler optimizations affect your data processing patterns.
 */
