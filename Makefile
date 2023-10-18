GTEST_ROOT = googletest/googletest

CFLAGS = -Wall -Wextra -pedantic -g -gdwarf-4 -O0 -std=c99
CXXFLAGS = -Wall -Wextra -pedantic -g -gdwarf-4 -O2 -std=c++17 -I$(GTEST_ROOT)/include -I$(GTEST_ROOT)
LDLIBS = -lpthread

all: algorithms

algorithms: algorithms_tests.o algorithms.o $(GTEST_ROOT)/src/gtest-all.o
	$(CXX) -o $@ $^ -lpthread

clean:
	rm -f *.o $(GTEST_ROOT)/src/gtest-all.o
	rm -f algorithms

tests: algorithms
	./algorithms
