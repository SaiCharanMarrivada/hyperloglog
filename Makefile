CPP = g++
CCFLAGS = -Wall -fopenmp --std=c++20
OPTFLAGS = -O3 -DCMOV -funroll-loops -march=native

STACKALLOCATE = 1
ifeq ($(STACKALLOCATE), 1)
	OPTFLAGS += -DSTACK_ALLOCATE
endif

test_hll:
	$(CPP) $(CCFLAGS) $(OPTFLAGS) test_hll.cc -o test_hll

run: test_hll
	./test_hll

clean:
	rm test_hll
