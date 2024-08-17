#include <omp.h>
#include <cmath>
#include <iostream>
#include "hyperloglog.h"
#include "komihash.h"

#define N 3e11

int main() {
  HyperLogLog<int64_t> hll;
  double t = omp_get_wtime();

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

  std::cout << "time elapsed: " << (omp_get_wtime() - t) << "\n";
  std::cout << "estimate: " << hll.size() << "\n";
}
