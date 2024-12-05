CXX = clang++
CXXFLAGS = -Xpreprocessor -fopenmp -std=c++17 -Wall
LDFLAGS = -lomp
TARGET = Simulation
SOURCES = Simulation.cpp FileManager.cpp body.cpp vector.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean