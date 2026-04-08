#include "../include/codegen.h"
#include <iostream>

/**
 * @brief Generates a new temporary variable name (e.g., t1, t2).
 * 
 * @return std::string The name of the new temporary variable.
 */
std::string CodeGenerator::newTemp()
{
    return "t" + std::to_string(++tempCount);
}

/**
 * @brief Generates a new label name (e.g., L1, L2).
 * 
 * @return std::string The name of the new label.
 */
std::string CodeGenerator::newLabel()
{
    return "L" + std::to_string(++labelCount);
}

/**
 * @brief Main entry point to generate three-address code from the AST.
 * 
 * After generation, it invokes the optimizer and prints both the original and 
 * optimized intermediate code.
 * 
 * @param root Shared pointer to the root of the AST.
 */
void CodeGenerator::generate(ASTNodePtr root)
{
    generateStmt(root);
    
    std::cout << "\n================ THREE-ADDRESS CODE ================\n";
    optimizer.print();
    std::cout << "====================================================\n\n";

    std::cout << "[SUCCESS] Optimizing Code...\n";
    optimizer.optimize();

    std::cout << "================ OPTIMIZED THREE-ADDRESS CODE ================\n";
    optimizer.print();
    std::cout << "==============================================================\n\n";
}

/**
 * @brief Generates TAC instructions for expression nodes.
 * 
 * Recursively processes binary operations and returns the result variable.
 * 
 * @param expr Shared pointer to the expression AST node.
 * @return std::string The variable or constant representing the result of the expression.
 */
std::string CodeGenerator::generateExpr(ASTNodePtr expr)
{
    if (!expr)
        return "";

    if (auto num = std::dynamic_pointer_cast<NumberNode>(expr))
    {
        return num->value;
    }
    if (auto var = std::dynamic_pointer_cast<VariableNode>(expr))
    {
        return var->name;
    }
    if (auto binOp = std::dynamic_pointer_cast<BinaryOpNode>(expr))
    {
        // Evaluate left and right sides recursively
        std::string left = generateExpr(binOp->left);
        std::string right = generateExpr(binOp->right);

        // Generate a new temporary variable for the result
        std::string temp = newTemp();
        optimizer.addInstruction(Instruction(temp, binOp->op, left, right));
        return temp;
    }
    return "";
}

/**
 * @brief Generates TAC instructions for statements.
 * 
 * Handles declarations, assignments, and complex control structures (if, while, for).
 * 
 * @param node Shared pointer to the statement AST node.
 */
void CodeGenerator::generateStmt(ASTNodePtr node)
{
    if (!node)
        return;

    if (auto block = std::dynamic_pointer_cast<BlockNode>(node))
    {
        for (auto &stmt : block->statements)
            generateStmt(stmt);
    }
    else if (auto decl = std::dynamic_pointer_cast<DeclarationNode>(node))
    {
        if (decl->initialization)
        {
            std::string rhs = generateExpr(decl->initialization);
            optimizer.addInstruction(Instruction(decl->identifier, "=", rhs, ""));
        }
    }
    else if (auto assign = std::dynamic_pointer_cast<AssignmentNode>(node))
    {
        std::string rhs = generateExpr(assign->expression);
        optimizer.addInstruction(Instruction(assign->identifier, "=", rhs, ""));
    }
    else if (auto ifStmt = std::dynamic_pointer_cast<IfNode>(node))
    {
        std::string cond = generateExpr(ifStmt->condition);
        std::string labelTrue = newLabel();
        std::string labelEnd = newLabel();

        optimizer.addInstruction(Instruction(labelTrue, "if", cond, ""));

        if (ifStmt->elseBlock)
        {
            std::string labelFalse = newLabel();
            optimizer.addInstruction(Instruction(labelFalse, "goto", "", ""));

            optimizer.addInstruction(Instruction(labelTrue));
            generateStmt(ifStmt->ifBlock);
            optimizer.addInstruction(Instruction(labelEnd, "goto", "", ""));

            optimizer.addInstruction(Instruction(labelFalse));
            generateStmt(ifStmt->elseBlock);
        }
        else
        {
            optimizer.addInstruction(Instruction(labelEnd, "goto", "", ""));
            optimizer.addInstruction(Instruction(labelTrue));
            generateStmt(ifStmt->ifBlock);
        }
        optimizer.addInstruction(Instruction(labelEnd));
    }
    else if (auto whileStmt = std::dynamic_pointer_cast<WhileNode>(node))
    {
        std::string labelStart = newLabel();
        std::string labelTrue = newLabel();
        std::string labelEnd = newLabel();

        optimizer.addInstruction(Instruction(labelStart));
        std::string cond = generateExpr(whileStmt->condition);
        optimizer.addInstruction(Instruction(labelTrue, "if", cond, ""));
        optimizer.addInstruction(Instruction(labelEnd, "goto", "", ""));

        optimizer.addInstruction(Instruction(labelTrue));
        generateStmt(whileStmt->body);
        optimizer.addInstruction(Instruction(labelStart, "goto", "", "")); // Loop back up

        optimizer.addInstruction(Instruction(labelEnd));
    }
    else if (auto forStmt = std::dynamic_pointer_cast<ForNode>(node))
    {
        // A for loop is essentially an initialization followed by a while loop
        generateStmt(forStmt->initialization);

        std::string labelStart = newLabel();
        std::string labelTrue = newLabel();
        std::string labelEnd = newLabel();

        optimizer.addInstruction(Instruction(labelStart));
        std::string cond = generateExpr(forStmt->condition);
        optimizer.addInstruction(Instruction(labelTrue, "if", cond, ""));
        optimizer.addInstruction(Instruction(labelEnd, "goto", "", ""));

        optimizer.addInstruction(Instruction(labelTrue));
        generateStmt(forStmt->body);
        generateStmt(forStmt->update);
        optimizer.addInstruction(Instruction(labelStart, "goto", "", ""));

        optimizer.addInstruction(Instruction(labelEnd));
    }
}
