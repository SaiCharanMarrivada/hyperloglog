# hyperloglog

A very simple hyperloglog implementation based on `Probably.jl`'s hyperloglog. Hyperloglog can estimate large cardinalities using sub-linear space, for instance using only 16KiB, one can estimate cardinalities as high as $2^{62}$. Hash function provided should have good uniform distribution for hyperloglog to work correctly.

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
#include "hyperloglog.h"
#include <iostream>

Hyperloglog<int> hll; // by default precision is set to 14
Hyperloglog<int, 15> hll2; // can be changed by passing second template parameter

for (int i = 0; i < 10000; i++) {
  hll.insert(i); // or hll.add(i);
}

std::cout << hll.size() /* or hll.report() */ << "\n";
```

# Benchmark
The code is run on Intel &trade; i7-13700 cpu with 24 threads. OpenMP is used for multithreading. ![](./running_times.png?raw=true) 
