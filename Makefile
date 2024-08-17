CPP = g++
CCFLAGS = -Wall -fopenmp --std=c++20
INCLUDES = -I./benchmark/include/
OPTFLAGS = -O3 -DCMOV -funroll-loops -march=native
LFLAGS = -L./benchmark/build/src/ -lbenchmark
STACKALLOCATE = 1
DEBUG = 0

ifeq ($(STACKALLOCATE), 1)
	OPTFLAGS += -DSTACK_ALLOCATE
endif

ifeq ($(DEBUG), 1)
	OPTFLAGS += -g
endif

test_hll: test_hll.cc
	$(CPP) $(INCLUDES) $(CCFLAGS) $(OPTFLAGS) test_hll.cc -o test_hll $(LFLAGS)

run: test_hll
	./test_hll

clean:
	rm test_hll
