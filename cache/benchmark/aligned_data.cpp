#include "benchmark.h"
#include <vector>

using AlignedDataMDVector =  std::vector<MarketData32Aligned>;
static SimulatedInput<AlignedDataMDVector, Volume>
    alignedDataNonAlignedVectorInputGenerator(AlignedDataMDVector{});
const auto &SimulatedAlignedDataNonAlignedVectorInputData =
    alignedDataNonAlignedVectorInputGenerator.get();

void ProcessAlignedDataNonAlignedVectorMarketData(benchmark::State &state) {
  for (auto _ : state) {
    for (const auto &update : SimulatedAlignedDataNonAlignedVectorInputData) {
      benchmark::DoNotOptimize(&update.price);
      benchmark::DoNotOptimize(&update.symbol_id);
      benchmark::DoNotOptimize(&update.volume);
    }
  }
}
BENCHMARK(ProcessAlignedDataNonAlignedVectorMarketData);




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
BENCHMARK_MAIN();
