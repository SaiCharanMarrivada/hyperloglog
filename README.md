# hyperloglog

A very simple hyperloglog implementation based on `Probably.jl`'s hyperloglog(< 100 LOC). Hyperloglog can estimate large cardinalities using sub-linear space, for instance using only 16KiB, one can estimate cardinalities as high as $2^{62}$. Hash function provided should have good uniform distribution for hyperloglog to work correctly.
This requires compiler with C++20 support.

# How to run
```bash
git clone https://github.com/SaiCharanMarrivada/hyperloglog.git
cd hyperloglog
./install_deps.sh
make run
```
By default `g++` is used, to use `clang++` change the last line to `make CLANG=1 run`.

# Usage
```cpp
#include <functional>
#include <iostream>

#include "hyperloglog.h"
#include "komihash.h"

struct Int {
  int x;

  Int(int x) : x(x) {}
};

template <>
struct std::hash<Int> {
  // inject `std::hash` specialization for custom types
  inline size_t operator()(Int x) { return komihash(&x, 4, 0); }
};

int main() {
  HyperLogLog<Int> hll;  // by default precision is set to 14
  HyperLogLog<Int, 15>
      hll2;  // can be changed by passing second template parameter

  for (int i = 0; i < 10000; i++) {
    hll.insert(i);  // or hll.add(i);
  }

  std::cout << "estimate: " << hll.size() /* or hll.report() */ << "\n";
}
```

# Benchmark
`test_hll.cc` is run on Intel &trade; i7-13700 24-core cpu with 1-24 threads. OpenMP is used for multithreading. ![](./running_times.png?raw=true).

### TODO
- [  ] add other operations like set-difference, set-intersection and so on.
- [  ] add support for serialization-deserialization.
