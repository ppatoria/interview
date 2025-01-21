#include "benchmark.h"
/**
 * Benchmarking Functions
 */

static SimulatedAlignedArrayGenerator<MarketDataArranged, Volume>
    arrangedDataAlignedArrayInputGenerator;
const auto &SimulatedArrangedDataAlignedArrayInputData =
    arrangedDataAlignedArrayInputGenerator.get();


void ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop(
    benchmark::State &state) {
  const auto &updates = SimulatedArrangedDataAlignedArrayInputData.data;
  for (auto _ : state) {

#pragma GCC unroll 4
    for(int i=0; i < updates.size(); i++) {

      benchmark::DoNotOptimize(&updates[i].price);
      benchmark::DoNotOptimize(&updates[i].symbol_id);
      benchmark::DoNotOptimize(&updates[i].volume);
    }
  }
}
BENCHMARK(ProcessArrangedDataAlignedArrayMarketDataUnrollingIndexBasedLoop);

void ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop(
    benchmark::State &state) {
  const auto &updates = SimulatedArrangedDataAlignedArrayInputData.data;
  for (auto _ : state) {

#pragma GCC unroll 4
    for(const auto& update :  updates) {

      benchmark::DoNotOptimize(&update.price);
      benchmark::DoNotOptimize(&update.symbol_id);
      benchmark::DoNotOptimize(&update.volume);
    }
  }
}
BENCHMARK(ProcessArrangedDataAlignedArrayMarketDataUnrollingRangeBasedLoop);

void ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled(
    benchmark::State &state) {
  const auto &updates = SimulatedArrangedDataAlignedArrayInputData.data;
  for (auto _ : state) {

    for(const auto& update :  updates) {

      benchmark::DoNotOptimize(&update.price);
      benchmark::DoNotOptimize(&update.symbol_id);
      benchmark::DoNotOptimize(&update.volume);
    }
  }
}
BENCHMARK(ProcessArrangedDataAlignedArrayMarketDataUnrollingLoopDisabled);


BENCHMARK_MAIN();
