# Makefile for compiling and running parsing table maker

# Compiler
CXX = g++
CXXFLAGS = -Wall -g

# Source files
SRCS = table_maker.cpp

# Header files
HEADERS = nonterminals.h terminals.h rules.h

# Executable name
TARGET = table_maker

# Default rule
all: $(TARGET)

# Rule to create the executable
$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Clean rule to remove compiled files
clean:
	rm -f $(TARGET)

# Run rule to execute the program
run: $(TARGET)
	./$(TARGET) > $(TARGET).out

.PHONY: all clean run
