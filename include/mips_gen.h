#ifndef MIPS_GEN_H
#define MIPS_GEN_H

#include "optimizer.h"
#include <vector>
#include <string>
#include <map>

/**
 * @class MipsGenerator
 * @brief Translates Three-Address Code (TAC) into MIPS Assembly.
 */
class MipsGenerator {
private:
    std::map<std::string, int> stackOffsets; // Maps variables to stack positions
    int stackPtr = 0;

    /**
     * @brief Gets the stack offset for a variable, allocating it if necessary.
     */
    int getOffset(const std::string& var);

public:
    /**
     * @brief Generates MIPS assembly from a list of TAC instructions.
     * 
     * @param instructions The list of instructions to translate.
     */
    void generate(const std::vector<Instruction>& instructions);
};

#endif
