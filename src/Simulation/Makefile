CXX = clang++
CXXFLAGS = -Xpreprocessor -fopenmp -libomp -std=c++17 -Wall
TARGET = Simulation
SOURCES = Simulation.cpp FileManager.cpp body.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(SOURCES) -o $(TARGET) $(CXXFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
