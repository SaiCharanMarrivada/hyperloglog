# hyperloglog

A very simple hyperloglog implementation based on `Probably.jl`'s hyperloglog. Hyperloglog can estimate large cardinalities using sub-linear space, for instance using only 16KiB, one can estimate cardinalities as high as $2^{62}$. Hash function provided should have good uniform distribution for hyperloglog to work correctly.

# How to run
```bash
git clone https://github.com/SaiCharanMarrivada/hyperloglog.git
cd hyperloglog
make run
```
By default `g++` is used, to use `clang++` change the last line to `make CLANG=1 run`.

# Benchmark
The code is run on Intel &trade; i7-13700 cpu with 24 threads. OpenMP &trade; is used for multithreading. ![](./running_times.png?raw=true) 
