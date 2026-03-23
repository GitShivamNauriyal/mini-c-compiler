# MiniC-Compiler

A multi-phase compiler for a subset of the C programming language, built from scratch using C++, Flex, and Bison.

## Overview
This project demonstrates the complete pipeline of compiler construction, translating high-level C-subset source code into Intermediate Representation (IR). It includes robust error handling for both syntax and semantic logic.

## Planned Features
* **Data Types:** Support for `int`, `float`, and `char`.
* **Control Flow:** Implementation of `if-else` conditionals and loops (`for`, `while`).
* **Arithmetic:** Support for basic arithmetic operations (`+`, `-`, `*`, `/`) with correct operator precedence.
* **Semantic Verification:** * Symbol Table management for variable scoping.
    * Type-checking and type mismatch error reporting.
* **AST Generation:** Constructs a logical Abstract Syntax Tree for code translation.
* **Intermediate Code:** Generates Three-Address Code (3AC).

## Technology Stack
* **Language:** C++ (Object-Oriented Design)
* **Lexer Generator:** Flex
* **Parser Generator:** Bison (GNU)
* **Build System:** GNU Make
* **Environment:** WSL (Kali Linux)

## Project Structure
* `src/`: Contains core compiler logic (Lexer, Parser, AST, Semantic Analyzer, Code Generator).
* `include/`: Shared header files.
* `tests/`: Sample `.c` files containing valid and invalid syntax to test error reporting.

## Getting Started
*(Build instructions and usage commands will be added here once the Makefile is finalized).*