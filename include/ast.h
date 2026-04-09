#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "symbol_table.h"

class ASTNode;
using ASTNodePtr = std::shared_ptr<ASTNode>;

/**
 * @brief Helper function to print indentation spaces.
 * 
 * @param indent Number of spaces to print.
 * @param out Output stream (defaults to std::cout).
 */
void printIndent(int indent, std::ostream& out = std::cout);

class ASTNode
{
public:
    virtual ~ASTNode() = default;

    // Virtual interface for hierarchical display
    virtual void print(int indent = 0, std::ostream& out = std::cout) const = 0;
};

// Represents the root of the file or a `{ ... }` block containing multiple lines of code.
class BlockNode : public ASTNode
{
public:
    std::vector<ASTNodePtr> statements;

    void addStatement(ASTNodePtr stmt) { statements.push_back(stmt); }
    void print(int indent = 0, std::ostream& out = std::cout) const override;
};

// Represents variable declarations: e.g., 'int x;'
class DeclarationNode : public ASTNode
{
public:
    DataType type;
    std::string identifier;
    ASTNodePtr initialization;

    DeclarationNode(DataType t, const std::string &id, ASTNodePtr init = nullptr)
        : type(t), identifier(id), initialization(init) {}
    void print(int indent = 0, std::ostream& out = std::cout) const override;
};

// Represents variable usage: e.g., 'x' inside 'x = y + 5;'
class VariableNode : public ASTNode
{
public:
    std::string name;

    VariableNode(const std::string &n) : name(n) {}
    void print(int indent = 0, std::ostream& out = std::cout) const override;
};

// Represents raw constants: e.g., '10' or '3.14'
class NumberNode : public ASTNode
{
public:
    std::string value;
    bool isFloat; 

    NumberNode(const std::string &val, bool isF) : value(val), isFloat(isF) {}
    void print(int indent = 0, std::ostream& out = std::cout) const override;
};

// Represents operations: e.g., '+', '-', '==', '<'
class BinaryOpNode : public ASTNode
{
public:
    std::string op;
    ASTNodePtr left;
    ASTNodePtr right;

    BinaryOpNode(const std::string &o, ASTNodePtr l, ASTNodePtr r) : op(o), left(l), right(r) {}
    void print(int indent = 0, std::ostream& out = std::cout) const override;
};

// Represents assignments: e.g., 'x = 10;'
class AssignmentNode : public ASTNode
{
public:
    std::string identifier;
    ASTNodePtr expression;

    AssignmentNode(const std::string &id, ASTNodePtr expr) : identifier(id), expression(expr) {}
    void print(int indent = 0, std::ostream& out = std::cout) const override;
};

// Represents conditionals. The 'elseBlock' is optional (can be nullptr).
class IfNode : public ASTNode
{
public:
    ASTNodePtr condition;
    ASTNodePtr ifBlock;
    ASTNodePtr elseBlock;

    IfNode(ASTNodePtr cond, ASTNodePtr ifB, ASTNodePtr elseB = nullptr)
        : condition(cond), ifBlock(ifB), elseBlock(elseB) {}
    void print(int indent = 0, std::ostream& out = std::cout) const override;
};

// Represents 'while' loops. 
class WhileNode : public ASTNode
{
public:
    ASTNodePtr condition;
    ASTNodePtr body;

    WhileNode(ASTNodePtr cond, ASTNodePtr b) : condition(cond), body(b) {}
    void print(int indent = 0, std::ostream& out = std::cout) const override;
};

// Represents 'for' loops. 
class ForNode : public ASTNode
{
public:
    ASTNodePtr initialization;
    ASTNodePtr condition;
    ASTNodePtr update;
    ASTNodePtr body;

    ForNode(ASTNodePtr init, ASTNodePtr cond, ASTNodePtr upd, ASTNodePtr b)
        : initialization(init), condition(cond), update(upd), body(b) {}
    void print(int indent = 0, std::ostream& out = std::cout) const override;
};

#endif
