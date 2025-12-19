# Makefile for Finance Tracker C++ Backend
# Data Structures & Applications Lab Project

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = finance_engine
SOURCES = main.cpp
HEADERS = hashmap.h linkedlist.h bst.h heap.h queue.h stack.h trie.h finance_engine.h

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

# Debug build
debug: CXXFLAGS = -std=c++17 -Wall -Wextra -g -DDEBUG
debug: $(TARGET)

.PHONY: all clean debug
