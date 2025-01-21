#include "benchmark.h"
#include <vector>

using MDVector = std::vector<MarketData>;
static SimulatedInput<MDVector, Volume> inputGenerator(MDVector{});
const auto &SimulatedInputData = inputGenerator.get();

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
BENCHMARK_MAIN();
