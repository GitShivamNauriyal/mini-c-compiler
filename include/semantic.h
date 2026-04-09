#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "symbol_table.h"
#include <memory>
#include <string>
#include <iostream>

class SemanticAnalyzer
{
private:
    SymbolTable symTab;
    bool hasError = false;

    void visit(ASTNodePtr node);

    DataType getExprType(ASTNodePtr expr);

    void reportError(const std::string &message);

public:
    bool analyze(ASTNodePtr root);

    void printSymbolTable(std::ostream& out = std::cout) { symTab.print(out); }
};

#endif
