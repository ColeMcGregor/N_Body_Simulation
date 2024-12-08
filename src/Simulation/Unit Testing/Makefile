# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -I./src/Simulation/Unit\ Testing

# Files and directories
SRC = vector.cpp
TEST = UnitTest.cpp
OBJS = $(SRC:.cpp=.o) $(TEST:.cpp=.o)
TARGET = UnitTest

# Build the test executable
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Run the tests
run: all
	./$(TARGET)
