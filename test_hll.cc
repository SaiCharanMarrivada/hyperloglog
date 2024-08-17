#include <omp.h>

#include <cmath>
#include <iostream>

#include "benchmark/benchmark.h"
#include "hyperloglog.h"
#include "komihash.h"

#define N 3e9

void test_hll(benchmark::State &state) {
  for (auto _ : state) {
    HyperLogLog<int64_t> hll;

    int id, nthreads;

#pragma omp parallel
    {
      HyperLogLog<int64_t> temp_hll;
      id = omp_get_thread_num();
      nthreads = omp_get_num_threads();

      for (int64_t i = id; i < (int64_t)N; i += nthreads) {
        temp_hll.add(komihash(&i, 8, 0));
      }

#pragma omp critical
      hll += temp_hll;
    }
  }
}

BENCHMARK(test_hll);
BENCHMARK_MAIN();
