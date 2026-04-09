#include "../include/mips_gen.h"
#include <iostream>

/**
 * @brief Helper to check if a string represents a numerical literal.
 */
static bool isNumber(const std::string &s)
{
    if (s.empty())
        return false;
    if (s[0] == '-' && s.length() > 1)
        return std::isdigit(s[1]);
    return std::isdigit(s[0]);
}

/**
 * @brief Gets the stack offset for a variable.
 */
int MipsGenerator::getOffset(const std::string &var)
{
    if (stackOffsets.find(var) == stackOffsets.end())
    {
        stackOffsets[var] = stackPtr;
        stackPtr -= 4;
    }
    return stackOffsets[var];
}

/**
 * @brief Generates MIPS assembly from a list of TAC instructions.
 */
void MipsGenerator::generate(const std::vector<Instruction> &instructions, std::ostream &out)
{
    out << "\n================ GENERATING MIPS ASSEMBLY ================\n";
    out << ".data\n";
    out << ".text\n";
    out << ".globl main\n\n";
    out << "main:\n";
    out << "  # Initialize stack pointer and frame pointer\n";
    out << "  move $fp, $sp\n\n";

    for (const auto &instr : instructions)
    {
        if (!instr.label.empty())
        {
            out << instr.label << ":\n";
            continue;
        }

        if (instr.op == "=")
        {
            if (isNumber(instr.arg1))
            {
                out << "  li $t0, " << instr.arg1 << "  # Load constant\n";
            }
            else
            {
                out << "  lw $t0, " << getOffset(instr.arg1) << "($fp)  # Load " << instr.arg1 << "\n";
            }
            out << "  sw $t0, " << getOffset(instr.result) << "($fp)  # Store to " << instr.result << "\n";
        }
        else if (instr.op == "+" || instr.op == "-" || instr.op == "*" || instr.op == "/")
        {
            if (isNumber(instr.arg1))
                out << "  li $t0, " << instr.arg1 << "\n";
            else
                out << "  lw $t0, " << getOffset(instr.arg1) << "($fp)\n";

            if (isNumber(instr.arg2))
                out << "  li $t1, " << instr.arg2 << "\n";
            else
                out << "  lw $t1, " << getOffset(instr.arg2) << "($fp)\n";

            if (instr.op == "+")
                out << "  add $t2, $t0, $t1\n";
            else if (instr.op == "-")
                out << "  sub $t2, $t0, $t1\n";
            else if (instr.op == "*")
                out << "  mul $t2, $t0, $t1\n";
            else if (instr.op == "/")
                out << "  div $t0, $t1\n  mflo $t2\n";

            out << "  sw $t2, " << getOffset(instr.result) << "($fp)\n";
        }
        else if (instr.op == "if")
        {
            out << "  lw $t0, " << getOffset(instr.arg1) << "($fp)\n";
            out << "  bnez $t0, " << instr.result << "  # Branch if non-zero\n";
        }
        else if (instr.op == "goto")
        {
            out << "  j " << instr.result << "\n";
        }
        else if (instr.op == "==" || instr.op == "!=" || instr.op == "<" || instr.op == ">" || instr.op == "<=" || instr.op == ">=")
        {
            if (isNumber(instr.arg1))
                out << "  li $t0, " << instr.arg1 << "\n";
            else
                out << "  lw $t0, " << getOffset(instr.arg1) << "($fp)\n";

            if (isNumber(instr.arg2))
                out << "  li $t1, " << instr.arg2 << "\n";
            else
                out << "  lw $t1, " << getOffset(instr.arg2) << "($fp)\n";

            if (instr.op == "==")
                out << "  seq $t2, $t0, $t1\n";
            else if (instr.op == "!=")
                out << "  sne $t2, $t0, $t1\n";
            else if (instr.op == "<")
                out << "  slt $t2, $t0, $t1\n";
            else if (instr.op == ">")
                out << "  sgt $t2, $t0, $t1\n";
            else if (instr.op == "<=")
                out << "  sle $t2, $t0, $t1\n";
            else if (instr.op == ">=")
                out << "  sge $t2, $t0, $t1\n";

            out << "  sw $t2, " << getOffset(instr.result) << "($fp)\n";
        }
    }

    out << "\n  # Exit Program (System Call 10)\n";
    out << "  li $v0, 10\n";
    out << "  syscall\n";
    out << "==========================================================\n\n";
}
