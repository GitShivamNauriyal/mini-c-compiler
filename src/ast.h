#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "symbol_table.h"

class ASTNode;
using ASTNodePtr = std::shared_ptr<ASTNode>;

class ASTNode
{
public:
    virtual ~ASTNode() = default;

    // Virtual interface forces every child node to define its own logic for
    // debugging (print) and eventually Intermediate Code Generation (ICG).
    virtual void print(int indent = 0) const = 0;
};

// Represents the root of the file or a `{ ... }` block containing multiple lines of code.
class BlockNode : public ASTNode
{
public:
    std::vector<ASTNodePtr> statements;

    void addStatement(ASTNodePtr stmt) { statements.push_back(stmt); }
    void print(int indent = 0) const override;
};

// Represents variable declarations: e.g., 'int x;'
// The semantic analyzer uses this to populate the Symbol Table.
class DeclarationNode : public ASTNode
{
public:
    DataType type;
    std::string identifier;

    DeclarationNode(DataType t, const std::string &id) : type(t), identifier(id) {}
    void print(int indent = 0) const override;
};

// Represents variable usage: e.g., 'x' inside 'x = y + 5;'
// Triggers a lookup in the Symbol Table to verify the variable exists.
class VariableNode : public ASTNode
{
public:
    std::string name;

    VariableNode(const std::string &n) : name(n) {}
    void print(int indent = 0) const override;
};

// Represents raw constants: e.g., '10' or '3.14'
// Stored as strings here; the semantic analyzer casts them based on the target variable.
class NumberNode : public ASTNode
{
public:
    std::string value;
    bool isFloat; // Used to trigger type-mismatch errors if assigned to an int

    NumberNode(const std::string &val, bool isF) : value(val), isFloat(isF) {}
    void print(int indent = 0) const override;
};

// Represents operations: e.g., '+', '-', '==', '<'
// The left and right children must be evaluated before the operation is applied.
class BinaryOpNode : public ASTNode
{
public:
    std::string op;
    ASTNodePtr left;
    ASTNodePtr right;

    BinaryOpNode(const std::string &o, ASTNodePtr l, ASTNodePtr r) : op(o), left(l), right(r) {}
    void print(int indent = 0) const override;
};

// Represents assignments: e.g., 'x = 10;'
// The semantic analyzer verifies the type of 'left' matches the evaluated type of 'right'.
class AssignmentNode : public ASTNode
{
public:
    std::string identifier;
    ASTNodePtr expression;

    AssignmentNode(const std::string &id, ASTNodePtr expr) : identifier(id), expression(expr) {}
    void print(int indent = 0) const override;
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
    void print(int indent = 0) const override;
};

// Represents 'while' loops. The code generator will create assembly 'jump' instructions pointing back to 'condition'.
class WhileNode : public ASTNode
{
public:
    ASTNodePtr condition;
    ASTNodePtr body;

    WhileNode(ASTNodePtr cond, ASTNodePtr b) : condition(cond), body(b) {}
    void print(int indent = 0) const override;
};

// Represents 'for' loops. Internally mapped by the code generator as an initialization followed by a while loop.
class ForNode : public ASTNode
{
public:
    ASTNodePtr initialization;
    ASTNodePtr condition;
    ASTNodePtr update;
    ASTNodePtr body;

    ForNode(ASTNodePtr init, ASTNodePtr cond, ASTNodePtr upd, ASTNodePtr b)
        : initialization(init), condition(cond), update(upd), body(b) {}
    void print(int indent = 0) const override;
};

#endif