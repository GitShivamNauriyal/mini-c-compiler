# MiniC-Compiler

A multi-phase compiler for a subset of the C programming language, built from scratch using C++, Flex, and Bison.

## Overview
This project demonstrates the complete pipeline of compiler construction, translating high-level C-subset source code into Intermediate Representation (IR) and finally to MIPS32 assembly. It includes robust error handling for both syntax and semantic logic, along with an optimization engine.

## Project Status: Completed (Final Integration)
The compiler has reached its final development phase, successfully implementing the entire pipeline from source code to executable MIPS assembly.

### Implemented Features
*   **Lexical & Syntax Analysis:** Handles keywords, identifiers, literals, arithmetic, and control flow.
*   **Semantic Analysis:** Symbol table management, scoping, and type-checking (with support for implicit conversions between `int`, `float`, and `char`).
*   **Intermediate Code Generation:** Translates AST into Three-Address Code (TAC).
*   **Code Optimization:** A multi-pass optimizer that refines the generated IR.
*   **MIPS Assembly Generation:** Translates the (optimized) TAC into MIPS32 assembly.
*   **Artifact Generation:** Automatically saves outputs (AST, Symbol Table, TAC, MIPS) into separate files in the `output/` directory.

### Supported Syntax
*   **Data Types:** `int`, `float`, `char`.
*   **Control Flow:** `if-else`, `while` loops, `for` loops.
*   **Arithmetic Operators:** `+`, `-`, `*`, `/`.
*   **Relational Operators:** `==`, `!=`, `<`, `>`, `<=`, `>=`.
*   **Comments:** Support for single-line (`//`) and multi-line (`/* ... */`) comments.

### Optimization Techniques
The compiler implements several IR-level optimizations:
*   **Constant Folding:** Evaluates expressions like `2 + 3` into `5` at compile time.
*   **Constant Propagation:** Replaces variables with known constant values throughout the code.
*   **Algebraic Simplification:** Removes identities such as `x + 0`, `x * 1`, and `x * 0`.
*   **Dead Code Elimination (DCE):** Removes unused temporary variables and unreachable assignments.
*   **Common Subexpression Elimination (CSE):** Reuses results of redundant calculations (e.g., `a + b` calculated once).
*   **Loop-Invariant Code Motion (LICM):** Moves calculations that don't change inside a loop to the outside.

## 🚀 Future Roadmap
While the core compiler pipeline is complete, the following areas are identified for potential future development:
1.  **Arrays & Pointers:** Implementation of array indexing and pointer arithmetic for more complex data structures.
2.  **Function Support:** Support for function declarations, recursive calls, and proper stack frame management.
3.  **Advanced Register Allocation:** Shifting from a stack-based MIPS generation to an optimized register allocation strategy (e.g., Graph Coloring).
4.  **Standard Library Integration:** Adding built-in syscall wrappers for I/O operations like `print_int` or `read_char`.
5.  **Enhanced Error Recovery:** Improving parser resilience to report multiple errors in a single pass using Bison's error-handling tokens.

## Technology Stack
*   **Language:** C++17
*   **Tools:** Flex, Bison (GNU), GNU Make
*   **Environment:** Linux / WSL

## Project Structure
*   `src/`: Core implementation files (`.cpp`).
*   `include/`: Header files (`.h`).
*   `tests/`: Extensive test suite categorized into `valid/` and `errors/`.
*   `run.sh`: Unified script for building, cleaning, and testing.

## Getting Started

### Prerequisites
```bash
sudo apt update
sudo apt install flex bison build-essential g++ make
```

### Usage (run.sh)
The `run.sh` script is the primary way to interact with the project:

1.  **Build and Test All:** Compiles the project and runs all valid/error test cases with `-O1`.
    ```bash
    ./run.sh -b
    ```
2.  **Run Tests with Flags:**
    *   `./run.sh -t`: Runs tests with optimization ON.
    *   `./run.sh --test-no-opt`: Runs tests with optimization OFF.
3.  **Compile a Specific File:** Runs the compiler on a single source file.
    ```bash
    ./run.sh tests/valid/test1.c -O1
    ```

## 📂 Output Artifacts
After a successful compilation, the `output/` directory contains:
*   `ast.txt`: Hierarchical Abstract Syntax Tree.
*   `symbol_table.txt`: Table of all identifiers and their types.
*   `tac.txt`: Raw Three-Address Code.
*   `tac_optimized.txt`: The IR after optimization passes.
*   `mips.asm`: The final MIPS32 assembly code (compatible with **MARS** or **SPIM**).

## 👨‍💻 Author
*   🗿 Shivam Nauriyal
-   **shivamnauriyal1224@gmail.com**
