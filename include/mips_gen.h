#ifndef MIPS_GEN_H
#define MIPS_GEN_H

#include "optimizer.h"
#include <vector>
#include <string>
#include <map>
#include <iostream>

/**
 * @class MipsGenerator
 * @brief Translates Three-Address Code (TAC) into MIPS Assembly.
 */
class MipsGenerator {
private:
    std::map<std::string, int> stackOffsets; 
    int stackPtr = 0;

    int getOffset(const std::string& var);

public:
    void generate(const std::vector<Instruction>& instructions, std::ostream& out = std::cout);
};

#endif
