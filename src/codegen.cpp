#include "../include/codegen.h"
#include "../include/mips_gen.h"
#include <iostream>

/**
 * @brief Generates a new temporary variable name.
 */
std::string CodeGenerator::newTemp()
{
    return "t" + std::to_string(++tempCount);
}

/**
 * @brief Generates a new label name.
 */
std::string CodeGenerator::newLabel()
{
    return "L" + std::to_string(++labelCount);
}

/**
 * @brief Main entry point to generate three-address code and MIPS from the AST.
 */
void CodeGenerator::generate(ASTNodePtr root, bool optimize, 
                              std::ostream& tacOut,
                              std::ostream& optTacOut,
                              std::ostream& mipsOut)
{
    generateStmt(root);
    
    tacOut << "\n================ THREE-ADDRESS CODE ================\n";
    optimizer.print(tacOut);
    tacOut << "====================================================\n\n";

    if (optimize) {
        optimizer.optimize();

        optTacOut << "================ OPTIMIZED THREE-ADDRESS CODE ================\n";
        optimizer.print(optTacOut);
        optTacOut << "==============================================================\n\n";
    }

    // Generate MIPS Assembly from the (possibly optimized) instructions
    MipsGenerator mips;
    mips.generate(optimizer.instructions, mipsOut);
}

/**
 * @brief Generates TAC instructions for expression nodes.
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
        std::string left = generateExpr(binOp->left);
        std::string right = generateExpr(binOp->right);

        std::string temp = newTemp();
        optimizer.addInstruction(Instruction(temp, binOp->op, left, right));
        return temp;
    }
    return "";
}

/**
 * @brief Generates TAC instructions for statements.
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
        optimizer.addInstruction(Instruction(labelStart, "goto", "", ""));

        optimizer.addInstruction(Instruction(labelEnd));
    }
    else if (auto forStmt = std::dynamic_pointer_cast<ForNode>(node))
    {
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
