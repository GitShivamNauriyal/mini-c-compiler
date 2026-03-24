#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "symbol_table.h"
#include <memory>
#include <string>

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

    void printSymbolTable() { symTab.printTable(); }
};

#endif