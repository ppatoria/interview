# Cache Optimization benchmark results

## Buffering
### Code

```cpp

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
                benchmark::DoNotOptimize(update.symbol_id);
                benchmark::DoNotOptimize(update.price);
                benchmark::DoNotOptimize(update.volume) ;
            }
        }

    }
}
BENCHMARK(processUsingBufferBenchmark);

```
### Assembly Output

```asm
       push   %r14
       pxor   %xmm0,%xmm0
       mov    %rdi,%r14
       push   %r13
       push   %r12
       push   %rbp
       push   %rbx
       sub    $0x620,%rsp
       mov    %rsp,%rdi
       movaps %xmm0,(%rsp)
       movq   $0x0,0x10(%rsp)
       callq  409a20 <void populateSimulatedInput<std::vector<MarketData, std::allocator<MarketData> > >(std::vector<MarketData, std::allocator<MarketData> >&)>
       cmpb   $0x0,0x1a(%r14)
       je     40912b <processUsingBufferBenchmark(benchmark::State&)+0x6b>
       mov    %r14,%rdi
       callq  40bd70 <benchmark::State::StartKeepRunning()>
       mov    %r14,%rdi
       callq  40bfa0 <benchmark::State::FinishKeepRunning()>
       mov    (%rsp),%rdi
       test   %rdi,%rdi
       je     409291 <processUsingBufferBenchmark(benchmark::State&)+0x1d1>
       mov    0x10(%rsp),%rsi
       add    $0x620,%rsp
       pop    %rbx
       pop    %rbp
       sub    %rdi,%rsi
       pop    %r12
       pop    %r13
       pop    %r14
       jmpq   405480 <operator delete(void*, unsigned long)@plt>
       mov    %r14,%rdi
       mov    0x10(%r14),%rbx
       callq  40bd70 <benchmark::State::StartKeepRunning()>
       mov    $0x40,%r12d
       lea    0x20(%rsp),%r13
       test   %rbx,%rbx
       je     4090fa <processUsingBufferBenchmark(benchmark::State&)+0x3a>
       nopw   0x0(%rax,%rax,1)
       mov    0x8(%rsp),%rax
       mov    (%rsp),%rdx
       pxor   %xmm2,%xmm2
       movabs $0xaaaaaaaaaaaaaaab,%rcx
       movapd %xmm2,%xmm1
       mov    %rax,%r8
       sub    %rdx,%r8
       mov    %r8,%r11
       sar    $0x3,%r11
       imul   %rcx,%r11
       cmp    %rdx,%rax
       je     409260 <processUsingBufferBenchmark(benchmark::State&)+0x1a0>
       pxor   %xmm2,%xmm2
       mov    %r11,%rax
       movapd %xmm2,%xmm1
       mov    $0x600,%r9d
       mov    $0x40,%r10d
       cmp    $0x3f,%rax
       jbe    409248 <processUsingBufferBenchmark(benchmark::State&)+0x188>
       mov    %r9d,%ecx
       mov    %r9d,%ebp
       mov    %r13,%rdi
       mov    %rdx,%rsi
0.16%  shr    $0x3,%ecx
       and    $0x4,%r9d
59.16% rep movsq %ds:(%rsi),%es:(%rdi)
14.76% je     4091c7 <processUsingBufferBenchmark(benchmark::State&)+0x107>
       mov    (%rsi),%ecx
       mov    %ecx,(%rdi)
       mov    $0x4,%ecx
0.21%  test   $0x2,%bpl
       je     4091db <processUsingBufferBenchmark(benchmark::State&)+0x11b>
       movzwl (%rsi,%rcx,1),%r9d
       mov    %r9w,(%rdi,%rcx,1)
       add    $0x2,%rcx
       and    $0x1,%ebp
       je     4091e8 <processUsingBufferBenchmark(benchmark::State&)+0x128>
       movzbl (%rsi,%rcx,1),%esi
       mov    %sil,(%rdi,%rcx,1)
       lea    (%r10,%r10,2),%rsi
       lea    0x28(%rsp),%rcx
       lea    0x28(%rsp,%rsi,8),%rsi
       nopw   %cs:0x0(%rax,%rax,1)
0.82%  pxor   %xmm0,%xmm0
1.17%  add    $0x18,%rcx
0.72%  cvtsi2sdl -0x10(%rcx),%xmm0
5.82%  addsd  %xmm0,%xmm1
2.31%  mulsd  -0x18(%rcx),%xmm0
0.98%  addsd  %xmm0,%xmm2
13.59% cmp    %rcx,%rsi
       jne    409200 <processUsingBufferBenchmark(benchmark::State&)+0x140>
       mov    %r12,%rcx
       add    $0x600,%rdx
       sub    $0x600,%r8
       sub    %rax,%rcx
       lea    -0x40(%rax),%rsi
       add    %r11,%rcx
       jae    409260 <processUsingBufferBenchmark(benchmark::State&)+0x1a0>
       mov    %rsi,%rax
0.29%  jmpq   409190 <processUsingBufferBenchmark(benchmark::State&)+0xd0>
       nopl   0x0(%rax)
       cmp    $0x18,%r8
       je     409279 <processUsingBufferBenchmark(benchmark::State&)+0x1b9>
       mov    %r8,%r9
       mov    %rax,%r10
       jmpq   4091a6 <processUsingBufferBenchmark(benchmark::State&)+0xe6>
       nopl   0x0(%rax)
       movq   %xmm1,%rax
       movq   %xmm2,%rax
       sub    $0x1,%rbx
       jne    409150 <processUsingBufferBenchmark(benchmark::State&)+0x90>
       jmpq   4090fa <processUsingBufferBenchmark(benchmark::State&)+0x3a>
       movdqu (%rdx),%xmm3
       mov    0x10(%rdx),%ecx
       mov    %rax,%r10
       mov    %ecx,0x30(%rsp)
       movaps %xmm3,0x20(%rsp)
       jmpq   4091e8 <processUsingBufferBenchmark(benchmark::State&)+0x128>
       add    $0x620,%rsp
       pop    %rbx
       pop    %rbp
       pop    %r12
       pop    %r13
       pop    %r14
       retq
       mov    %rax,%rbx
       jmpq   405976 <processUsingBufferBenchmark(benchmark::State&) [clone .cold]>
```

### Quick Benchmark Result:
    **CPU Time** : 134,050,332.969
    **Compared to Prefetching** : 1.1 times slower
    **Compared to Allignment** : 6.7 times slower
## Prefetching
### Code
```cpp
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

```

### Assembly

```asm
       push   %rbp
       pxor   %xmm0,%xmm0
       mov    %rdi,%rbp
       push   %rbx
       sub    $0x28,%rsp
       mov    %rsp,%rdi
       movaps %xmm0,(%rsp)
       movq   $0x0,0x10(%rsp)
       callq  409a20 <void populateSimulatedInput<std::vector<MarketData, std::allocator<MarketData> > >(std::vector<MarketData, std::allocator<MarketData> >&)>
       cmpb   $0x0,0x1a(%rbp)
       je     408ff8 <ProcessByPrefetching(benchmark::State&)+0x58>
       mov    %rbp,%rdi
       callq  40bd70 <benchmark::State::StartKeepRunning()>
       mov    %rbp,%rdi
       callq  40bfa0 <benchmark::State::FinishKeepRunning()>
       mov    (%rsp),%rdi
       test   %rdi,%rdi
       je     4090af <ProcessByPrefetching(benchmark::State&)+0x10f>
       mov    0x10(%rsp),%rsi
       add    $0x28,%rsp
       pop    %rbx
       pop    %rbp
       sub    %rdi,%rsi
       jmpq   405480 <operator delete(void*, unsigned long)@plt>
       mov    %rbp,%rdi
       mov    0x10(%rbp),%rbx
       callq  40bd70 <benchmark::State::StartKeepRunning()>
       test   %rbx,%rbx
       je     408fd0 <ProcessByPrefetching(benchmark::State&)+0x30>
       nopl   0x0(%rax)
       mov    (%rsp),%rax
       mov    0x8(%rsp),%rsi
       cmp    %rax,%rsi
       je     408fd0 <ProcessByPrefetching(benchmark::State&)+0x30>
       mov    %rsi,%rdx
       sub    %rax,%rdx
       cmp    $0x600,%rdx
       ja     409090 <ProcessByPrefetching(benchmark::State&)+0xf0>
       nopl   (%rax)
       cmp    %rax,%rsi
       je     4090a0 <ProcessByPrefetching(benchmark::State&)+0x100>
       mov    %rsi,%rcx
0.03%  mov    %rcx,%rdi
0.03%  mov    %rax,%rdx
0.15%  sub    %rax,%rdi
       and    $0x8,%edi
       je     409058 <ProcessByPrefetching(benchmark::State&)+0xb8>
       lea    0x18(%rax),%rdx
       prefetcht0 (%rax)
       cmp    %rdx,%rcx
       je     409070 <ProcessByPrefetching(benchmark::State&)+0xd0>
       nopw   0x0(%rax,%rax,1)
0.03%  prefetcht0 (%rdx)
54.79% prefetcht0 0x18(%rdx)
40.68% add    $0x30,%rdx
0.06%  cmp    %rdx,%rcx
       jne    409058 <ProcessByPrefetching(benchmark::State&)+0xb8>
       nopl   0x0(%rax,%rax,1)
1.69%  add    $0x18,%rax
2.34%  cmp    %rax,%rcx
       jne    409070 <ProcessByPrefetching(benchmark::State&)+0xd0>
       cmp    %rcx,%rsi
       je     4090a0 <ProcessByPrefetching(benchmark::State&)+0x100>
0.03%  mov    %rcx,%rax
       mov    %rsi,%rdx
       sub    %rax,%rdx
       cmp    $0x600,%rdx
       jbe    409030 <ProcessByPrefetching(benchmark::State&)+0x90>
0.15%  lea    0x600(%rax),%rcx
0.03%  jmp    409038 <ProcessByPrefetching(benchmark::State&)+0x98>
       nopl   0x0(%rax)
       sub    $0x1,%rbx
       jne    409010 <ProcessByPrefetching(benchmark::State&)+0x70>
       jmpq   408fd0 <ProcessByPrefetching(benchmark::State&)+0x30>
       add    $0x28,%rsp
       pop    %rbx
       pop    %rbp
       retq
       mov    %rax,%rbx
       jmpq   405966 <ProcessByPrefetching(benchmark::State&) [clone .cold]>
```

### Quick Benchmark Result:
    **CPU Time** : 124,389,565.491
    **Compared to Buffering** : 1.1 times faster
    **Compared to Allignment** : 6.2 times slower

## Allignment
### Code 
```cpp
#include <benchmark/benchmark.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <random>
#include <chrono>
#include <cstdlib> // For posix_memalign
#include <type_traits>

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
```

### Assembly

```cpp
       push   %r12
       pxor   %xmm0,%xmm0
       mov    %rdi,%r12
       push   %rbp
       push   %rbx
       sub    $0x20,%rsp
       mov    %rsp,%rdi
       movaps %xmm0,(%rsp)
       movq   $0x0,0x10(%rsp)
       callq  409490 <void populateSimulatedInput<std::vector<MarketDataAlligned, aligned_allocator<MarketDataAlligned, 64ul> > >(std::vector<MarketDataAlligned, aligned_allocator<MarketDataAlligned, 64ul> >&)>
       cmpb   $0x0,0x1a(%r12)
       je     408f0b <ProcessAlignedMarketData(benchmark::State&)+0xdb>
       mov    %r12,%rdi
       callq  40bd70 <benchmark::State::StartKeepRunning()>
       mov    (%rsp),%rbx
       nopl   0x0(%rax)
       mov    %r12,%rdi
       callq  40bfa0 <benchmark::State::FinishKeepRunning()>
       mov    $0xa,%edx
       mov    $0x42501e,%esi
       mov    $0x42e640,%edi
       callq  4054f0 <std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long)@plt>
       mov    0x8(%rsp),%rsi
       mov    $0x42e640,%edi
       sub    %rbx,%rsi
       sar    $0x5,%rsi
       callq  405300 <std::ostream& std::ostream::_M_insert<unsigned long>(unsigned long)@plt>
       mov    $0x15,%edx
       mov    $0x425029,%esi
       mov    %rax,%rdi
       mov    %rax,%rbp
       callq  4054f0 <std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long)@plt>
       mov    0x0(%rbp),%rax
       mov    -0x18(%rax),%rax
       mov    0xf0(%rbp,%rax,1),%r12
       test   %r12,%r12
       je     408f8f <ProcessAlignedMarketData(benchmark::State&)+0x15f>
       cmpb   $0x0,0x38(%r12)
       je     408f58 <ProcessAlignedMarketData(benchmark::State&)+0x128>
       movsbl 0x43(%r12),%esi
       mov    %rbp,%rdi
       callq  405040 <std::ostream::put(char)@plt>
       mov    %rax,%rdi
       callq  405310 <std::ostream::flush()@plt>
       test   %rbx,%rbx
       je     408f86 <ProcessAlignedMarketData(benchmark::State&)+0x156>
       add    $0x20,%rsp
       mov    %rbx,%rdi
       pop    %rbx
       pop    %rbp
       pop    %r12
       jmpq   405560 <free@plt>
       mov    %r12,%rdi
       mov    0x10(%r12),%rbp
       callq  40bd70 <benchmark::State::StartKeepRunning()>
       mov    (%rsp),%rbx
       test   %rbp,%rbp
       je     408e70 <ProcessAlignedMarketData(benchmark::State&)+0x40>
       nopl   (%rax)
       mov    0x8(%rsp),%rdx
       cmp    %rbx,%rdx
       je     408e70 <ProcessAlignedMarketData(benchmark::State&)+0x40>
       mov    %rbx,%rax
       nopl   0x0(%rax)
99.85% add    $0x20,%rax
0.15%  cmp    %rax,%rdx
       jne    408f40 <ProcessAlignedMarketData(benchmark::State&)+0x110>
       mov    (%rsp),%rbx
       sub    $0x1,%rbp
       jne    408f28 <ProcessAlignedMarketData(benchmark::State&)+0xf8>
       jmpq   408e70 <ProcessAlignedMarketData(benchmark::State&)+0x40>
       mov    %r12,%rdi
       callq  405500 <std::ctype<char>::_M_widen_init() const@plt>
       mov    (%r12),%rax
       mov    $0xa,%esi
       mov    0x30(%rax),%rax
       cmp    $0x4092b0,%rax
       je     408ee2 <ProcessAlignedMarketData(benchmark::State&)+0xb2>
       mov    %r12,%rdi
       callq  *%rax
       movsbl %al,%esi
       jmpq   408ee2 <ProcessAlignedMarketData(benchmark::State&)+0xb2>
       add    $0x20,%rsp
       pop    %rbx
       pop    %rbp
       pop    %r12
       retq
       callq  4055c0 <std::__throw_bad_cast()@plt>
       mov    %rax,%rbx
       jmpq   405950 <ProcessAlignedMarketData(benchmark::State&) [clone .cold]>
```

### Quick Benchmark Result:
    **CPU Time** : 19,964,785.862
    **Compared to Buffering** : 6.7 times faster
    **Compared to Allignment** : 6.2 times faster

## Complete code as executed in Quick Benchmark
```cpp

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

```
