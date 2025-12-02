CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -pthread

EXEC_BENCH = benchmark
EXEC_TEST = fixed_hashmap_test

SRC_COMMON = fixed_hashmap.cpp
SRC_BENCH = benchmark.cpp
SRC_TEST = fixed_hashmap_test.cpp

OBJ_COMMON = $(SRC_COMMON:.cpp=.o)
OBJ_BENCH = $(SRC_BENCH:.cpp=.o)
OBJ_TEST = $(SRC_TEST:.cpp=.o)

.PHONY: all clean benchmark test

all: $(EXEC_BENCH) $(EXEC_TEST)

# Command: make benchmark
benchmark: $(EXEC_BENCH)

# Command: make test
test: $(EXEC_TEST)


$(EXEC_BENCH): $(OBJ_BENCH) $(OBJ_COMMON)
	$(CXX) $(CXXFLAGS) -o $(EXEC_BENCH) $(OBJ_BENCH) $(OBJ_COMMON)

$(EXEC_TEST): $(OBJ_TEST) $(OBJ_COMMON)
	$(CXX) $(CXXFLAGS) -o $(EXEC_TEST) $(OBJ_TEST) $(OBJ_COMMON)

%.o: %.cpp fixed_hashmap.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(EXEC_BENCH) $(EXEC_TEST) *.o