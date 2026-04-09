#include "../include/mips_gen.h"
#include <iostream>

/**
 * @brief Helper to check if a string represents a numerical literal.
 * 
 * @param s The string to check.
 * @return bool True if the string is a number, false otherwise.
 */
static bool isNumber(const std::string& s) {
    if (s.empty()) return false;
    if (s[0] == '-' && s.length() > 1) return std::isdigit(s[1]);
    return std::isdigit(s[0]);
}

/**
 * @brief Gets the stack offset for a variable, allocating it if necessary.
 * 
 * In this simplified model, every variable is given a unique 4-byte slot 
 * on the stack relative to the Frame Pointer ($fp).
 * 
 * @param var The variable name.
 * @return int The stack offset (e.g., 0, -4, -8).
 */
int MipsGenerator::getOffset(const std::string& var) {
    if (stackOffsets.find(var) == stackOffsets.end()) {
        stackOffsets[var] = stackPtr;
        stackPtr -= 4; 
    }
    return stackOffsets[var];
}

/**
 * @brief Generates MIPS assembly from a list of TAC instructions.
 * 
 * Translates Three-Address Code instructions into MIPS assembly using 
 * a load-store architecture. It handles arithmetic, logic, and control flow.
 * 
 * @param instructions The vector of TAC instructions to translate.
 */
void MipsGenerator::generate(const std::vector<Instruction>& instructions) {
    std::cout << "\n================ GENERATING MIPS ASSEMBLY ================\n";
    std::cout << ".data\n";
    std::cout << ".text\n";
    std::cout << ".globl main\n\n";
    std::cout << "main:\n";
    std::cout << "  # Initialize stack pointer and frame pointer\n";
    std::cout << "  move $fp, $sp\n\n";

    for (const auto& instr : instructions) {
        if (!instr.label.empty()) {
            std::cout << instr.label << ":\n";
            continue;
        }

        if (instr.op == "=") {
            // Assignment: result = arg1
            if (isNumber(instr.arg1)) {
                std::cout << "  li $t0, " << instr.arg1 << "  # Load constant\n";
            } else {
                std::cout << "  lw $t0, " << getOffset(instr.arg1) << "($fp)  # Load " << instr.arg1 << "\n";
            }
            std::cout << "  sw $t0, " << getOffset(instr.result) << "($fp)  # Store to " << instr.result << "\n";
        } 
        else if (instr.op == "+" || instr.op == "-" || instr.op == "*" || instr.op == "/") {
            // Binary Arithmetic Operations: result = arg1 op arg2
            
            // Load Arg 1 into $t0
            if (isNumber(instr.arg1)) std::cout << "  li $t0, " << instr.arg1 << "\n";
            else std::cout << "  lw $t0, " << getOffset(instr.arg1) << "($fp)\n";

            // Load Arg 2 into $t1
            if (isNumber(instr.arg2)) std::cout << "  li $t1, " << instr.arg2 << "\n";
            else std::cout << "  lw $t1, " << getOffset(instr.arg2) << "($fp)\n";

            if (instr.op == "+") std::cout << "  add $t2, $t0, $t1\n";
            else if (instr.op == "-") std::cout << "  sub $t2, $t0, $t1\n";
            else if (instr.op == "*") std::cout << "  mul $t2, $t0, $t1\n";
            else if (instr.op == "/") std::cout << "  div $t0, $t1\n  mflo $t2\n";

            // Store result from $t2 back to stack
            std::cout << "  sw $t2, " << getOffset(instr.result) << "($fp)\n";
        }
        else if (instr.op == "if") {
            // Conditional Branch: if arg1 goto result (label)
            std::cout << "  lw $t0, " << getOffset(instr.arg1) << "($fp)\n";
            std::cout << "  bnez $t0, " << instr.result << "  # Branch if non-zero\n";
        }
        else if (instr.op == "goto") {
            // Unconditional Jump: goto result (label)
            std::cout << "  j " << instr.result << "\n";
        }
        else if (instr.op == "==" || instr.op == "!=" || instr.op == "<" || instr.op == ">" || instr.op == "<=" || instr.op == ">=") {
            // Comparison Operations: result = arg1 op arg2 (yields 1 or 0)
            
            if (isNumber(instr.arg1)) std::cout << "  li $t0, " << instr.arg1 << "\n";
            else std::cout << "  lw $t0, " << getOffset(instr.arg1) << "($fp)\n";

            if (isNumber(instr.arg2)) std::cout << "  li $t1, " << instr.arg2 << "\n";
            else std::cout << "  lw $t1, " << getOffset(instr.arg2) << "($fp)\n";

            if (instr.op == "==") std::cout << "  seq $t2, $t0, $t1\n";
            else if (instr.op == "!=") std::cout << "  sne $t2, $t0, $t1\n";
            else if (instr.op == "<") std::cout << "  slt $t2, $t0, $t1\n";
            else if (instr.op == ">") std::cout << "  sgt $t2, $t0, $t1\n";
            else if (instr.op == "<=") std::cout << "  sle $t2, $t0, $t1\n";
            else if (instr.op == ">=") std::cout << "  sge $t2, $t0, $t1\n";

            std::cout << "  sw $t2, " << getOffset(instr.result) << "($fp)\n";
        }
    }

    std::cout << "\n  # Exit Program (System Call 10)\n";
    std::cout << "  li $v0, 10\n";
    std::cout << "  syscall\n";
    std::cout << "==========================================================\n\n";
}
