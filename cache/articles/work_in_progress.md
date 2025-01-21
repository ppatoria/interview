# Cache Optimization Techniques: A Deep Dive

## Introduction
Cache optimization is a critical factor in achieving high performance in modern applications. In this article, we explore three techniques we tested to improve cache efficiency:

1. **Data and Data Structure Alignment**
2. **Buffering**
3. **Prefetching**

We provide an overview of each approach and analyze their impact on performance based on experimental results. Along the way, we will refine the design of the `MarketData` struct step by step, highlighting the intricacies of cache optimization.

---

## Techniques Explored

### 1. Data and Data Structure Alignment
Aligned data ensures that the memory layout is conducive to efficient cache usage. We tested this approach by aligning individual structures and arrays to cache line boundaries (e.g., 32 or 64 bytes). The variations included:

- Aligning individual structures (e.g., using `alignas` keywords).
- Arranging the fields of the structure to minimize padding.
- Ensuring arrays of structures are aligned to cache lines.

### 2. Buffering
Buffering involves loading a block of data into a temporary buffer for batch processing, reducing cache misses. However, our results showed that buffering was not efficient in the tested scenarios. We believe this is due to the overhead of copying data into the buffer, which negated any potential benefits. (This is an observation and not a verified conclusion.)

### 3. Prefetching
Prefetching anticipates data usage by explicitly loading data into the cache before it is accessed. Despite its potential, prefetching showed limited efficiency in our tests. The additional loop for prefetching and the cost of explicit prefetch calls likely outweighed the gains. (This is an observation based on our test setup; the actual reasons require further investigation.)

---

## Key Findings

1. **Default Behavior**
   - In optimized builds (e.g., `-O3`), the compiler’s default handling often performed well, even without manual alignment or data rearrangement.
   - Possible reasons: advanced compiler optimizations such as vectorization, efficient memory layout, and field-wise optimizations within structures.

2. **Data Alignment**
   - Best results were achieved by arranging data to fit maximally within cache lines.
   - Using arrays instead of vectors (eliminating dynamic allocations) and aligning arrays to cache line boundaries improved performance.
   - Explicit loop unrolling equivalent to the cache line size yielded additional gains.

3. **Buffering and Prefetching**
   - Buffering was less effective due to potential copying overhead.
   - Prefetching didn’t provide significant benefits in our scenario, possibly due to its implementation overhead.

---

## Step-by-Step Refinement of `MarketData`

### Initial Structure
```cpp
struct MarketData {
    int symbol_id;
    double price;
    int volume;
};
```
**Problem:** Suboptimal cache alignment and padding between fields.

### Aligned Structure
```cpp
struct alignas(32) MarketDataAligned {
    int symbol_id;
    double price;
    int volume;
};
```
**Impact:** Alignment ensures memory accesses are cache-line friendly, reducing cache misses.

### Rearranged Fields
```cpp
struct MarketDataArranged {
    double price;
    int symbol_id;
    int volume;
};
```
**Impact:** Rearranging fields minimizes padding, optimizing memory layout.

### Using Aligned Arrays
```cpp
template <typename T, std::size_t N>
struct AlignedArray {
    alignas(64) std::array<T, N> data;
};
```
**Impact:** Entire arrays are cache-line aligned, enhancing sequential access efficiency.

---

## Experimental Results
### Debug Build Performance (Selected Results)
| Technique                              | Time (ns) | Iterations |
|---------------------------------------|-----------|------------|
| Default (unaligned)                   | 14,938,964 | 47         |
| Aligned Structures                    | 3,543,928  | 202        |
| Aligned & Arranged Data               | 3,419,874  | 206        |
| Buffering (Default)                   | 10,729,308 | 67         |
| Prefetching (Default)                 | 26,007,952 | 27         |

### Release Build Performance (Selected Results)
| Technique                              | Time (ns) | Iterations |
|---------------------------------------|-----------|------------|
| Default (unaligned)                   | 452,816    | 1,541      |
| Aligned Structures                    | 447,599    | 1,569      |
| Aligned & Arranged Data               | 452,344    | 1,567      |
| Buffering (Default)                   | 2,615,618  | 252        |
| Prefetching (Default)                 | 1,713,034  | 398        |

---

## Conclusion
Our experiments demonstrate the importance of understanding and leveraging cache alignment for performance optimization. Key takeaways include:

1. Aligning data and structuring fields thoughtfully can yield significant performance gains.
2. Compiler optimizations in release builds often outperform manual techniques under certain conditions.
3. Techniques like buffering and prefetching are scenario-dependent and require careful implementation to be effective.

Future work includes exploring:

- Profiling and validating the assumptions behind buffering and prefetching results.
- Examining advanced compiler optimizations like vectorization and their interaction with custom data structures.

---

## Diagrams and Visuals

![Cache Line Alignment](https://example.com/cache-line-alignment.png)  
*Visualizing data alignment to cache lines.*

![Performance Trends](https://example.com/performance-trends.png)  
*Comparing performance metrics for various techniques.*

*Note: Replace the placeholder links with actual images generated or sourced.*

---

Let’s continue to push the boundaries of performance through cache optimization!
# Advanced Cache Optimization: Insights and Code Walkthrough

## Introduction
Cache optimization is a cornerstone of high-performance computing. This article delves into advanced cache optimization techniques through code examples and step-by-step improvements. By leveraging benchmarking results, we refine the design and implementation of a `MarketData` struct, illustrating how specific optimizations translate into tangible performance gains.

---

## Initial Implementation: Baseline Design
The initial design of the `MarketData` struct is as follows:

```cpp
struct MarketData {
    int symbol_id;
    double price;
    int volume;
};
```

### Benchmark Results
- **Debug Build:** ~14,938,964 ns
- **Release Build:** ~452,816 ns

### Analysis
The structure is simple but suboptimal for cache usage due to potential misalignment and padding between fields. Let’s refine it step by step.

---

## Step 1: Aligning Data Structures
Alignment ensures memory accesses are cache-line friendly. By explicitly aligning `MarketData` to 32 bytes, we reduce cache misses.

### Aligned Implementation
```cpp
struct alignas(32) MarketDataAligned {
    int symbol_id;
    double price;
    int volume;
};
```

### Benchmark Results
- **Debug Build:** ~3,543,928 ns
- **Release Build:** ~447,599 ns

### Explanation
Aligning the structure improves cache utilization by ensuring that each instance fits neatly into a cache line. This reduces the likelihood of cache line splits during sequential access.

---

## Step 2: Rearranging Fields
Reordering fields minimizes padding by grouping variables with similar sizes together. This further optimizes memory layout.

### Rearranged Implementation
```cpp
struct MarketDataArranged {
    double price;
    int symbol_id;
    int volume;
};
```

### Benchmark Results
- **Debug Build:** ~3,419,874 ns
- **Release Build:** ~452,344 ns

### Explanation
By placing the largest field (`double price`) first, we reduce internal fragmentation. This change optimizes the memory layout, ensuring better cache efficiency.

---

## Step 3: Using Aligned Arrays
Aligning arrays of structures ensures the entire data block is cache-line friendly. This is particularly beneficial for batch processing.

### Array Alignment Implementation
```cpp
template <typename T, std::size_t N>
struct AlignedArray {
    alignas(64) std::array<T, N> data;
};

using AlignedMarketDataArray = AlignedArray<MarketDataArranged, 1000>;
```

### Benchmark Results
- **Debug Build:** ~3,419,874 ns
- **Release Build:** ~452,344 ns

### Explanation
Aligned arrays leverage cache lines more effectively during sequential access. The alignment ensures fewer cache misses when iterating over the array.

---

## Step 4: Loop Unrolling
Unrolling loops to match the number of elements that fit in a cache line maximizes efficiency by reducing loop overhead and leveraging instruction-level parallelism.

### Unrolled Loop Implementation
```cpp
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
```

### Benchmark Results
- **Debug Build:** ~3,518,871 ns
- **Release Build:** ~430,286 ns

### Explanation
Loop unrolling reduces the overhead of loop control and enables the compiler to better optimize instruction scheduling. This approach is particularly effective when processing data aligned to cache lines.

---

## Buffering: A Less Effective Approach
Buffering involves copying data into a temporary buffer for batch processing. While conceptually promising, our results indicate limited efficiency gains.

### Buffering Implementation
```cpp
template <typename DataType, template <typename, typename> class Container, typename Allocator>
void ProcessDataWithBuffering(const Container<DataType, Allocator>& updates, benchmark::State& state) {
    std::array<DataType, 64> buffer;

    for (auto _ : state) {
        for (std::size_t i = 0; i < updates.size(); i += 64) {
            std::size_t chunk_size = std::min<size_t>(64, updates.size() - i);
            std::copy_n(updates.begin() + i, chunk_size, buffer.begin());

            for (std::size_t j = 0; j < chunk_size; ++j) {
                const auto& update = buffer[j];
                benchmark::DoNotOptimize(&update.symbol_id);
                benchmark::DoNotOptimize(&update.price);
                benchmark::DoNotOptimize(&update.volume);
            }
        }
    }
}
```

### Benchmark Results
- **Debug Build:** ~10,729,308 ns
- **Release Build:** ~2,615,618 ns

### Explanation
The overhead of copying data into the buffer likely outweighs the benefits, especially for scenarios with large data volumes.

---

## Prefetching: Observation-Based Results
Prefetching aims to load data into the cache proactively. However, our tests revealed limited efficiency, likely due to its implementation overhead.

### Prefetching Implementation
```cpp
void ProcessWithPrefetching(benchmark::State &state) {
    const auto &updates = SimulatedInputData;

    for (auto _ : state) {
        auto it = updates.begin();
        auto end = updates.end();

        while (it != end) {
            std::size_t count = std::min(static_cast<size_t>(std::distance(it, end)), 64);
            auto next_it = std::next(it, count);
            for (auto prefetch_it = it; prefetch_it != next_it; ++prefetch_it) {
                __builtin_prefetch(&(*prefetch_it), 0, 3);
            }

            for (auto current = it; current != next_it; ++current) {
                benchmark::DoNotOptimize(&current->symbol_id);
                benchmark::DoNotOptimize(&current->price);
                benchmark::DoNotOptimize(&current->volume);
            }

            it = next_it;
        }
    }
}
```

### Benchmark Results
- **Debug Build:** ~26,007,952 ns
- **Release Build:** ~1,713,034 ns

### Explanation
The extra loop and explicit prefetching calls may negate potential benefits. Further profiling is needed to validate these observations.

---

## Conclusion
Through iterative optimizations, we observed:

1. Data alignment and rearrangement significantly improved performance.
2. Compiler optimizations in release builds often rival manual techniques.
3. Buffering and prefetching are scenario-dependent and require careful implementation.

Future work should include deeper profiling to validate assumptions and exploring more advanced compiler optimizations.

---

*Note: Replace placeholder diagram links with actual visuals if needed.*


# Advanced Cache Optimization: Insights and Code Walkthrough

## Introduction
Cache optimization is a cornerstone of high-performance computing. This article delves into advanced cache optimization techniques through code examples and step-by-step improvements. By leveraging benchmarking results, we refine the design and implementation of a `MarketData` struct, illustrating how specific optimizations translate into tangible performance gains.

---

## Initial Implementation: Baseline Design
The initial design of the `MarketData` struct is as follows:

```cpp
struct MarketData {
    int symbol_id;
    double price;
    int volume;
};
```

### Benchmark Results
- **Debug Build:** ~14,938,964 ns
- **Release Build:** ~452,816 ns

### Analysis
The structure is simple but suboptimal for cache usage due to potential misalignment and padding between fields. Let’s refine it step by step.

---

## Step 1: Aligning Data
Alignment ensures memory accesses are cache-line friendly. By explicitly aligning `MarketData` to 32 bytes, we reduce cache misses.

### Aligned Implementation
```cpp
struct alignas(32) MarketDataAligned {
    int symbol_id;
    double price;
    int volume;
};
```

### Benchmark Results
- **Debug Build:** ~3,543,928 ns
- **Release Build:** ~447,599 ns

### Explanation
Aligning the structure improves cache utilization by ensuring that each instance fits neatly into a cache line. This reduces the likelihood of cache line splits during sequential access. Additionally, we found that this step can be further improved by removing the 32-byte alignment and instead rearranging the fields to align with a 64-byte cache line for better cache line utilization.

---

## Step 2: Rearranging Fields
Reordering fields minimizes padding by grouping variables with similar sizes together. This further optimizes memory layout.

### Rearranged Implementation
```cpp
struct MarketDataArranged {
    double price;
    int symbol_id;
    int volume;
};
```

### Benchmark Results
- **Debug Build:** ~3,419,874 ns
- **Release Build:** ~452,344 ns

### Explanation
By placing the largest field (`double price`) first, we reduce internal fragmentation. This change optimizes the memory layout, ensuring better cache efficiency.

---

## Step 3: Aligning Containers
Aligning entire containers of data ensures the data block is cache-line friendly. This is particularly beneficial for batch processing. Two subpoints are considered here:

### (a) Using Aligned Arrays
Aligned arrays leverage cache lines effectively during sequential access, ensuring fewer cache misses.

#### Array Alignment Implementation
```cpp
template <typename T, std::size_t N>
struct AlignedArray {
    alignas(64) std::array<T, N> data;
};

using AlignedMarketDataArray = AlignedArray<MarketDataArranged, 1000>;
```

### Benchmark Results
- **Debug Build:** ~3,419,874 ns
- **Release Build:** ~452,344 ns

### Explanation
Aligned arrays are highly efficient for fixed-size data due to their contiguous and cache-friendly memory layout.

### (b) Using Aligned Vectors
When dynamic arrays are required, aligned vectors provide an alternative. Using a custom aligned allocator ensures that vector data is also cache-aligned.

#### Custom Allocator Implementation
```cpp
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
```

#### Usage Example
```cpp
using AlignedVector = std::vector<MarketDataArranged, aligned_allocator<MarketDataArranged, 64>>;
AlignedVector alignedVector;
```

### Explanation
While not as efficient as arrays due to dynamic memory management overhead, aligned vectors are useful when flexibility is needed.

---

## Step 4: Loop Unrolling
Unrolling loops to match the number of elements that fit in a cache line maximizes efficiency by reducing loop overhead and leveraging instruction-level parallelism.

### Unrolled Loop Implementation
```cpp
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
```

### Benchmark Results
- **Debug Build:** ~3,518,871 ns
- **Release Build:** ~430,286 ns

### Explanation
Loop unrolling reduces the overhead of loop control and enables the compiler to better optimize instruction scheduling. This approach is particularly effective when processing data aligned to cache lines.

---

## Buffering: A Less Effective Approach
Buffering involves copying data into a temporary buffer for batch processing. While conceptually promising, our results indicate limited efficiency gains. However, this technique may prove beneficial in scenarios not explored here.

### Buffering Implementation
```cpp
template <typename DataType, template <typename, typename> class Container, typename Allocator>
void ProcessDataWithBuffering(const Container<DataType, Allocator>& updates, benchmark::State& state) {
    std::array<DataType, 64> buffer;

    for (auto _ : state) {
        for (std::size_t i = 0; i < updates.size(); i += 64) {
            std::size_t chunk_size = std::min<size_t>(64, updates.size() - i);
            std::copy_n(updates.begin() + i, chunk_size, buffer.begin());

            for (std::size_t j = 0; j < chunk_size; ++j) {
                const auto& update = buffer[j];
                benchmark::DoNotOptimize(&update.symbol_id);
                benchmark::DoNotOptimize(&update.price);
                benchmark::DoNotOptimize(&update.volume);
            }
        }
    }
}
```

### Benchmark Results
- **Debug Build:** ~10,729,308 ns
- **Release Build:** ~2,615,618 ns

### Explanation
The overhead of copying data into the buffer likely outweighs the benefits, especially for scenarios with large data volumes. However, other use cases not covered here might benefit from this approach.

---

## Prefetching: Observation-Based Results
Prefetching aims to load data into the cache proactively. However, our tests revealed limited efficiency, likely due to its implementation overhead. This technique might excel in other contexts not explored here.

### Prefetching Implementation
```cpp
void ProcessWithPrefetching(benchmark::State &state) {
    const auto &updates = SimulatedInputData;

    for (auto _ : state) {
        auto it = updates.begin();
        auto end = updates.end();

        while (it != end) {
            std::size_t count = std::min(static_cast<size_t>(std::distance(it, end)), 64);
            auto next_it = std::next(it, count);
            for (auto prefetch_it = it; prefetch_it != next_it; ++prefetch_it) {
                __builtin_prefetch(&(*prefetch_it), 0, 3);
            }

            for (auto current = it; current != next_it; ++current) {
                benchmark::DoNotOptimize(&current->symbol_id);
                benchmark::DoNotOptimize(&current->price);
                benchmark::DoNotOptimize(&current->volume);
            }

            it = next_it;
        }
    }
}
```

### Benchmark Results
- **Debug Build:** ~26,007,952 ns
- **Release Build:** ~1,713,034 ns

### Explanation
The extra loop and explicit prefetching calls may negate potential benefits. Further profiling is needed to validate these observations. Prefetching might perform better in different scenarios.

---

## Conclusion
Through iterative optimizations, we observed:

1. Data alignment and rearrangement significantly improved performance.
2. Compiler optimizations in release builds often rival manual techniques.
3. Buffering and prefetching are scenario-dependent and require careful implementation.

Future work should include deeper profiling to validate assumptions and exploring more advanced compiler optimizations.

---

*Note: Replace placeholder diagram links with actual visuals if needed.*



# unloop benchmark results
ppatoria@ux:~/dev/interview/cache/benchmark$ ./cpu_turbo.sh disable; taskset -c 2 ./build/cache_benchmark_unrolling_loop --benchmark_min_time=1.0s --benchmark_repetitions=10 --benchmark_counters_tabular=true
Disabling Turbo Boost and setting performance governor...
1
2025-01-21T02:03:57-05:00
Running ./build/cache_benchmark_unrolling_loop
Run on (4 X 3300 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 3072 KiB (x1)
Load Average: 0.73, 1.08, 1.02
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------------------------------------------
Benchmark                                                                        Time             CPU   Iterations
------------------------------------------------------------------------------------------------------------------
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            547642 ns       545896 ns         2591
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            537190 ns       536745 ns         2591
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            544217 ns       543860 ns         2591
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            538002 ns       537707 ns         2591
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            542799 ns       542466 ns         2591
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            540629 ns       540301 ns         2591
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            546709 ns       546350 ns         2591
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            573019 ns       564686 ns         2591
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            543773 ns       543454 ns         2591
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            543750 ns       543421 ns         2591
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_mean       545773 ns       544489 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_median     543761 ns       543437 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_stddev      10147 ns         7772 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_cv           1.86 %          1.43 %            10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            542016 ns       541701 ns         2622
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            539279 ns       538918 ns         2622
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            541881 ns       541520 ns         2622
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            538448 ns       538130 ns         2622
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            538201 ns       537868 ns         2622
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            539907 ns       539542 ns         2622
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            539112 ns       538816 ns         2622
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            581549 ns       581085 ns         2622
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            574328 ns       573908 ns         2622
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            536668 ns       536343 ns         2622
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_mean       547139 ns       546783 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_median     539593 ns       539230 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_stddev      16400 ns        16354 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_cv           3.00 %          2.99 %            10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              559040 ns       558678 ns         2481
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              559127 ns       558791 ns         2481
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              563819 ns       563409 ns         2481
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              569746 ns       569435 ns         2481
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              561627 ns       561290 ns         2481
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              562269 ns       561928 ns         2481
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              559911 ns       559549 ns         2481
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              571490 ns       571034 ns         2481
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              561469 ns       561120 ns         2481
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              564123 ns       563780 ns         2481
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_mean         563262 ns       562901 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_median       561948 ns       561609 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_stddev         4266 ns         4250 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_cv             0.76 %          0.75 %            10
ppatoria@ux:~/dev/interview/cache/benchmark$ 

# Default (no aligned no arranged data no aligned data structure)
ppatoria@ux:~/dev/interview/cache/benchmark$ ./cpu_turbo.sh disable; taskset -c 2 ./build/default --benchmark_min_time=1.0s --benchmark_repetitions=10 --benchmark_counters_tabular=true
Disabling Turbo Boost and setting performance governor...
1
2025-01-21T02:24:08-05:00
Running ./build/default
Run on (4 X 3300 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 3072 KiB (x1)
Load Average: 0.64, 0.77, 0.83
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------
Benchmark                         Time             CPU   Iterations
-------------------------------------------------------------------
ProcessMarketData            564352 ns       564016 ns         2502
ProcessMarketData            564075 ns       563721 ns         2502
ProcessMarketData            562268 ns       561942 ns         2502
ProcessMarketData            564908 ns       564563 ns         2502
ProcessMarketData            565412 ns       565066 ns         2502
ProcessMarketData            560302 ns       559922 ns         2502
ProcessMarketData            565223 ns       564901 ns         2502
ProcessMarketData            562583 ns       562266 ns         2502
ProcessMarketData            564614 ns       564288 ns         2502
ProcessMarketData            563034 ns       562698 ns         2502
ProcessMarketData_mean       563677 ns       563338 ns           10
ProcessMarketData_median     564213 ns       563869 ns           10
ProcessMarketData_stddev       1612 ns         1620 ns           10
ProcessMarketData_cv           0.29 %          0.29 %            10
ppatoria@ux:~/dev/interview/cache/benchmark$ 
# Aligned Data 
ppatoria@ux:~/dev/interview/cache/benchmark/build$ ./cpu_turbo.sh disable; taskset -c 2 ./aligned_data --benchmark_min_time=1.0s --benchmark_repetitions=10 --benchmark_counters_tabular=true
bash: ./cpu_turbo.sh: No such file or directory
2025-01-21T03:35:43-05:00
Running ./aligned_data
Run on (4 X 3300 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 3072 KiB (x1)
Load Average: 0.62, 0.46, 0.40
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------------------------------
Benchmark                                                    Time             CPU   Iterations
----------------------------------------------------------------------------------------------
ProcessAlignedDataNonAlignedVectorMarketData            567139 ns       566806 ns         2497
ProcessAlignedDataNonAlignedVectorMarketData            568114 ns       567778 ns         2497
ProcessAlignedDataNonAlignedVectorMarketData            560345 ns       560017 ns         2497
ProcessAlignedDataNonAlignedVectorMarketData            562517 ns       562202 ns         2497
ProcessAlignedDataNonAlignedVectorMarketData            561366 ns       561033 ns         2497
ProcessAlignedDataNonAlignedVectorMarketData            565390 ns       565042 ns         2497
ProcessAlignedDataNonAlignedVectorMarketData            562679 ns       562363 ns         2497
ProcessAlignedDataNonAlignedVectorMarketData            564199 ns       563843 ns         2497
ProcessAlignedDataNonAlignedVectorMarketData            563256 ns       562915 ns         2497
ProcessAlignedDataNonAlignedVectorMarketData            566714 ns       566369 ns         2497
ProcessAlignedDataNonAlignedVectorMarketData_mean       564172 ns       563837 ns           10
ProcessAlignedDataNonAlignedVectorMarketData_median     563728 ns       563379 ns           10
ProcessAlignedDataNonAlignedVectorMarketData_stddev       2595 ns         2590 ns           10
ProcessAlignedDataNonAlignedVectorMarketData_cv           0.46 %          0.46 %            10
ProcessAlignedDataAlignedVectorMarketData               578348 ns       577896 ns         2450
ProcessAlignedDataAlignedVectorMarketData               559267 ns       558948 ns         2450
ProcessAlignedDataAlignedVectorMarketData               563215 ns       562872 ns         2450
ProcessAlignedDataAlignedVectorMarketData               562318 ns       561978 ns         2450
ProcessAlignedDataAlignedVectorMarketData               565753 ns       565428 ns         2450
ProcessAlignedDataAlignedVectorMarketData               562195 ns       561880 ns         2450
ProcessAlignedDataAlignedVectorMarketData               561139 ns       560776 ns         2450
ProcessAlignedDataAlignedVectorMarketData               561120 ns       560777 ns         2450
ProcessAlignedDataAlignedVectorMarketData               563396 ns       563077 ns         2450
ProcessAlignedDataAlignedVectorMarketData               562703 ns       562367 ns         2450
ProcessAlignedDataAlignedVectorMarketData_mean          563945 ns       563600 ns           10
ProcessAlignedDataAlignedVectorMarketData_median        562510 ns       562173 ns           10
ProcessAlignedDataAlignedVectorMarketData_stddev          5339 ns         5304 ns           10
ProcessAlignedDataAlignedVectorMarketData_cv              0.95 %          0.94 %            10
ProcessAlignedDataAlignedArrayMarketData                561733 ns       561410 ns         2491
ProcessAlignedDataAlignedArrayMarketData                561229 ns       560865 ns         2491
ProcessAlignedDataAlignedArrayMarketData                565639 ns       565251 ns         2491
ProcessAlignedDataAlignedArrayMarketData                562160 ns       561799 ns         2491
ProcessAlignedDataAlignedArrayMarketData                562240 ns       561808 ns         2491
ProcessAlignedDataAlignedArrayMarketData                566147 ns       565741 ns         2491
ProcessAlignedDataAlignedArrayMarketData                567204 ns       566850 ns         2491
ProcessAlignedDataAlignedArrayMarketData                566638 ns       566285 ns         2491
ProcessAlignedDataAlignedArrayMarketData                560808 ns       560436 ns         2491
ProcessAlignedDataAlignedArrayMarketData                559706 ns       559338 ns         2491
ProcessAlignedDataAlignedArrayMarketData_mean           563350 ns       562978 ns           10
ProcessAlignedDataAlignedArrayMarketData_median         562200 ns       561804 ns           10
ProcessAlignedDataAlignedArrayMarketData_stddev           2753 ns         2751 ns           10
ProcessAlignedDataAlignedArrayMarketData_cv               0.49 %          0.49 %            10
ppatoria@ux:~/dev/interview/cache/benchmark/build$ 
# Arranged Data
ppatoria@ux:~/dev/interview/cache/benchmark/build$ export CXX=/bin/g++ ; export CC=/bin/gcc; cd ~/dev/interview/cache/benchmark; rm -rf build; mkdir build; cd build/; cmake -D CMAKE_BUILD_TYPE=Release ../; make run_cache_benchmark_unrolling_loop
-- The C compiler identification is GNU 13.1.0
-- The CXX compiler identification is GNU 13.1.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /bin/gcc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /bin/g++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Success
-- Found Threads: TRUE  
-- Build type: Release
-- C++ Flags: -O3 -DNDEBUG -funroll-loops
-- C++ Flags Release: -O3 -DNDEBUG -funroll-loops
-- Configuring done
-- Generating done
-- Build files have been written to: /home/ppatoria/dev/interview/cache/benchmark/build
[ 33%] Building CXX object CMakeFiles/cache_benchmark_unrolling_loop.dir/unrolling_loop.cpp.o
[ 66%] Linking CXX executable cache_benchmark_unrolling_loop
[ 66%] Built target cache_benchmark_unrolling_loop
[100%] Running benchmark for cache_benchmark_unrolling_loop with specific CPU settings
1
2025-01-21T01:50:29-05:00
Running ./cache_benchmark_unrolling_loop
Run on (4 X 3300 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 3072 KiB (x1)
Load Average: 1.45, 1.06, 0.86
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-- LOG(0): Value passed to --benchmark_min_time should have a suffix. Eg., `30s` for 30-seconds.-- LOG(0): Value passed to --benchmark_min_time should have a suffix. Eg., `30s` for 30-seconds.-- LOG(0): Value passed to --benchmark_min_time should have a suffix. Eg., `30s` for 30-seconds.------------------------------------------------------------------------------------------------------------------
Benchmark                                                                        Time             CPU   Iterations
------------------------------------------------------------------------------------------------------------------
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            541834 ns       541418 ns         2532
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            539141 ns       538846 ns         2532
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            541927 ns       541583 ns         2532
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            546659 ns       546295 ns         2532
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            541848 ns       541506 ns         2532
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            541928 ns       541573 ns         2532
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            539564 ns       539252 ns         2532
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            538635 ns       538276 ns         2532
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            535780 ns       535473 ns         2532
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            544352 ns       543934 ns         2532
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_mean       541167 ns       540816 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_median     541841 ns       541462 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_stddev       3064 ns         3038 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_cv           0.57 %          0.56 %            10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            536112 ns       535790 ns         2581
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            535470 ns       535161 ns         2581
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            543135 ns       542831 ns         2581
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            538192 ns       537881 ns         2581
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            541043 ns       540718 ns         2581
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            549108 ns       548745 ns         2581
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            537001 ns       536692 ns         2581
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            539557 ns       539221 ns         2581
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            540939 ns       540598 ns         2581
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            538793 ns       538473 ns         2581
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_mean       539935 ns       539611 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_median     539175 ns       538847 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_stddev       4004 ns         3991 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_cv           0.74 %          0.74 %            10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              567263 ns       566840 ns         2443
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              572249 ns       571825 ns         2443
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              567599 ns       567228 ns         2443
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              566521 ns       566120 ns         2443
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              558266 ns       557949 ns         2443
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              559643 ns       559302 ns         2443
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              566315 ns       565967 ns         2443
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              567243 ns       566891 ns         2443
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              559495 ns       559147 ns         2443
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              561429 ns       561078 ns         2443
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_mean         564602 ns       564235 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_median       566418 ns       566044 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_stddev         4583 ns         4554 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_cv             0.81 %          0.81 %            10
0
[100%] Built target run_cache_benchmark_unrolling_loop
ppatoria@ux:~/dev/interview/cache/benchmark/build$ export CXX=/bin/g++ ; export CC=/bin/gcc; cd ~/dev/interview/cache/benchmark; rm -rf build; mkdir build; cd build/; cmake -D CMAKE_BUILD_TYPE=Release ../; make run_cache_benchmark_unrolling_loop
-- The C compiler identification is GNU 13.1.0
-- The CXX compiler identification is GNU 13.1.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /bin/gcc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /bin/g++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Success
-- Found Threads: TRUE  
-- Build type: Release
-- C++ Flags: 
-- C++ Flags Release: -O3 -DNDEBUG -funroll-loops
-- Configuring done
-- Generating done
-- Build files have been written to: /home/ppatoria/dev/interview/cache/benchmark/build
[ 33%] Building CXX object CMakeFiles/cache_benchmark_unrolling_loop.dir/unrolling_loop.cpp.o
[ 66%] Linking CXX executable cache_benchmark_unrolling_loop
[ 66%] Built target cache_benchmark_unrolling_loop
[100%] Running benchmark for cache_benchmark_unrolling_loop with specific CPU settings
1
2025-01-21T01:52:18-05:00
Running ./cache_benchmark_unrolling_loop
Run on (4 X 3300 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 3072 KiB (x1)
Load Average: 0.74, 0.95, 0.85
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-- LOG(0): Value passed to --benchmark_min_time should have a suffix. Eg., `30s` for 30-seconds.-- LOG(0): Value passed to --benchmark_min_time should have a suffix. Eg., `30s` for 30-seconds.-- LOG(0): Value passed to --benchmark_min_time should have a suffix. Eg., `30s` for 30-seconds.------------------------------------------------------------------------------------------------------------------
Benchmark                                                                        Time             CPU   Iterations
------------------------------------------------------------------------------------------------------------------
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            543465 ns       543123 ns         2588
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            545089 ns       544778 ns         2588
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            560883 ns       560437 ns         2588
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            552515 ns       552130 ns         2588
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            544654 ns       544319 ns         2588
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            541652 ns       541330 ns         2588
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            544794 ns       544451 ns         2588
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            546441 ns       546088 ns         2588
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            537090 ns       536795 ns         2588
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            545714 ns       545380 ns         2588
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_mean       546230 ns       545883 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_median     544941 ns       544614 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_stddev       6434 ns         6393 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_cv           1.18 %          1.17 %            10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            541808 ns       541493 ns         2586
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            543923 ns       543616 ns         2586
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            542682 ns       542300 ns         2586
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            538201 ns       537837 ns         2586
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            564843 ns       564432 ns         2586
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            537698 ns       537359 ns         2586
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            547830 ns       547421 ns         2586
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            566200 ns       565672 ns         2586
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            546578 ns       546172 ns         2586
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            555070 ns       554605 ns         2586
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_mean       548483 ns       548091 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_median     545250 ns       544894 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_stddev      10274 ns        10220 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_cv           1.87 %          1.86 %            10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              572622 ns       572010 ns         2459
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              578835 ns       578370 ns         2459
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              575766 ns       575254 ns         2459
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              579979 ns       579331 ns         2459
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              578053 ns       577431 ns         2459
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              564809 ns       564425 ns         2459
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              572651 ns       572123 ns         2459
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              589182 ns       588483 ns         2459
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              597569 ns       596888 ns         2459
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              577267 ns       576639 ns         2459
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_mean         578673 ns       578096 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_median       577660 ns       577035 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_stddev         9099 ns         9024 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_cv             1.57 %          1.56 %            10
0
[100%] Built target run_cache_benchmark_unrolling_loop
ppatoria@ux:~/dev/interview/cache/benchmark/build$ sudo cpufreq-set -g performance
ppatoria@ux:~/dev/interview/cache/benchmark/build$ echo 1 | sudo tee /sys/devices/system/cpu/intel_pstate/no_turbo
1
ppatoria@ux:~/dev/interview/cache/benchmark/build$ export CXX=/bin/g++ ; export CC=/bin/gcc; cd ~/dev/interview/cache/benchmark; rm -rf build; mkdir build; cd build/; cmake -D CMAKE_BUILD_TYPE=Release ../; make;
-- The C compiler identification is GNU 13.1.0
-- The CXX compiler identification is GNU 13.1.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /bin/gcc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /bin/g++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Success
-- Found Threads: TRUE  
-- Build type: Release
-- C++ Flags: 
-- C++ Flags Release: -O3 -DNDEBUG -funroll-loops
-- Configuring done
-- Generating done
-- Build files have been written to: /home/ppatoria/dev/interview/cache/benchmark/build
[ 16%] Building CXX object CMakeFiles/cache_benchmark.dir/benchmark.cpp.o
[ 33%] Linking CXX executable cache_benchmark
[ 33%] Built target cache_benchmark
[ 50%] Building CXX object CMakeFiles/cache_benchmark_aligned_data.dir/benchmark_aligned_data.cpp.o
[ 66%] Linking CXX executable cache_benchmark_aligned_data
[ 66%] Built target cache_benchmark_aligned_data
[ 83%] Building CXX object CMakeFiles/cache_benchmark_unrolling_loop.dir/unrolling_loop.cpp.o
[100%] Linking CXX executable cache_benchmark_unrolling_loop
[100%] Built target cache_benchmark_unrolling_loop
ppatoria@ux:~/dev/interview/cache/benchmark/build$ taskset -c 2 ./build/cache_benchmark_unrolling_loop --benchmark_min_time=1.0 --benchmark_repetitions=10 --benchmark_counters_tabular=true
taskset: failed to execute ./build/cache_benchmark_unrolling_loop: No such file or directory
ppatoria@ux:~/dev/interview/cache/benchmark/build$ taskset -c 2 ./cache_benchmark_unrolling_loop --benchmark_min_time=1.0 --benchmark_repetitions=10 --benchmark_counters_tabular=true
2025-01-21T01:55:56-05:00
Running ./cache_benchmark_unrolling_loop
Run on (4 X 3300 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 3072 KiB (x1)
Load Average: 0.77, 0.96, 0.89
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-- LOG(0): Value passed to --benchmark_min_time should have a suffix. Eg., `30s` for 30-seconds.-- LOG(0): Value passed to --benchmark_min_time should have a suffix. Eg., `30s` for 30-seconds.-- LOG(0): Value passed to --benchmark_min_time should have a suffix. Eg., `30s` for 30-seconds.------------------------------------------------------------------------------------------------------------------
Benchmark                                                                        Time             CPU   Iterations
------------------------------------------------------------------------------------------------------------------
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            545272 ns       544948 ns         2590
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            549189 ns       548854 ns         2590
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            540388 ns       540076 ns         2590
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            547318 ns       546926 ns         2590
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            542416 ns       542069 ns         2590
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            544625 ns       544277 ns         2590
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            553546 ns       553186 ns         2590
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            543241 ns       542905 ns         2590
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            544049 ns       543710 ns         2590
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            542199 ns       541852 ns         2590
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_mean       545224 ns       544880 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_median     544337 ns       543993 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_stddev       3879 ns         3870 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_cv           0.71 %          0.71 %            10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            537697 ns       537366 ns         2613
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            537675 ns       537345 ns         2613
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            538284 ns       537952 ns         2613
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            541155 ns       540801 ns         2613
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            540207 ns       539913 ns         2613
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            570769 ns       570389 ns         2613
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            543987 ns       543649 ns         2613
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            543340 ns       542994 ns         2613
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            539503 ns       539168 ns         2613
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            542715 ns       542321 ns         2613
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_mean       543533 ns       543190 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_median     540681 ns       540357 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_stddev       9841 ns         9826 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_cv           1.81 %          1.81 %            10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              561951 ns       561584 ns         2499
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              566398 ns       566049 ns         2499
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              561449 ns       561103 ns         2499
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              559773 ns       559421 ns         2499
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              566654 ns       566319 ns         2499
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              561947 ns       561602 ns         2499
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              561690 ns       561312 ns         2499
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              559450 ns       559082 ns         2499
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              565043 ns       564656 ns         2499
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              562129 ns       561809 ns         2499
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_mean         562648 ns       562294 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_median       561949 ns       561593 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_stddev         2537 ns         2540 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_cv             0.45 %          0.45 %            10
ppatoria@ux:~/dev/interview/cache/benchmark/build$ taskset -c 2 ./cache_benchmark_unrolling_loop --benchmark_min_time=1.0 --benchmark_repetitions=10 --benchmark_counters_tabular=true
2025-01-21T01:57:14-05:00
Running ./cache_benchmark_unrolling_loop
Run on (4 X 3300 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 3072 KiB (x1)
Load Average: 1.00, 1.00, 0.91
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-- LOG(0): Value passed to --benchmark_min_time should have a suffix. Eg., `30s` for 30-seconds.-- LOG(0): Value passed to --benchmark_min_time should have a suffix. Eg., `30s` for 30-seconds.-- LOG(0): Value passed to --benchmark_min_time should have a suffix. Eg., `30s` for 30-seconds.------------------------------------------------------------------------------------------------------------------
Benchmark                                                                        Time             CPU   Iterations
------------------------------------------------------------------------------------------------------------------
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            536717 ns       536342 ns         2609
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            547699 ns       547006 ns         2609
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            544382 ns       544059 ns         2609
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            547205 ns       546880 ns         2609
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            536838 ns       536505 ns         2609
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            543043 ns       542696 ns         2609
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            545642 ns       545318 ns         2609
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            562212 ns       561656 ns         2609
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            541001 ns       540655 ns         2609
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            537092 ns       536762 ns         2609
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_mean       544183 ns       543788 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_median     543712 ns       543378 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_stddev       7602 ns         7531 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_cv           1.40 %          1.38 %            10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            540250 ns       539913 ns         2614
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            548778 ns       548403 ns         2614
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            545137 ns       544817 ns         2614
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            537231 ns       536891 ns         2614
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            547832 ns       547483 ns         2614
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            552938 ns       552461 ns         2614
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            601231 ns       593255 ns         2614
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            580359 ns       572716 ns         2614
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            560796 ns       555547 ns         2614
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            588773 ns       580139 ns         2614
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_mean       560332 ns       557163 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_median     550858 ns       550432 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_stddev      22099 ns        18644 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_cv           3.94 %          3.35 %            10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              573080 ns       565692 ns         2450
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              585064 ns       577820 ns         2450
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              600544 ns       593544 ns         2450
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              568287 ns       565071 ns         2450
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              573510 ns       569851 ns         2450
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              561955 ns       559336 ns         2450
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              561304 ns       560389 ns         2450
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              591393 ns       590608 ns         2450
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              563387 ns       562982 ns         2450
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              561959 ns       561532 ns         2450
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_mean         574048 ns       570683 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_median       570683 ns       565382 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_stddev        13853 ns        12496 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_cv             2.41 %          2.19 %            10
ppatoria@ux:~/dev/interview/cache/benchmark/build$ taskset -c 2 ./cache_benchmark_unrolling_loop --benchmark_min_time=1.0s --benchmark_repetitions=10 --benchmark_counters_tabular=true
2025-01-21T01:59:11-05:00
Running ./cache_benchmark_unrolling_loop
Run on (4 X 3300 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 3072 KiB (x1)
Load Average: 1.17, 1.08, 0.96
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------------------------------------------
Benchmark                                                                        Time             CPU   Iterations
------------------------------------------------------------------------------------------------------------------
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            545363 ns       544996 ns         2551
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            539614 ns       539290 ns         2551
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            536974 ns       536663 ns         2551
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            537869 ns       537527 ns         2551
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            541248 ns       540910 ns         2551
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            539123 ns       538793 ns         2551
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            536898 ns       536572 ns         2551
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            537851 ns       537505 ns         2551
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            547753 ns       547368 ns         2551
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            552962 ns       552157 ns         2551
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_mean       541565 ns       541178 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_median     539368 ns       539042 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_stddev       5404 ns         5283 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_cv           1.00 %          0.98 %            10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            614516 ns       612001 ns         2463
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            547809 ns       547433 ns         2463
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            553944 ns       553521 ns         2463
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            547753 ns       547295 ns         2463
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            537367 ns       537020 ns         2463
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            549554 ns       548784 ns         2463
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            541350 ns       540987 ns         2463
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            548677 ns       548281 ns         2463
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            569948 ns       569228 ns         2463
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            543834 ns       543366 ns         2463
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_mean       555475 ns       554792 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_median     548243 ns       547857 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_stddev      22510 ns        21880 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_cv           4.05 %          3.94 %            10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              574693 ns       574169 ns         2455
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              589177 ns       588601 ns         2455
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              583779 ns       583107 ns         2455
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              567729 ns       567161 ns         2455
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              570719 ns       570151 ns         2455
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              570580 ns       570023 ns         2455
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              581077 ns       580476 ns         2455
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              568516 ns       568020 ns         2455
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              570844 ns       570149 ns         2455
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              572913 ns       572278 ns         2455
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_mean         575003 ns       574413 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_median       571878 ns       571215 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_stddev         7223 ns         7206 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_cv             1.26 %          1.25 %            10
ppatoria@ux:~/dev/interview/cache/benchmark/build$ chmod +x cpu_turbo.sh
chmod: cannot access 'cpu_turbo.sh': No such file or directory
ppatoria@ux:~/dev/interview/cache/benchmark/build$ cd ..
ppatoria@ux:~/dev/interview/cache/benchmark$ chmod +x cpu_turbo.sh
ppatoria@ux:~/dev/interview/cache/benchmark$ ./cpu_turbo.sh disable; taskset -c 2 ./build/cache_benchmark_unrolling_loop --benchmark_min_time=1.0s --benchmark_repetitions=10 --benchmark_counters_tabular=true
Disabling Turbo Boost and setting performance governor...
1
2025-01-21T02:03:57-05:00
Running ./build/cache_benchmark_unrolling_loop
Run on (4 X 3300 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 3072 KiB (x1)
Load Average: 0.73, 1.08, 1.02
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------------------------------------------
Benchmark                                                                        Time             CPU   Iterations
------------------------------------------------------------------------------------------------------------------
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            547642 ns       545896 ns         2591
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            537190 ns       536745 ns         2591
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            544217 ns       543860 ns         2591
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            538002 ns       537707 ns         2591
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            542799 ns       542466 ns         2591
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            540629 ns       540301 ns         2591
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            546709 ns       546350 ns         2591
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            573019 ns       564686 ns         2591
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            543773 ns       543454 ns         2591
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop            543750 ns       543421 ns         2591
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_mean       545773 ns       544489 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_median     543761 ns       543437 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_stddev      10147 ns         7772 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop_cv           1.86 %          1.43 %            10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            542016 ns       541701 ns         2622
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            539279 ns       538918 ns         2622
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            541881 ns       541520 ns         2622
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            538448 ns       538130 ns         2622
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            538201 ns       537868 ns         2622
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            539907 ns       539542 ns         2622
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            539112 ns       538816 ns         2622
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            581549 ns       581085 ns         2622
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            574328 ns       573908 ns         2622
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop            536668 ns       536343 ns         2622
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_mean       547139 ns       546783 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_median     539593 ns       539230 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_stddev      16400 ns        16354 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop_cv           3.00 %          2.99 %            10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              559040 ns       558678 ns         2481
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              559127 ns       558791 ns         2481
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              563819 ns       563409 ns         2481
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              569746 ns       569435 ns         2481
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              561627 ns       561290 ns         2481
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              562269 ns       561928 ns         2481
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              559911 ns       559549 ns         2481
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              571490 ns       571034 ns         2481
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              561469 ns       561120 ns         2481
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled              564123 ns       563780 ns         2481
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_mean         563262 ns       562901 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_median       561948 ns       561609 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_stddev         4266 ns         4250 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled_cv             0.76 %          0.75 %            10
ppatoria@ux:~/dev/interview/cache/benchmark$ ./cpu_turbo.sh disable; taskset -c 2 ./build/cache_benchmark_aligned_data --benchmark_min_time=1.0s --benchmark_repetitions=10 --benchmark_counters_tabular=true
Disabling Turbo Boost and setting performance governor...
1
2025-01-21T02:12:48-05:00
Running ./build/cache_benchmark_aligned_data
Run on (4 X 3300 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 3072 KiB (x1)
Load Average: 0.55, 0.67, 0.85
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------------------------
Benchmark                                                               Time             CPU   Iterations
---------------------------------------------------------------------------------------------------------
ProcessMarketData                                                  561486 ns       561147 ns         2477
ProcessMarketData                                                  563251 ns       562925 ns         2477
ProcessMarketData                                                  561168 ns       560856 ns         2477
ProcessMarketData                                                  592914 ns       592414 ns         2477
ProcessMarketData                                                  568092 ns       567762 ns         2477
ProcessMarketData                                                  570961 ns       570605 ns         2477
ProcessMarketData                                                  565840 ns       565496 ns         2477
ProcessMarketData                                                  566634 ns       566305 ns         2477
ProcessMarketData                                                  561748 ns       561405 ns         2477
ProcessMarketData                                                  561055 ns       560695 ns         2477
ProcessMarketData_mean                                             567315 ns       566961 ns           10
ProcessMarketData_median                                           564546 ns       564210 ns           10
ProcessMarketData_stddev                                             9605 ns         9556 ns           10
ProcessMarketData_cv                                                 1.69 %          1.69 %            10
ProcessAlignedDataAlignedVectorMarketData                          565136 ns       564809 ns         2509
ProcessAlignedDataAlignedVectorMarketData                          561165 ns       560794 ns         2509
ProcessAlignedDataAlignedVectorMarketData                          562748 ns       562370 ns         2509
ProcessAlignedDataAlignedVectorMarketData                          567523 ns       567168 ns         2509
ProcessAlignedDataAlignedVectorMarketData                          559955 ns       559614 ns         2509
ProcessAlignedDataAlignedVectorMarketData                          570922 ns       570489 ns         2509
ProcessAlignedDataAlignedVectorMarketData                          565935 ns       565552 ns         2509
ProcessAlignedDataAlignedVectorMarketData                          565330 ns       564979 ns         2509
ProcessAlignedDataAlignedVectorMarketData                          567039 ns       566579 ns         2509
ProcessAlignedDataAlignedVectorMarketData                          571380 ns       570959 ns         2509
ProcessAlignedDataAlignedVectorMarketData_mean                     565713 ns       565331 ns           10
ProcessAlignedDataAlignedVectorMarketData_median                   565632 ns       565265 ns           10
ProcessAlignedDataAlignedVectorMarketData_stddev                     3763 ns         3737 ns           10
ProcessAlignedDataAlignedVectorMarketData_cv                         0.67 %          0.66 %            10
ProcessAlignedDataAlignedArrayMarketData                           569792 ns       569398 ns         2475
ProcessAlignedDataAlignedArrayMarketData                           564913 ns       564567 ns         2475
ProcessAlignedDataAlignedArrayMarketData                           567847 ns       567397 ns         2475
ProcessAlignedDataAlignedArrayMarketData                           569931 ns       569481 ns         2475
ProcessAlignedDataAlignedArrayMarketData                           562230 ns       561885 ns         2475
ProcessAlignedDataAlignedArrayMarketData                           563826 ns       563491 ns         2475
ProcessAlignedDataAlignedArrayMarketData                           564846 ns       564501 ns         2475
ProcessAlignedDataAlignedArrayMarketData                           565324 ns       564950 ns         2475
ProcessAlignedDataAlignedArrayMarketData                           564128 ns       563781 ns         2475
ProcessAlignedDataAlignedArrayMarketData                           561794 ns       561451 ns         2475
ProcessAlignedDataAlignedArrayMarketData_mean                      565463 ns       565090 ns           10
ProcessAlignedDataAlignedArrayMarketData_median                    564880 ns       564534 ns           10
ProcessAlignedDataAlignedArrayMarketData_stddev                      2857 ns         2820 ns           10
ProcessAlignedDataAlignedArrayMarketData_cv                          0.51 %          0.50 %            10
ProcessArrangedDataAlignedVectorMarketData                         558913 ns       558574 ns         2505
ProcessArrangedDataAlignedVectorMarketData                         562770 ns       562360 ns         2505
ProcessArrangedDataAlignedVectorMarketData                         565877 ns       565494 ns         2505
ProcessArrangedDataAlignedVectorMarketData                         582426 ns       581957 ns         2505
ProcessArrangedDataAlignedVectorMarketData                         560963 ns       560585 ns         2505
ProcessArrangedDataAlignedVectorMarketData                         560379 ns       560019 ns         2505
ProcessArrangedDataAlignedVectorMarketData                         574635 ns       574260 ns         2505
ProcessArrangedDataAlignedVectorMarketData                         564071 ns       563747 ns         2505
ProcessArrangedDataAlignedVectorMarketData                         562814 ns       562451 ns         2505
ProcessArrangedDataAlignedVectorMarketData                         566186 ns       565864 ns         2505
ProcessArrangedDataAlignedVectorMarketData_mean                    565904 ns       565531 ns           10
ProcessArrangedDataAlignedVectorMarketData_median                  563443 ns       563099 ns           10
ProcessArrangedDataAlignedVectorMarketData_stddev                    7271 ns         7243 ns           10
ProcessArrangedDataAlignedVectorMarketData_cv                        1.28 %          1.28 %            10
ProcessArrangedDataAlignedArrayMarketData                          558995 ns       558670 ns         2507
ProcessArrangedDataAlignedArrayMarketData                          558406 ns       558100 ns         2507
ProcessArrangedDataAlignedArrayMarketData                          561030 ns       560710 ns         2507
ProcessArrangedDataAlignedArrayMarketData                          563659 ns       563329 ns         2507
ProcessArrangedDataAlignedArrayMarketData                          559019 ns       558710 ns         2507
ProcessArrangedDataAlignedArrayMarketData                          563354 ns       563011 ns         2507
ProcessArrangedDataAlignedArrayMarketData                          569691 ns       569365 ns         2507
ProcessArrangedDataAlignedArrayMarketData                          579501 ns       579022 ns         2507
ProcessArrangedDataAlignedArrayMarketData                          566573 ns       566230 ns         2507
ProcessArrangedDataAlignedArrayMarketData                          570839 ns       570423 ns         2507
ProcessArrangedDataAlignedArrayMarketData_mean                     565107 ns       564757 ns           10
ProcessArrangedDataAlignedArrayMarketData_median                   563506 ns       563170 ns           10
ProcessArrangedDataAlignedArrayMarketData_stddev                     6702 ns         6653 ns           10
ProcessArrangedDataAlignedArrayMarketData_cv                         1.19 %          1.18 %            10
ProcessNonAlignedNonArrangedDataAlignedArrayMarketData             563161 ns       562786 ns         2449
ProcessNonAlignedNonArrangedDataAlignedArrayMarketData             561645 ns       561327 ns         2449
ProcessNonAlignedNonArrangedDataAlignedArrayMarketData             561605 ns       561243 ns         2449
ProcessNonAlignedNonArrangedDataAlignedArrayMarketData             559080 ns       558757 ns         2449
ProcessNonAlignedNonArrangedDataAlignedArrayMarketData             559844 ns       559505 ns         2449
ProcessNonAlignedNonArrangedDataAlignedArrayMarketData             567088 ns       566726 ns         2449
ProcessNonAlignedNonArrangedDataAlignedArrayMarketData             564116 ns       563750 ns         2449
ProcessNonAlignedNonArrangedDataAlignedArrayMarketData             561450 ns       561122 ns         2449
ProcessNonAlignedNonArrangedDataAlignedArrayMarketData             562027 ns       561698 ns         2449
ProcessNonAlignedNonArrangedDataAlignedArrayMarketData             563241 ns       562872 ns         2449
ProcessNonAlignedNonArrangedDataAlignedArrayMarketData_mean        562326 ns       561979 ns           10
ProcessNonAlignedNonArrangedDataAlignedArrayMarketData_median      561836 ns       561512 ns           10
ProcessNonAlignedNonArrangedDataAlignedArrayMarketData_stddev        2259 ns         2246 ns           10
ProcessNonAlignedNonArrangedDataAlignedArrayMarketData_cv            0.40 %          0.40 %            10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoop             785941 ns       785520 ns         1779
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoop             786908 ns       786462 ns         1779
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoop             848492 ns       847897 ns         1779
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoop             793311 ns       792817 ns         1779
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoop             803577 ns       803153 ns         1779
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoop             791934 ns       791520 ns         1779
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoop             798920 ns       798398 ns         1779
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoop             801228 ns       800738 ns         1779
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoop             797244 ns       796740 ns         1779
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoop             785861 ns       785365 ns         1779
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoop_mean        799342 ns       798861 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoop_median      795277 ns       794779 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoop_stddev       18406 ns        18365 ns           10
ProcessArrangedDataAlignedArrayMarketDataUnrollingLoop_cv            2.30 %          2.30 %            10
ProcessArrangedDataAlignedVectorMarketDataUnrollingLoop           1905521 ns      1904418 ns          715
ProcessArrangedDataAlignedVectorMarketDataUnrollingLoop           1939691 ns      1938568 ns          715
ProcessArrangedDataAlignedVectorMarketDataUnrollingLoop           1911415 ns      1910245 ns          715
ProcessArrangedDataAlignedVectorMarketDataUnrollingLoop           1912049 ns      1910832 ns          715
ProcessArrangedDataAlignedVectorMarketDataUnrollingLoop           1904262 ns      1903020 ns          715
ProcessArrangedDataAlignedVectorMarketDataUnrollingLoop           1935855 ns      1934641 ns          715
ProcessArrangedDataAlignedVectorMarketDataUnrollingLoop           1947334 ns      1946115 ns          715
ProcessArrangedDataAlignedVectorMarketDataUnrollingLoop           2166118 ns      2164649 ns          715
ProcessArrangedDataAlignedVectorMarketDataUnrollingLoop           1992626 ns      1991405 ns          715
ProcessArrangedDataAlignedVectorMarketDataUnrollingLoop           1899750 ns      1898676 ns          715
ProcessArrangedDataAlignedVectorMarketDataUnrollingLoop_mean      1951462 ns      1950257 ns           10
ProcessArrangedDataAlignedVectorMarketDataUnrollingLoop_median    1923952 ns      1922737 ns           10
ProcessArrangedDataAlignedVectorMarketDataUnrollingLoop_stddev      80513 ns        80418 ns           10
ProcessArrangedDataAlignedVectorMarketDataUnrollingLoop_cv           4.13 %          4.12 %            10
ppatoria@ux:~/dev/interview/cache/benchmark$ ProcessAlignedDataAlignedArrayMarketData                           569792 ns       569398 ns         2475
bash: ProcessAlignedDataAlignedArrayMarketData: command not found
ppatoria@ux:~/dev/interview/cache/benchmark$ touch default.cpp
ppatoria@ux:~/dev/interview/cache/benchmark$ export CXX=/bin/g++ ; export CC=/bin/gcc; cd ~/dev/interview/cache/benchmark; rm -rf build; mkdir build; cd build/; cmake -D CMAKE_BUILD_TYPE=Release ../; make;
-- The C compiler identification is GNU 13.1.0
-- The CXX compiler identification is GNU 13.1.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /bin/gcc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /bin/g++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Success
-- Found Threads: TRUE  
-- Build type: Release
-- C++ Flags: 
-- C++ Flags Release: -O3 -DNDEBUG -funroll-loops
-- Configuring done
-- Generating done
-- Build files have been written to: /home/ppatoria/dev/interview/cache/benchmark/build
[ 12%] Building CXX object CMakeFiles/cache_benchmark.dir/benchmark.cpp.o
[ 25%] Linking CXX executable cache_benchmark
[ 25%] Built target cache_benchmark
[ 37%] Building CXX object CMakeFiles/cache_benchmark_aligned_data.dir/benchmark_aligned_data.cpp.o
[ 50%] Linking CXX executable cache_benchmark_aligned_data
[ 50%] Built target cache_benchmark_aligned_data
[ 62%] Building CXX object CMakeFiles/cache_benchmark_unrolling_loop.dir/unrolling_loop.cpp.o
[ 75%] Linking CXX executable cache_benchmark_unrolling_loop
[ 75%] Built target cache_benchmark_unrolling_loop
[ 87%] Building CXX object CMakeFiles/default.dir/default.cpp.o
/home/ppatoria/dev/interview/cache/benchmark/default.cpp:4:30: error: ‘MarketData’ was not declared in this scope
    4 | using MDVector = std::vector<MarketData>;
      |                              ^~~~~~~~~~
/home/ppatoria/dev/interview/cache/benchmark/default.cpp:4:40: error: template argument 1 is invalid
    4 | using MDVector = std::vector<MarketData>;
      |                                        ^
/home/ppatoria/dev/interview/cache/benchmark/default.cpp:4:40: error: template argument 2 is invalid
/home/ppatoria/dev/interview/cache/benchmark/default.cpp:4:23: error: ‘<expression error>’ in namespace ‘std’ does not name a type
    4 | using MDVector = std::vector<MarketData>;
      |                       ^~~~~~~~~~~~~~~~~~
/home/ppatoria/dev/interview/cache/benchmark/default.cpp:5:23: error: ‘MDVector’ was not declared in this scope
    5 | static SimulatedInput<MDVector, Volume> inputGenerator(MDVector{});
      |                       ^~~~~~~~
/home/ppatoria/dev/interview/cache/benchmark/default.cpp:5:23: error: ‘MDVector’ was not declared in this scope
/home/ppatoria/dev/interview/cache/benchmark/default.cpp:5:23: error: ‘MDVector’ was not declared in this scope
/home/ppatoria/dev/interview/cache/benchmark/default.cpp:5:23: error: ‘MDVector’ was not declared in this scope
/home/ppatoria/dev/interview/cache/benchmark/default.cpp:5:23: error: ‘MDVector’ was not declared in this scope
/home/ppatoria/dev/interview/cache/benchmark/default.cpp:5:23: error: ‘MDVector’ was not declared in this scope
/home/ppatoria/dev/interview/cache/benchmark/default.cpp:5:23: error: ‘MDVector’ was not declared in this scope
/home/ppatoria/dev/interview/cache/benchmark/default.cpp:5:23: error: ‘MDVector’ was not declared in this scope
/home/ppatoria/dev/interview/cache/benchmark/default.cpp:5:8: error: ‘SimulatedInput’ does not name a type
    5 | static SimulatedInput<MDVector, Volume> inputGenerator(MDVector{});
      |        ^~~~~~~~~~~~~~
/home/ppatoria/dev/interview/cache/benchmark/default.cpp:5:66: error: expected unqualified-id before ‘)’ token
    5 | static SimulatedInput<MDVector, Volume> inputGenerator(MDVector{});
      |                                                                  ^
/home/ppatoria/dev/interview/cache/benchmark/default.cpp:6:34: error: ‘inputGenerator’ was not declared in this scope
    6 | const auto &SimulatedInputData = inputGenerator.get();
      |                                  ^~~~~~~~~~~~~~
make[2]: *** [CMakeFiles/default.dir/build.make:76: CMakeFiles/default.dir/default.cpp.o] Error 1
make[1]: *** [CMakeFiles/Makefile2:171: CMakeFiles/default.dir/all] Error 2
make: *** [Makefile:91: all] Error 2
ppatoria@ux:~/dev/interview/cache/benchmark/build$ export CXX=/bin/g++ ; export CC=/bin/gcc; cd ~/dev/interview/cache/benchmark; rm -rf build; mkdir build; cd build/; cmake -D CMAKE_BUILD_TYPE=Release ../; make;
-- The C compiler identification is GNU 13.1.0
-- The CXX compiler identification is GNU 13.1.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /bin/gcc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /bin/g++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Success
-- Found Threads: TRUE  
-- Build type: Release
-- C++ Flags: 
-- C++ Flags Release: -O3 -DNDEBUG -funroll-loops
-- Configuring done
-- Generating done
-- Build files have been written to: /home/ppatoria/dev/interview/cache/benchmark/build
[ 12%] Building CXX object CMakeFiles/cache_benchmark.dir/benchmark.cpp.o
[ 25%] Linking CXX executable cache_benchmark
[ 25%] Built target cache_benchmark
[ 37%] Building CXX object CMakeFiles/cache_benchmark_aligned_data.dir/benchmark_aligned_data.cpp.o
[ 50%] Linking CXX executable cache_benchmark_aligned_data
[ 50%] Built target cache_benchmark_aligned_data
[ 62%] Building CXX object CMakeFiles/cache_benchmark_unrolling_loop.dir/unrolling_loop.cpp.o
[ 75%] Linking CXX executable cache_benchmark_unrolling_loop
[ 75%] Built target cache_benchmark_unrolling_loop
[ 87%] Building CXX object CMakeFiles/default.dir/default.cpp.o
[100%] Linking CXX executable default
[100%] Built target default
ppatoria@ux:~/dev/interview/cache/benchmark/build$ ./cpu_turbo.sh disable; taskset -c 2 ./build/default --benchmark_min_time=1.0s --benchmark_repetitions=10 --benchmark_counters_tabular=true
bash: ./cpu_turbo.sh: No such file or directory
taskset: failed to execute ./build/default: No such file or directory
ppatoria@ux:~/dev/interview/cache/benchmark/build$ cd ..
ppatoria@ux:~/dev/interview/cache/benchmark$ ./cpu_turbo.sh disable; taskset -c 2 ./build/default --benchmark_min_time=1.0s --benchmark_repetitions=10 --benchmark_counters_tabular=true
Disabling Turbo Boost and setting performance governor...
1
2025-01-21T02:24:08-05:00
Running ./build/default
Run on (4 X 3300 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 3072 KiB (x1)
Load Average: 0.64, 0.77, 0.83
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------
Benchmark                         Time             CPU   Iterations
-------------------------------------------------------------------
ProcessMarketData            564352 ns       564016 ns         2502
ProcessMarketData            564075 ns       563721 ns         2502
ProcessMarketData            562268 ns       561942 ns         2502
ProcessMarketData            564908 ns       564563 ns         2502
ProcessMarketData            565412 ns       565066 ns         2502
ProcessMarketData            560302 ns       559922 ns         2502
ProcessMarketData            565223 ns       564901 ns         2502
ProcessMarketData            562583 ns       562266 ns         2502
ProcessMarketData            564614 ns       564288 ns         2502
ProcessMarketData            563034 ns       562698 ns         2502
ProcessMarketData_mean       563677 ns       563338 ns           10
ProcessMarketData_median     564213 ns       563869 ns           10
ProcessMarketData_stddev       1612 ns         1620 ns           10
ProcessMarketData_cv           0.29 %          0.29 %            10
ppatoria@ux:~/dev/interview/cache/benchmark$ ./cpu_turbo.sh disable; taskset -c 2 ./build/default --benchmark_min_time=1.0s --benchmark_repetitions=10 --benchmark_counters_tabular=true
Disabling Turbo Boost and setting performance governor...
1
2025-01-21T02:28:19-05:00
Running ./build/default
Run on (4 X 3300 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 3072 KiB (x1)
Load Average: 1.22, 0.87, 0.85
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.

  C-c C-c
ppatoria@ux:~/dev/interview/cache/benchmark$ touch aligned_data_non_aligned_datastructure.cpp
ppatoria@ux:~/dev/interview/cache/benchmark$ mv aligned_data_non_aligned_datastructure.cpp aligned_data.cpp
ppatoria@ux:~/dev/interview/cache/benchmark$ export CXX=/bin/g++ ; export CC=/bin/gcc; cd ~/dev/interview/cache/benchmark; rm -rf build; mkdir build; cd build/; cmake -D CMAKE_BUILD_TYPE=Release ../; make;
-- The C compiler identification is GNU 13.1.0
-- The CXX compiler identification is GNU 13.1.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /bin/gcc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /bin/g++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Success
-- Found Threads: TRUE  
-- Build type: Release
-- C++ Flags: 
-- C++ Flags Release: -O3 -DNDEBUG -funroll-loops
-- Configuring done
-- Generating done
-- Build files have been written to: /home/ppatoria/dev/interview/cache/benchmark/build
[ 10%] Building CXX object CMakeFiles/cache_benchmark.dir/benchmark.cpp.o
[ 20%] Linking CXX executable cache_benchmark
[ 20%] Built target cache_benchmark
[ 30%] Building CXX object CMakeFiles/cache_benchmark_aligned_data.dir/benchmark_aligned_data.cpp.o
[ 40%] Linking CXX executable cache_benchmark_aligned_data
[ 40%] Built target cache_benchmark_aligned_data
[ 50%] Building CXX object CMakeFiles/cache_benchmark_unrolling_loop.dir/unrolling_loop.cpp.o
[ 60%] Linking CXX executable cache_benchmark_unrolling_loop
[ 60%] Built target cache_benchmark_unrolling_loop
[ 70%] Building CXX object CMakeFiles/default.dir/default.cpp.o
[ 80%] Linking CXX executable default
[ 80%] Built target default
[ 90%] Building CXX object CMakeFiles/aligned_data.dir/aligned_data.cpp.o
[100%] Linking CXX executable aligned_data
/bin/ld: /usr/lib/gcc/x86_64-linux-gnu/13/../../../x86_64-linux-gnu/Scrt1.o: in function `_start':
(.text+0x1b): undefined reference to `main'
collect2: error: ld returned 1 exit status
make[2]: *** [CMakeFiles/aligned_data.dir/build.make:98: aligned_data] Error 1
make[1]: *** [CMakeFiles/Makefile2:199: CMakeFiles/aligned_data.dir/all] Error 2
make: *** [Makefile:91: all] Error 2
ppatoria@ux:~/dev/interview/cache/benchmark/build$ export CXX=/bin/g++ ; export CC=/bin/gcc; cd ~/dev/interview/cache/benchmark; rm -rf build; mkdir build; cd build/; cmake -D CMAKE_BUILD_TYPE=Release ../; make;
-- The C compiler identification is GNU 13.1.0
-- The CXX compiler identification is GNU 13.1.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /bin/gcc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /bin/g++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Success
-- Found Threads: TRUE  
-- Build type: Release
-- C++ Flags: 
-- C++ Flags Release: -O3 -DNDEBUG -funroll-loops
-- Configuring done
-- Generating done
-- Build files have been written to: /home/ppatoria/dev/interview/cache/benchmark/build
[ 10%] Building CXX object CMakeFiles/cache_benchmark.dir/benchmark.cpp.o
[ 20%] Linking CXX executable cache_benchmark
[ 20%] Built target cache_benchmark
[ 30%] Building CXX object CMakeFiles/cache_benchmark_aligned_data.dir/benchmark_aligned_data.cpp.o
[ 40%] Linking CXX executable cache_benchmark_aligned_data
[ 40%] Built target cache_benchmark_aligned_data
[ 50%] Building CXX object CMakeFiles/cache_benchmark_unrolling_loop.dir/unrolling_loop.cpp.o
[ 60%] Linking CXX executable cache_benchmark_unrolling_loop
[ 60%] Built target cache_benchmark_unrolling_loop
[ 70%] Building CXX object CMakeFiles/default.dir/default.cpp.o
[ 80%] Linking CXX executable default
[ 80%] Built target default
[ 90%] Building CXX object CMakeFiles/aligned_data.dir/aligned_data.cpp.o
[100%] Linking CXX executable aligned_data
[100%] Built target aligned_data
ppatoria@ux:~/dev/interview/cache/benchmark/build$ ./cpu_turbo.sh disable; taskset -c 2 ./build/aligned_data --benchmark_min_time=1.0s --benchmark_repetitions=10 --benchmark_counters_tabular=true
bash: ./cpu_turbo.sh: No such file or directory
taskset: failed to execute ./build/aligned_data: No such file or directory
ppatoria@ux:~/dev/interview/cache/benchmark/build$ cd ..
ppatoria@ux:~/dev/interview/cache/benchmark$ ./cpu_turbo.sh disable; taskset -c 2 ./build/aligned_data --benchmark_min_time=1.0s --benchmark_repetitions=10 --benchmark_counters_tabular=true
Disabling Turbo Boost and setting performance governor...
1
2025-01-21T02:36:58-05:00
Running ./build/aligned_data
Run on (4 X 3300 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 3072 KiB (x1)
Load Average: 0.51, 0.52, 0.69
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------------------
Benchmark                                                Time             CPU   Iterations
------------------------------------------------------------------------------------------
ProcessAlignedDataAlignedArrayMarketData            565197 ns       564786 ns         2501
ProcessAlignedDataAlignedArrayMarketData            572751 ns       571815 ns         2501
ProcessAlignedDataAlignedArrayMarketData            563820 ns       563320 ns         2501
ProcessAlignedDataAlignedArrayMarketData            560288 ns       559957 ns         2501
ProcessAlignedDataAlignedArrayMarketData            569771 ns       569422 ns         2501
ProcessAlignedDataAlignedArrayMarketData            569006 ns       568603 ns         2501
ProcessAlignedDataAlignedArrayMarketData            561220 ns       560936 ns         2501
ProcessAlignedDataAlignedArrayMarketData            565124 ns       564815 ns         2501
ProcessAlignedDataAlignedArrayMarketData            569758 ns       569441 ns         2501
ProcessAlignedDataAlignedArrayMarketData            565810 ns       565462 ns         2501
ProcessAlignedDataAlignedArrayMarketData_mean       566274 ns       565856 ns           10
ProcessAlignedDataAlignedArrayMarketData_median     565503 ns       565139 ns           10
ProcessAlignedDataAlignedArrayMarketData_stddev       3998 ns         3894 ns           10
ProcessAlignedDataAlignedArrayMarketData_cv           0.71 %          0.69 %            10
ppatoria@ux:~/dev/interview/cache/benchmark$ export CXX=/bin/g++ ; export CC=/bin/gcc; cd ~/dev/interview/cache/benchmark; rm -rf build; mkdir build; cd build/; cmake -D CMAKE_BUILD_TYPE=Release ../; make;
-- The C compiler identification is GNU 13.1.0
-- The CXX compiler identification is GNU 13.1.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /bin/gcc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /bin/g++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Success
-- Found Threads: TRUE  
-- Build type: Release
-- C++ Flags: 
-- C++ Flags Release: -O3 -DNDEBUG -funroll-loops
-- Configuring done
-- Generating done
-- Build files have been written to: /home/ppatoria/dev/interview/cache/benchmark/build
[ 10%] Building CXX object CMakeFiles/cache_benchmark.dir/benchmark.cpp.o
[ 20%] Linking CXX executable cache_benchmark
[ 20%] Built target cache_benchmark
[ 30%] Building CXX object CMakeFiles/cache_benchmark_aligned_data.dir/benchmark_aligned_data.cpp.o
[ 40%] Linking CXX executable cache_benchmark_aligned_data
[ 40%] Built target cache_benchmark_aligned_data
[ 50%] Building CXX object CMakeFiles/cache_benchmark_unrolling_loop.dir/unrolling_loop.cpp.o
[ 60%] Linking CXX executable cache_benchmark_unrolling_loop
[ 60%] Built target cache_benchmark_unrolling_loop
[ 70%] Building CXX object CMakeFiles/default.dir/default.cpp.o
[ 80%] Linking CXX executable default
[ 80%] Built target default
[ 90%] Building CXX object CMakeFiles/aligned_data.dir/aligned_data.cpp.o
[100%] Linking CXX executable aligned_data
[100%] Built target aligned_data
ppatoria@ux:~/dev/interview/cache/benchmark/build$ ./cpu_turbo.sh disable; taskset -c 2 ./build/aligned_data --benchmark_min_time=1.0s --benchmark_repetitions=10 --benchmark_counters_tabular=true
bash: ./cpu_turbo.sh: No such file or directory
taskset: failed to execute ./build/aligned_data: No such file or directory
ppatoria@ux:~/dev/interview/cache/benchmark/build$ ./cpu_turbo.sh disable; taskset -c 2 ./aligned_data --benchmark_min_time=1.0s --benchmark_repetitions=10 --benchmark_counters_tabular=true
bash: ./cpu_turbo.sh: No such file or directory
2025-01-21T03:35:43-05:00
Running ./aligned_data
Run on (4 X 3300 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 3072 KiB (x1)
Load Average: 0.62, 0.46, 0.40
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------------------------------
Benchmark                                                    Time             CPU   Iterations
----------------------------------------------------------------------------------------------
ProcessAlignedDataNonAlignedVectorMarketData            567139 ns       566806 ns         2497
ProcessAlignedDataNonAlignedVectorMarketData            568114 ns       567778 ns         2497
ProcessAlignedDataNonAlignedVectorMarketData            560345 ns       560017 ns         2497
ProcessAlignedDataNonAlignedVectorMarketData            562517 ns       562202 ns         2497
ProcessAlignedDataNonAlignedVectorMarketData            561366 ns       561033 ns         2497
ProcessAlignedDataNonAlignedVectorMarketData            565390 ns       565042 ns         2497
ProcessAlignedDataNonAlignedVectorMarketData            562679 ns       562363 ns         2497
ProcessAlignedDataNonAlignedVectorMarketData            564199 ns       563843 ns         2497
ProcessAlignedDataNonAlignedVectorMarketData            563256 ns       562915 ns         2497
ProcessAlignedDataNonAlignedVectorMarketData            566714 ns       566369 ns         2497
ProcessAlignedDataNonAlignedVectorMarketData_mean       564172 ns       563837 ns           10
ProcessAlignedDataNonAlignedVectorMarketData_median     563728 ns       563379 ns           10
ProcessAlignedDataNonAlignedVectorMarketData_stddev       2595 ns         2590 ns           10
ProcessAlignedDataNonAlignedVectorMarketData_cv           0.46 %          0.46 %            10
ProcessAlignedDataAlignedVectorMarketData               578348 ns       577896 ns         2450
ProcessAlignedDataAlignedVectorMarketData               559267 ns       558948 ns         2450
ProcessAlignedDataAlignedVectorMarketData               563215 ns       562872 ns         2450
ProcessAlignedDataAlignedVectorMarketData               562318 ns       561978 ns         2450
ProcessAlignedDataAlignedVectorMarketData               565753 ns       565428 ns         2450
ProcessAlignedDataAlignedVectorMarketData               562195 ns       561880 ns         2450
ProcessAlignedDataAlignedVectorMarketData               561139 ns       560776 ns         2450
ProcessAlignedDataAlignedVectorMarketData               561120 ns       560777 ns         2450
ProcessAlignedDataAlignedVectorMarketData               563396 ns       563077 ns         2450
ProcessAlignedDataAlignedVectorMarketData               562703 ns       562367 ns         2450
ProcessAlignedDataAlignedVectorMarketData_mean          563945 ns       563600 ns           10
ProcessAlignedDataAlignedVectorMarketData_median        562510 ns       562173 ns           10
ProcessAlignedDataAlignedVectorMarketData_stddev          5339 ns         5304 ns           10
ProcessAlignedDataAlignedVectorMarketData_cv              0.95 %          0.94 %            10
ProcessAlignedDataAlignedArrayMarketData                561733 ns       561410 ns         2491
ProcessAlignedDataAlignedArrayMarketData                561229 ns       560865 ns         2491
ProcessAlignedDataAlignedArrayMarketData                565639 ns       565251 ns         2491
ProcessAlignedDataAlignedArrayMarketData                562160 ns       561799 ns         2491
ProcessAlignedDataAlignedArrayMarketData                562240 ns       561808 ns         2491
ProcessAlignedDataAlignedArrayMarketData                566147 ns       565741 ns         2491
ProcessAlignedDataAlignedArrayMarketData                567204 ns       566850 ns         2491
ProcessAlignedDataAlignedArrayMarketData                566638 ns       566285 ns         2491
ProcessAlignedDataAlignedArrayMarketData                560808 ns       560436 ns         2491
ProcessAlignedDataAlignedArrayMarketData                559706 ns       559338 ns         2491
ProcessAlignedDataAlignedArrayMarketData_mean           563350 ns       562978 ns           10
ProcessAlignedDataAlignedArrayMarketData_median         562200 ns       561804 ns           10
ProcessAlignedDataAlignedArrayMarketData_stddev           2753 ns         2751 ns           10
ProcessAlignedDataAlignedArrayMarketData_cv               0.49 %          0.49 %            10
ppatoria@ux:~/dev/interview/cache/benchmark/build$ export CXX=/bin/g++ ; export CC=/bin/gcc; cd ~/dev/interview/cache/benchmark; rm -rf build; mkdir build; cd build/; cmake -D CMAKE_BUILD_TYPE=Release ../; make;
-- The C compiler identification is GNU 13.1.0
-- The CXX compiler identification is GNU 13.1.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /bin/gcc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /bin/g++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Success
-- Found Threads: TRUE  
-- Build type: Release
-- C++ Flags: 
-- C++ Flags Release: -O3 -DNDEBUG -funroll-loops
-- Configuring done
-- Generating done
-- Build files have been written to: /home/ppatoria/dev/interview/cache/benchmark/build
[  8%] Building CXX object CMakeFiles/cache_benchmark.dir/benchmark.cpp.o
[ 16%] Linking CXX executable cache_benchmark
[ 16%] Built target cache_benchmark
[ 25%] Building CXX object CMakeFiles/cache_benchmark_aligned_data.dir/benchmark_aligned_data.cpp.o
[ 33%] Linking CXX executable cache_benchmark_aligned_data
[ 33%] Built target cache_benchmark_aligned_data
[ 41%] Building CXX object CMakeFiles/cache_benchmark_unrolling_loop.dir/unrolling_loop.cpp.o
[ 50%] Linking CXX executable cache_benchmark_unrolling_loop
[ 50%] Built target cache_benchmark_unrolling_loop
[ 58%] Building CXX object CMakeFiles/default.dir/default.cpp.o
[ 66%] Linking CXX executable default
[ 66%] Built target default
[ 75%] Building CXX object CMakeFiles/aligned_data.dir/aligned_data.cpp.o
[ 83%] Linking CXX executable aligned_data
[ 83%] Built target aligned_data
[ 91%] Building CXX object CMakeFiles/arranged_data.dir/arranged_data.cpp.o
In file included from /home/ppatoria/dev/interview/cache/benchmark/benchmark.h:2,
                 from /home/ppatoria/dev/interview/cache/benchmark/arranged_data.cpp:1:
/home/ppatoria/dev/interview/cache/benchmark/arranged_data.cpp:56:1: error: expected initializer at end of input
   56 | BENCHMARK_MAIN()
      | ^~~~~~~~~~~~~~
make[2]: *** [CMakeFiles/arranged_data.dir/build.make:76: CMakeFiles/arranged_data.dir/arranged_data.cpp.o] Error 1
make[1]: *** [CMakeFiles/Makefile2:227: CMakeFiles/arranged_data.dir/all] Error 2
make: *** [Makefile:91: all] Error 2
ppatoria@ux:~/dev/interview/cache/benchmark/build$ export CXX=/bin/g++ ; export CC=/bin/gcc; cd ~/dev/interview/cache/benchmark; rm -rf build; mkdir build; cd build/; cmake -D CMAKE_BUILD_TYPE=Release ../; make;
-- The C compiler identification is GNU 13.1.0
-- The CXX compiler identification is GNU 13.1.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /bin/gcc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /bin/g++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Success
-- Found Threads: TRUE  
-- Build type: Release
-- C++ Flags: 
-- C++ Flags Release: -O3 -DNDEBUG -funroll-loops
-- Configuring done
-- Generating done
-- Build files have been written to: /home/ppatoria/dev/interview/cache/benchmark/build
[  8%] Building CXX object CMakeFiles/cache_benchmark.dir/benchmark.cpp.o
[ 16%] Linking CXX executable cache_benchmark
[ 16%] Built target cache_benchmark
[ 25%] Building CXX object CMakeFiles/cache_benchmark_aligned_data.dir/benchmark_aligned_data.cpp.o
[ 33%] Linking CXX executable cache_benchmark_aligned_data
[ 33%] Built target cache_benchmark_aligned_data
[ 41%] Building CXX object CMakeFiles/cache_benchmark_unrolling_loop.dir/unrolling_loop.cpp.o
[ 50%] Linking CXX executable cache_benchmark_unrolling_loop
[ 50%] Built target cache_benchmark_unrolling_loop
[ 58%] Building CXX object CMakeFiles/default.dir/default.cpp.o
[ 66%] Linking CXX executable default
[ 66%] Built target default
[ 75%] Building CXX object CMakeFiles/aligned_data.dir/aligned_data.cpp.o
[ 83%] Linking CXX executable aligned_data
[ 83%] Built target aligned_data
[ 91%] Building CXX object CMakeFiles/arranged_data.dir/arranged_data.cpp.o
[100%] Linking CXX executable arranged_data
[100%] Built target arranged_data
ppatoria@ux:~/dev/interview/cache/benchmark/build$ ./cpu_turbo.sh disable; taskset -c 2 ./arranged_data --benchmark_min_time=1.0s --benchmark_repetitions=10 --benchmark_counters_tabular=true
bash: ./cpu_turbo.sh: No such file or directory
2025-01-21T03:57:05-05:00
Running ./arranged_data
Run on (4 X 3300 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 3072 KiB (x1)
Load Average: 0.72, 0.71, 0.63
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-----------------------------------------------------------------------------------------------
Benchmark                                                     Time             CPU   Iterations
-----------------------------------------------------------------------------------------------
ProcessArrangedDataNonAlignedVectorMarketData            558780 ns       558477 ns         2501
ProcessArrangedDataNonAlignedVectorMarketData            568183 ns       567914 ns         2501
ProcessArrangedDataNonAlignedVectorMarketData            561648 ns       561325 ns         2501
ProcessArrangedDataNonAlignedVectorMarketData            565949 ns       565625 ns         2501
ProcessArrangedDataNonAlignedVectorMarketData            567010 ns       566689 ns         2501
ProcessArrangedDataNonAlignedVectorMarketData            558708 ns       558368 ns         2501
ProcessArrangedDataNonAlignedVectorMarketData            558834 ns       558478 ns         2501
ProcessArrangedDataNonAlignedVectorMarketData            558300 ns       557986 ns         2501
ProcessArrangedDataNonAlignedVectorMarketData            561786 ns       561423 ns         2501
ProcessArrangedDataNonAlignedVectorMarketData            577381 ns       576958 ns         2501
ProcessArrangedDataNonAlignedVectorMarketData_mean       563658 ns       563324 ns           10
ProcessArrangedDataNonAlignedVectorMarketData_median     561717 ns       561374 ns           10
ProcessArrangedDataNonAlignedVectorMarketData_stddev       6084 ns         6067 ns           10
ProcessArrangedDataNonAlignedVectorMarketData_cv           1.08 %          1.08 %            10
ProcessArrangedDataAlignedVectorMarketData               565176 ns       564839 ns         2469
ProcessArrangedDataAlignedVectorMarketData               559216 ns       558857 ns         2469
ProcessArrangedDataAlignedVectorMarketData               563965 ns       563641 ns         2469
ProcessArrangedDataAlignedVectorMarketData               562915 ns       562599 ns         2469
ProcessArrangedDataAlignedVectorMarketData               560872 ns       560576 ns         2469
ProcessArrangedDataAlignedVectorMarketData               563512 ns       563171 ns         2469
ProcessArrangedDataAlignedVectorMarketData               566892 ns       566606 ns         2469
ProcessArrangedDataAlignedVectorMarketData               565744 ns       565430 ns         2469
ProcessArrangedDataAlignedVectorMarketData               563261 ns       562980 ns         2469
ProcessArrangedDataAlignedVectorMarketData               558594 ns       558304 ns         2469
ProcessArrangedDataAlignedVectorMarketData_mean          563015 ns       562700 ns           10
ProcessArrangedDataAlignedVectorMarketData_median        563386 ns       563076 ns           10
ProcessArrangedDataAlignedVectorMarketData_stddev          2727 ns         2730 ns           10
ProcessArrangedDataAlignedVectorMarketData_cv              0.48 %          0.49 %            10
ProcessArrangedDataAlignedArrayMarketData                563141 ns       562770 ns         2478
ProcessArrangedDataAlignedArrayMarketData                561138 ns       560772 ns         2478
ProcessArrangedDataAlignedArrayMarketData                562737 ns       562275 ns         2478
ProcessArrangedDataAlignedArrayMarketData                564709 ns       564401 ns         2478
ProcessArrangedDataAlignedArrayMarketData                565085 ns       564700 ns         2478
ProcessArrangedDataAlignedArrayMarketData                570317 ns       569932 ns         2478
ProcessArrangedDataAlignedArrayMarketData                565967 ns       565665 ns         2478
ProcessArrangedDataAlignedArrayMarketData                560466 ns       560134 ns         2478
ProcessArrangedDataAlignedArrayMarketData                562491 ns       562129 ns         2478
ProcessArrangedDataAlignedArrayMarketData                574645 ns       574249 ns         2478
ProcessArrangedDataAlignedArrayMarketData_mean           565070 ns       564703 ns           10
ProcessArrangedDataAlignedArrayMarketData_median         563925 ns       563585 ns           10
ProcessArrangedDataAlignedArrayMarketData_stddev           4379 ns         4372 ns           10
ProcessArrangedDataAlignedArrayMarketData_cv               0.78 %          0.77 %            10
ppatoria@ux:~/dev/interview/cache/benchmark/build$ ProcessArrangedDataAlignedArrayMarketData                565085 ns       564700 ns         2478
bash: ProcessArrangedDataAlignedArrayMarketData: command not found
ppatoria@ux:~/dev/interview/cache/benchmark/build$ ./cpu_turbo.sh disable; taskset -c 2 ./arranged_data --benchmark_min_time=1.0s --benchmark_repetitions=10 --benchmark_counters_tabular=true
bash: ./cpu_turbo.sh: No such file or directory
2025-01-21T03:58:45-05:00
Running ./arranged_data
Run on (4 X 3300 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 3072 KiB (x1)
Load Average: 0.46, 0.67, 0.62
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-----------------------------------------------------------------------------------------------
Benchmark                                                     Time             CPU   Iterations
-----------------------------------------------------------------------------------------------
ProcessArrangedDataNonAlignedVectorMarketData            560239 ns       559932 ns         2476
ProcessArrangedDataNonAlignedVectorMarketData            558723 ns       558407 ns         2476
ProcessArrangedDataNonAlignedVectorMarketData            561247 ns       560929 ns         2476
ProcessArrangedDataNonAlignedVectorMarketData            561259 ns       560924 ns         2476
ProcessArrangedDataNonAlignedVectorMarketData            561607 ns       561323 ns         2476
ProcessArrangedDataNonAlignedVectorMarketData            558271 ns       557994 ns         2476
ProcessArrangedDataNonAlignedVectorMarketData            560278 ns       559966 ns         2476
ProcessArrangedDataNonAlignedVectorMarketData            557696 ns       557400 ns         2476
ProcessArrangedDataNonAlignedVectorMarketData            564726 ns       564398 ns         2476
ProcessArrangedDataNonAlignedVectorMarketData            561985 ns       561672 ns         2476
ProcessArrangedDataNonAlignedVectorMarketData_mean       560603 ns       560295 ns           10
ProcessArrangedDataNonAlignedVectorMarketData_median     560763 ns       560445 ns           10
ProcessArrangedDataNonAlignedVectorMarketData_stddev       2066 ns         2057 ns           10
ProcessArrangedDataNonAlignedVectorMarketData_cv           0.37 %          0.37 %            10
ProcessArrangedDataAlignedVectorMarketData               559146 ns       558834 ns         2503
ProcessArrangedDataAlignedVectorMarketData               560560 ns       560230 ns         2503
ProcessArrangedDataAlignedVectorMarketData               559140 ns       558815 ns         2503
ProcessArrangedDataAlignedVectorMarketData               561238 ns       560921 ns         2503
ProcessArrangedDataAlignedVectorMarketData               559635 ns       559301 ns         2503
ProcessArrangedDataAlignedVectorMarketData               567104 ns       566775 ns         2503
ProcessArrangedDataAlignedVectorMarketData               561725 ns       561414 ns         2503
ProcessArrangedDataAlignedVectorMarketData               560895 ns       560563 ns         2503
ProcessArrangedDataAlignedVectorMarketData               563682 ns       563319 ns         2503
ProcessArrangedDataAlignedVectorMarketData               563640 ns       563322 ns         2503
ProcessArrangedDataAlignedVectorMarketData_mean          561677 ns       561349 ns           10
ProcessArrangedDataAlignedVectorMarketData_median        561067 ns       560742 ns           10
ProcessArrangedDataAlignedVectorMarketData_stddev          2503 ns         2499 ns           10
ProcessArrangedDataAlignedVectorMarketData_cv              0.45 %          0.45 %            10
ProcessArrangedDataAlignedArrayMarketData                578757 ns       578003 ns         2465
ProcessArrangedDataAlignedArrayMarketData                558101 ns       557808 ns         2465
ProcessArrangedDataAlignedArrayMarketData                558797 ns       558495 ns         2465
ProcessArrangedDataAlignedArrayMarketData                563127 ns       562767 ns         2465
ProcessArrangedDataAlignedArrayMarketData                566866 ns       566509 ns         2465
ProcessArrangedDataAlignedArrayMarketData                561826 ns       561504 ns         2465
ProcessArrangedDataAlignedArrayMarketData                559525 ns       559228 ns         2465
ProcessArrangedDataAlignedArrayMarketData                565156 ns       564823 ns         2465
ProcessArrangedDataAlignedArrayMarketData                560374 ns       560048 ns         2465
ProcessArrangedDataAlignedArrayMarketData                561157 ns       560818 ns         2465
ProcessArrangedDataAlignedArrayMarketData_mean           563369 ns       563000 ns           10
ProcessArrangedDataAlignedArrayMarketData_median         561491 ns       561161 ns           10
ProcessArrangedDataAlignedArrayMarketData_stddev           6073 ns         5944 ns           10
ProcessArrangedDataAlignedArrayMarketData_cv               1.08 %          1.06 %            10
ppatoria@ux:~/dev/interview/cache/benchmark/build$ export CXX=/bin/g++ ; export CC=/bin/gcc; cd ~/dev/interview/cache/benchmark; rm -rf build; mkdir build; cd build/; cmake -D CMAKE_BUILD_TYPE=Release ../; make;
-- The C compiler identification is GNU 13.1.0
-- The CXX compiler identification is GNU 13.1.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /bin/gcc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /bin/g++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Success
-- Found Threads: TRUE  
-- Build type: Release
-- C++ Flags: 
-- C++ Flags Release: -O3 -DNDEBUG -funroll-loops
-- Configuring done
-- Generating done
-- Build files have been written to: /home/ppatoria/dev/interview/cache/benchmark/build
[  8%] Building CXX object CMakeFiles/cache_benchmark.dir/benchmark.cpp.o
[ 16%] Linking CXX executable cache_benchmark
[ 16%] Built target cache_benchmark
[ 25%] Building CXX object CMakeFiles/cache_benchmark_aligned_data.dir/benchmark_aligned_data.cpp.o
[ 33%] Linking CXX executable cache_benchmark_aligned_data
[ 33%] Built target cache_benchmark_aligned_data
[ 41%] Building CXX object CMakeFiles/cache_benchmark_unrolling_loop.dir/unrolling_loop.cpp.o
[ 50%] Linking CXX executable cache_benchmark_unrolling_loop
[ 50%] Built target cache_benchmark_unrolling_loop
[ 58%] Building CXX object CMakeFiles/default.dir/default.cpp.o
[ 66%] Linking CXX executable default
[ 66%] Built target default
[ 75%] Building CXX object CMakeFiles/aligned_data.dir/aligned_data.cpp.o
[ 83%] Linking CXX executable aligned_data
[ 83%] Built target aligned_data
[ 91%] Building CXX object CMakeFiles/arranged_data.dir/arranged_data.cpp.o
[100%] Linking CXX executable arranged_data
[100%] Built target arranged_data
ppatoria@ux:~/dev/interview/cache/benchmark/build$ ./cpu_turbo.sh disable; taskset -c 2 ./arranged_data --benchmark_min_time=1.0s --benchmark_repetitions=10 --benchmark_counters_tabular=true
bash: ./cpu_turbo.sh: No such file or directory
2025-01-21T04:00:23-05:00
Running ./arranged_data
Run on (4 X 3300 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 3072 KiB (x1)
Load Average: 0.56, 0.67, 0.63
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-----------------------------------------------------------------------------------------------
Benchmark                                                     Time             CPU   Iterations
-----------------------------------------------------------------------------------------------
ProcessArrangedDataAlignedArrayMarketData                567213 ns       566882 ns         2501
ProcessArrangedDataAlignedArrayMarketData                560460 ns       560082 ns         2501
ProcessArrangedDataAlignedArrayMarketData                559959 ns       559607 ns         2501
ProcessArrangedDataAlignedArrayMarketData                558394 ns       558071 ns         2501
ProcessArrangedDataAlignedArrayMarketData                560889 ns       560554 ns         2501
ProcessArrangedDataAlignedArrayMarketData                577273 ns       576833 ns         2501
ProcessArrangedDataAlignedArrayMarketData                561763 ns       561401 ns         2501
ProcessArrangedDataAlignedArrayMarketData                558369 ns       558035 ns         2501
ProcessArrangedDataAlignedArrayMarketData                560478 ns       560123 ns         2501
ProcessArrangedDataAlignedArrayMarketData                560778 ns       560454 ns         2501
ProcessArrangedDataAlignedArrayMarketData_mean           562558 ns       562204 ns           10
ProcessArrangedDataAlignedArrayMarketData_median         560628 ns       560288 ns           10
ProcessArrangedDataAlignedArrayMarketData_stddev           5727 ns         5700 ns           10
ProcessArrangedDataAlignedArrayMarketData_cv               1.02 %          1.01 %            10
ProcessArrangedDataNonAlignedVectorMarketData            565922 ns       565604 ns         2498
ProcessArrangedDataNonAlignedVectorMarketData            561240 ns       560952 ns         2498
ProcessArrangedDataNonAlignedVectorMarketData            559227 ns       558914 ns         2498
ProcessArrangedDataNonAlignedVectorMarketData            589845 ns       589259 ns         2498
ProcessArrangedDataNonAlignedVectorMarketData            566284 ns       565969 ns         2498
ProcessArrangedDataNonAlignedVectorMarketData            558555 ns       558238 ns         2498
ProcessArrangedDataNonAlignedVectorMarketData            559459 ns       559156 ns         2498
ProcessArrangedDataNonAlignedVectorMarketData            569923 ns       569528 ns         2498
ProcessArrangedDataNonAlignedVectorMarketData            562318 ns       561969 ns         2498
ProcessArrangedDataNonAlignedVectorMarketData            564624 ns       564265 ns         2498
ProcessArrangedDataNonAlignedVectorMarketData_mean       565740 ns       565385 ns           10
ProcessArrangedDataNonAlignedVectorMarketData_median     563471 ns       563117 ns           10
ProcessArrangedDataNonAlignedVectorMarketData_stddev       9223 ns         9140 ns           10
ProcessArrangedDataNonAlignedVectorMarketData_cv           1.63 %          1.62 %            10
ProcessArrangedDataAlignedVectorMarketData               560612 ns       560293 ns         2502
ProcessArrangedDataAlignedVectorMarketData               561655 ns       561319 ns         2502
ProcessArrangedDataAlignedVectorMarketData               562257 ns       561905 ns         2502
ProcessArrangedDataAlignedVectorMarketData               564501 ns       564179 ns         2502
ProcessArrangedDataAlignedVectorMarketData               559769 ns       559416 ns         2502
ProcessArrangedDataAlignedVectorMarketData               561169 ns       560827 ns         2502
ProcessArrangedDataAlignedVectorMarketData               559791 ns       559437 ns         2502
ProcessArrangedDataAlignedVectorMarketData               560262 ns       559893 ns         2502
ProcessArrangedDataAlignedVectorMarketData               560010 ns       559670 ns         2502
ProcessArrangedDataAlignedVectorMarketData               563895 ns       563558 ns         2502
ProcessArrangedDataAlignedVectorMarketData_mean          561392 ns       561050 ns           10
ProcessArrangedDataAlignedVectorMarketData_median        560890 ns       560560 ns           10
ProcessArrangedDataAlignedVectorMarketData_stddev          1693 ns         1700 ns           10
ProcessArrangedDataAlignedVectorMarketData_cv              0.30 %          0.30 %            10
ppatoria@ux:~/dev/interview/cache/benchmark/build$ 
