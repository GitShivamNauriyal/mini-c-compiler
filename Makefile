# Compiler and Flags
CXX = g++
CXXFLAGS = -Wall -std=c++17 -g -I$(SRC_DIR)

# Tools
FLEX = flex
BISON = bison

# Directories
SRC_DIR = src

# The final executable name
TARGET = minic

# C++ source files
CPP_SRCS = $(SRC_DIR)/ast.cpp $(SRC_DIR)/symbol_table.cpp $(SRC_DIR)/semantic.cpp $(SRC_DIR)/codegen.cpp $(SRC_DIR)/main.cpp
all: $(TARGET)

# Rule to link everything together into the final executable
$(TARGET): parser.tab.c lex.yy.c $(CPP_SRCS)
	$(CXX) $(CXXFLAGS) parser.tab.c lex.yy.c $(CPP_SRCS) -o $(TARGET)

# Rule to generate the Parser C code from Bison
parser.tab.c parser.tab.h: $(SRC_DIR)/parser.y
	$(BISON) -d -o parser.tab.c $(SRC_DIR)/parser.y

# Rule to generate the Lexer C code from Flex	
lex.yy.c: $(SRC_DIR)/lexer.l parser.tab.h
	$(FLEX) -o lex.yy.c $(SRC_DIR)/lexer.l

# Rule to clean up generated files (run 'make clean')
clean:
	rm -f $(TARGET) lex.yy.c parser.tab.c parser.tab.h