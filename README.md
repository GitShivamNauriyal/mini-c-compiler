# MiniC-Compiler

A multi-phase compiler for a subset of the C programming language, built from scratch using C++, Flex, and Bison.

## Overview
This project demonstrates the complete pipeline of compiler construction, translating high-level C-subset source code into Intermediate Representation (IR). It includes robust error handling for both syntax and semantic logic.

## Current Status: Phase 2 Completed (Syntax & AST)
The compiler currently successfully scans lexical tokens, validates C-subset grammar, and constructs an Abstract Syntax Tree (AST) in memory. 

### Implemented Features
* **Lexical Analysis:** Tokenizes keywords, identifiers, numbers (int/float), and operators.
* **Syntax Analysis:** Validates grammar for:
  * Variable declarations and inline initialization (e.g., `int x = 10;`).
  * Arithmetic operations (`+`, `-`, `*`, `/`) with correct operator precedence.
  * Control flow structures (`if-else` conditionals, `while` and `for` loops).
* **AST Generation:** Constructs and prints a hierarchical Abstract Syntax Tree for visual debugging.
* **Error Handling:** Precise line-number tracking for lexical and syntax errors.

### Planned Features (Next Phases)
* **Semantic Verification:** * Symbol Table management for variable scoping.
    * Type-checking and type-mismatch error reporting.
* **Intermediate Code:** Generates Three-Address Code (3AC) from the validated AST.

## Technology Stack
* **Language:** C++17 (Object-Oriented Design)
* **Lexer Generator:** Flex
* **Parser Generator:** Bison (GNU)
* **Build System:** GNU Make
* **Environment:** WSL (Linux / x86-64)

## Project Structure
* `src/`: Contains core compiler logic (Lexer, Parser, AST, Semantic Analyzer, Code Generator).
* `tests/`: Sample `.c` files containing valid and invalid syntax to test error reporting.
* `Makefile`: Build automation script.

## Getting Started

### Prerequisites
Ensure you have the required build tools installed on your Linux/WSL environment:
```bash
sudo apt update
sudo apt install flex bison build-essential g++ make
```

### Build Instructions
 To compile the project and generate the minic executable, run:
```Bash
make 
```
(To clean up generated C files and the executable, run make clean)
### Usage
Pass a C file as an argument to the compiler to analyze it:
```Bash
./minic tests/test_valid.c
```

## 👨‍💻 Author
* Shivam Nauriyal🗿
- **shivamnauriyal1224@gmail.com**.