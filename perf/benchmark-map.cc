/*
 * Benchmarks for hb_map_t operations.
 */
#include "benchmark/benchmark.h"

#include <cstdlib>
#include "hb.h"

void RandomMap(unsigned size, hb_map_t* out) {
  hb_map_clear(out);

  srand(size);
  for (unsigned i = 0; i < size; i++) {
    while (true) {
      hb_codepoint_t next = rand();
      if (hb_map_has (out, next)) continue;

      hb_map_set (out, next, rand ());
      break;
    }
  }
}

/* Insert a single value into map of varying sizes. */
static void BM_MapInsert(benchmark::State& state) {
  unsigned map_size = state.range(0);

  hb_map_t* original = hb_map_create ();
  RandomMap(map_size, original);
  assert(hb_map_get_population(original) == map_size);

  for (auto _ : state) {
    // TODO(garretrieger): create a copy of the original map.
    //                     Needs a hb_map_copy(..) in public api.

    hb_map_set (original, rand (), rand ());
  }

  hb_map_destroy(original);
}
BENCHMARK(BM_MapInsert)
    ->Range(1 << 10, 1 << 16);

/* Single value lookup on map of various sizes. */
static void BM_MapLookup(benchmark::State& state) {
  unsigned map_size = state.range(0);

  hb_map_t* original = hb_map_create ();
  RandomMap(map_size, original);
  assert(hb_map_get_population(original) == map_size);

  for (auto _ : state) {
    benchmark::DoNotOptimize(
        hb_map_get (original, rand()));
  }

  hb_map_destroy(original);
}
BENCHMARK(BM_MapLookup)
    ->Range(1 << 10, 1 << 16); // Set size


BENCHMARK_MAIN();