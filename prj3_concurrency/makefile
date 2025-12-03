# Compiler and Flags
CXX = clang++-14
# Use -O3 for benchmarking. Use -O0 -g for debugging.
CXXFLAGS = -std=c++17 -pthread -Wall -O3

# Common implementation
COMMON_SRC = RBTree.cpp
COMMON_OBJ = $(COMMON_SRC:.cpp=.o)

# Target 1: Global Lock Test
TARGET1 = global_lock_test
SRC1 = RBTree_test.cpp
OBJ1 = $(SRC1:.cpp=.o)

# Target 2: Advanced Lock Test
TARGET2 = advanced_lock_test
SRC2 = RBTree_advanced_lock_test.cpp
OBJ2 = $(SRC2:.cpp=.o)

# Target 3: Benchmark
TARGET3 = benchmark
SRC3 = benchmark.cpp
OBJ3 = $(SRC3:.cpp=.o)

# Default target: build all
all: $(TARGET1) $(TARGET2) $(TARGET3)

# Build Global Lock Test
$(TARGET1): $(COMMON_OBJ) $(OBJ1)
	$(CXX) $(CXXFLAGS) -o $(TARGET1) $(COMMON_OBJ) $(OBJ1)

# Build Advanced Lock Test
$(TARGET2): $(COMMON_OBJ) $(OBJ2)
	$(CXX) $(CXXFLAGS) -o $(TARGET2) $(COMMON_OBJ) $(OBJ2)

# Build Benchmark
$(TARGET3): $(COMMON_OBJ) $(OBJ3)
	$(CXX) $(CXXFLAGS) -o $(TARGET3) $(COMMON_OBJ) $(OBJ3)

# Compile object files (Depends on RBTree.h)
%.o: %.cpp RBTree.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET1) $(TARGET2) $(TARGET3) *.o

.PHONY: all clean