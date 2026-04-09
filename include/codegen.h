#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include "optimizer.h"
#include <string>
#include <iostream>

class CodeGenerator
{
private:
    int tempCount = 0;
    int labelCount = 0;
    Optimizer optimizer;

    // Helpers to generate new temporary variables (t1, t2) and labels (L1, L2)
    std::string newTemp();
    std::string newLabel();

    void generateStmt(ASTNodePtr node);
    std::string generateExpr(ASTNodePtr node);

public:
    /**
     * @brief Generates TAC and MIPS assembly.
     * 
     * @param root AST root.
     * @param optimize Enable optimization.
     * @param tacOut Stream for initial TAC.
     * @param optTacOut Stream for optimized TAC.
     * @param mipsOut Stream for MIPS assembly.
     */
    void generate(ASTNodePtr root, bool optimize = true, 
                  std::ostream& tacOut = std::cout,
                  std::ostream& optTacOut = std::cout,
                  std::ostream& mipsOut = std::cout);
};

#endif
