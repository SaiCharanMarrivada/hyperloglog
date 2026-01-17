#include "benchmark/benchmark.h"
#include "hyperloglog.h"
#include "komihash.h"
#include <mutex>
#include <thread>
#include <pthread.h>
#include <iostream>
#define XXH_INLINE_ALL /* or XXH_PRIVATE_API */
extern "C" {
#include "xxhash.h"
}

#define N (3e9) 

void test_hll() {
    HyperLogLog<int64_t> hll;
    // auto nthreads = std::thread::hardware_concurrency();
    auto nthreads = 10;
    std::mutex mutex;
    std::vector<std::thread> threads;
    threads.reserve(nthreads);
    auto chunk = N / nthreads;
    thread_local HyperLogLog<int64_t> temp_hll;

    for (size_t i = 0; i < nthreads; i++) {
        threads.emplace_back([&hll, nthreads, chunk, &mutex](int id) {
            for (int64_t i = id * chunk; i < std::min((id + 1) * chunk, N); i++) {
                temp_hll.insert(komihash(&i, 8, 0));
            }
            std::lock_guard<std::mutex> guard(mutex);
            hll += temp_hll;
        }, i);
    }

    for (auto &thread : threads) {
        thread.join();
    }
    std::cout << "size " << hll.size() << '\n';
}

int main() {
    test_hll();
}
