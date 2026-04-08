#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include "optimizer.h"
#include <string>

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
    void generate(ASTNodePtr root);
};

#endif