CXX      := g++
CXXFLAGS := -g -std=c++17 -Iinclude

SRC := $(wildcard src/*.cpp src/utils/*.cpp src/gsw/*.cpp)
OBJ := $(SRC:.cpp=.o)

TARGET := exec

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
