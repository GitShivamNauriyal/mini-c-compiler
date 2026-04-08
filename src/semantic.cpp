#include "../include/semantic.h"
#include <iostream>

/**
 * @brief Reports a semantic error to the console and sets the error flag.
 *
 * @param message The error message to display.
 */
void SemanticAnalyzer::reportError(const std::string &message)
{
    std::cerr << "[ERROR] Semantic Error: " << message << "\n";
    hasError = true;
}

/**
 * @brief Entry point for semantic analysis on the AST.
 *
 * @param root Shared pointer to the root of the AST.
 * @return bool True if analysis succeeded without errors, false otherwise.
 */
bool SemanticAnalyzer::analyze(ASTNodePtr root)
{
    hasError = false;
    visit(root);
    return !hasError;
}

/**
 * @brief Determines the data type of an expression node.
 *
 * Performs recursive type checking and lookups variables in the symbol table.
 *
 * @param expr Shared pointer to the expression AST node.
 * @return DataType The resolved data type (INT, FLOAT, CHAR, or UNKNOWN).
 */
DataType SemanticAnalyzer::getExprType(ASTNodePtr expr)
{
    if (!expr)
        return TYPE_UNKNOWN;

    if (auto num = std::dynamic_pointer_cast<NumberNode>(expr))
    {
        return num->isFloat ? TYPE_FLOAT : TYPE_INT;
    }
    if (auto var = std::dynamic_pointer_cast<VariableNode>(expr))
    {
        SymbolInfo *info = symTab.lookup(var->name);
        if (info)
            return info->type;

        reportError("Undeclared variable '" + var->name + "' used in expression.");
        return TYPE_UNKNOWN;
    }
    if (auto binOp = std::dynamic_pointer_cast<BinaryOpNode>(expr))
    {
        DataType leftType = getExprType(binOp->left);
        DataType rightType = getExprType(binOp->right);

        // Comparison operators always return an integer (0 or 1)
        if (binOp->op == "==" || binOp->op == "!=" || binOp->op == "<" || 
            binOp->op == ">" || binOp->op == "<=" || binOp->op == ">=")
            return TYPE_INT;

        // If either side of a math operation is a float, the result is a float
        if (leftType == TYPE_FLOAT || rightType == TYPE_FLOAT)
            return TYPE_FLOAT;
        return TYPE_INT;
    }
    return TYPE_UNKNOWN;
}

/**
 * @brief Checks if a source type can be implicitly converted to a target type.
 *
 * @param target The expected data type.
 * @param source The actual data type of the expression.
 * @return bool True if compatible, false otherwise.
 */
bool isCompatible(DataType target, DataType source)
{
    if (target == source)
        return true;
    if (target == TYPE_FLOAT && source == TYPE_INT)
        return true;
    if (target == TYPE_INT && source == TYPE_CHAR)
        return true;
    if (target == TYPE_CHAR && source == TYPE_INT)
        return true;
    return false;
}

/**
 * @brief Recursively traverses the AST to perform semantic checks.
 *
 * Handles declarations, assignments, and control flow structures.
 *
 * @param node Shared pointer to the current AST node being visited.
 */
void SemanticAnalyzer::visit(ASTNodePtr node)
{
    if (!node)
        return;

    if (auto block = std::dynamic_pointer_cast<BlockNode>(node))
    {
        for (auto &stmt : block->statements)
            visit(stmt);
    }
    else if (auto decl = std::dynamic_pointer_cast<DeclarationNode>(node))
    {
        if (symTab.exists(decl->identifier))
        {
            reportError("Variable '" + decl->identifier + "' is already declared.");
        }
        else
        {
            symTab.insert(decl->identifier, decl->type, 0);
        }

        if (decl->initialization)
        {
            visit(decl->initialization);
            DataType initType = getExprType(decl->initialization);
            if (initType != TYPE_UNKNOWN && !isCompatible(decl->type, initType))
            {
                reportError("Type mismatch in initialization of '" + decl->identifier + "'.");
            }
        }
    }
    else if (auto assign = std::dynamic_pointer_cast<AssignmentNode>(node))
    {
        visit(assign->expression);

        SymbolInfo *info = symTab.lookup(assign->identifier);
        if (!info)
        {
            reportError("Assignment to undeclared variable '" + assign->identifier + "'.");
        }
        else
        {
            DataType exprType = getExprType(assign->expression);
            if (exprType != TYPE_UNKNOWN && !isCompatible(info->type, exprType))
            {
                reportError("Type mismatch: Cannot assign to '" + assign->identifier + "'.");
            }
        }
    }
    else if (auto ifStmt = std::dynamic_pointer_cast<IfNode>(node))
    {
        visit(ifStmt->condition);
        visit(ifStmt->ifBlock);
        if (ifStmt->elseBlock)
            visit(ifStmt->elseBlock);
    }
    else if (auto whileStmt = std::dynamic_pointer_cast<WhileNode>(node))
    {
        visit(whileStmt->condition);
        visit(whileStmt->body);
    }
    else if (auto forStmt = std::dynamic_pointer_cast<ForNode>(node))
    {
        visit(forStmt->initialization);
        visit(forStmt->condition);
        visit(forStmt->update);
        visit(forStmt->body);
    }
}
