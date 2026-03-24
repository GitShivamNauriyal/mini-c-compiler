#include "codegen.h"
#include <iostream>

std::string CodeGenerator::newTemp()
{
    return "t" + std::to_string(++tempCount);
}

std::string CodeGenerator::newLabel()
{
    return "L" + std::to_string(++labelCount);
}

void CodeGenerator::generate(ASTNodePtr root)
{
    std::cout << "\n================ THREE-ADDRESS CODE ================\n";
    generateStmt(root);
    std::cout << "====================================================\n\n";
}

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
        std::cout << temp << " = " << left << " " << binOp->op << " " << right << "\n";
        return temp;
    }
    return "";
}

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
            std::cout << decl->identifier << " = " << rhs << "\n";
        }
    }
    else if (auto assign = std::dynamic_pointer_cast<AssignmentNode>(node))
    {
        std::string rhs = generateExpr(assign->expression);
        std::cout << assign->identifier << " = " << rhs << "\n";
    }
    else if (auto ifStmt = std::dynamic_pointer_cast<IfNode>(node))
    {
        std::string cond = generateExpr(ifStmt->condition);
        std::string labelTrue = newLabel();
        std::string labelEnd = newLabel();

        std::cout << "if " << cond << " goto " << labelTrue << "\n";

        if (ifStmt->elseBlock)
        {
            std::string labelFalse = newLabel();
            std::cout << "goto " << labelFalse << "\n";

            std::cout << labelTrue << ":\n";
            generateStmt(ifStmt->ifBlock);
            std::cout << "goto " << labelEnd << "\n";

            std::cout << labelFalse << ":\n";
            generateStmt(ifStmt->elseBlock);
        }
        else
        {
            std::cout << "goto " << labelEnd << "\n";
            std::cout << labelTrue << ":\n";
            generateStmt(ifStmt->ifBlock);
        }
        std::cout << labelEnd << ":\n";
    }
    else if (auto whileStmt = std::dynamic_pointer_cast<WhileNode>(node))
    {
        std::string labelStart = newLabel();
        std::string labelTrue = newLabel();
        std::string labelEnd = newLabel();

        std::cout << labelStart << ":\n";
        std::string cond = generateExpr(whileStmt->condition);
        std::cout << "if " << cond << " goto " << labelTrue << "\n";
        std::cout << "goto " << labelEnd << "\n";

        std::cout << labelTrue << ":\n";
        generateStmt(whileStmt->body);
        std::cout << "goto " << labelStart << "\n"; // Loop back up

        std::cout << labelEnd << ":\n";
    }
    else if (auto forStmt = std::dynamic_pointer_cast<ForNode>(node))
    {
        // A for loop is essentially an initialization followed by a while loop
        generateStmt(forStmt->initialization);

        std::string labelStart = newLabel();
        std::string labelTrue = newLabel();
        std::string labelEnd = newLabel();

        std::cout << labelStart << ":\n";
        std::string cond = generateExpr(forStmt->condition);
        std::cout << "if " << cond << " goto " << labelTrue << "\n";
        std::cout << "goto " << labelEnd << "\n";

        std::cout << labelTrue << ":\n";
        generateStmt(forStmt->body);
        generateStmt(forStmt->update);
        std::cout << "goto " << labelStart << "\n";

        std::cout << labelEnd << ":\n";
    }
}