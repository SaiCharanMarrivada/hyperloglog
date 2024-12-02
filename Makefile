CLANG=0
CCFLAGS = -Wall -fopenmp
INCLUDES = -I./benchmark/include/
OPTFLAGS = -O3 -funroll-loops -march=native # -DCMOV
LFLAGS = -L./benchmark/build/src/ -lbenchmark
STACKALLOCATE = 1
DEBUG = 0

ifeq ($(CLANG), 0)
	CPP = g++
else
	CPP = clang++
endif

ifeq ($(STACKALLOCATE), 1)
	OPTFLAGS += -DSTACK_ALLOCATE
endif

ifeq ($(DEBUG), 1)
	OPTFLAGS += -g
endif

test_hll: test_hll.cc hyperloglog.h
	$(CPP) $(INCLUDES) $(CCFLAGS) $(OPTFLAGS) test_hll.cc -o test_hll $(LFLAGS)

run: test_hll
	./test_hll --benchmark_time_unit=s

clean:
	rm test_hll
