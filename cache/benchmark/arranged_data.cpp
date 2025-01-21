#include "benchmark.h"
#include <vector>

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

using ArrangedMDVector = std::vector<MarketDataArranged> ;
static SimulatedInput<ArrangedMDVector, Volume>
    arrangedDataNonAlignedVectorInputGenerator(ArrangedMDVector{});
const auto &SimulatedArrangedDataNonAlignedVectorInputData =
    arrangedDataNonAlignedVectorInputGenerator.get();
void ProcessArrangedDataNonAlignedVectorMarketData(benchmark::State &state) {
  for (auto _ : state) {
    for (const auto &update : SimulatedArrangedDataNonAlignedVectorInputData) {
      benchmark::DoNotOptimize(&update.price);
      benchmark::DoNotOptimize(&update.symbol_id);
      benchmark::DoNotOptimize(&update.volume);
    }
  }
}
BENCHMARK(ProcessArrangedDataNonAlignedVectorMarketData);

using ArrangedAlignedMDVector = std::vector<MarketDataArranged, aligned_allocator<MarketData32Aligned, 64>>;
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






BENCHMARK_MAIN();

