CXX := g++
CXXFLAGS := -g -O2 -pthread -Wall -Wextra -std=c++23

TARGET := ccwc
SRC := main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
		$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
