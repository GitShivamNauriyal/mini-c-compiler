#include "../include/ast.h"
#include <iostream>

/**
 * @brief Helper function to print indentation spaces.
 * 
 * @param indent Number of spaces to print.
 */
void printIndent(int indent)
{
    for (int i = 0; i < indent; i++)
        std::cout << " ";
}

/**
 * @brief Prints the BlockNode and all its contained statements.
 * 
 * @param indent Number of spaces to indent for hierarchical display.
 */
void BlockNode::print(int indent) const
{
    printIndent(indent);
    std::cout << "Block {\n";
    for (auto &stmt : statements)
    {
        stmt->print(indent + 2);
    }
    printIndent(indent);
    std::cout << "}\n";
}

/**
 * @brief Prints a DeclarationNode, including its type, name, and initialization.
 * 
 * @param indent Number of spaces to indent.
 */
void DeclarationNode::print(int indent) const
{
    printIndent(indent);
    std::cout << "Declaration: " << typeToString(type) << " " << identifier << "\n";
    if (initialization)
    {
        printIndent(indent + 1);
        std::cout << "Initialized to:\n";
        initialization->print(indent + 4);
    }
}

/**
 * @brief Prints an AssignmentNode.
 * 
 * @param indent Number of spaces to indent.
 */
void AssignmentNode::print(int indent) const
{
    printIndent(indent);
    std::cout << "Assignment: " << identifier << " =\n";
    expression->print(indent + 4);
}

/**
 * @brief Prints an IfNode, including its condition, if-block, and optional else-block.
 * 
 * @param indent Number of spaces to indent.
 */
void IfNode::print(int indent) const
{
    printIndent(indent);
    std::cout << "If Statement:\n";
    printIndent(indent + 2);
    std::cout << "Condition:\n";
    condition->print(indent + 4);
    printIndent(indent + 2);
    std::cout << "If Block:\n";
    ifBlock->print(indent + 4);
    if (elseBlock)
    {
        printIndent(indent + 2);
        std::cout << "Else Block:\n";
        elseBlock->print(indent + 4);
    }
}

/**
 * @brief Prints a WhileNode.
 * 
 * @param indent Number of spaces to indent.
 */
void WhileNode::print(int indent) const
{
    printIndent(indent);
    std::cout << "While Statement:\n";
    printIndent(indent + 2);
    std::cout << "Condition:\n";
    condition->print(indent + 4);
    printIndent(indent + 2);
    std::cout << "Body:\n";
    body->print(indent + 4);
}

/**
 * @brief Prints a ForNode.
 * 
 * @param indent Number of spaces to indent.
 */
void ForNode::print(int indent) const
{
    printIndent(indent);
    std::cout << "For Statement:\n";
    printIndent(indent + 2);
    std::cout << "Initialization:\n";
    initialization->print(indent + 4);
    printIndent(indent + 2);
    std::cout << "Condition:\n";
    condition->print(indent + 4);
    printIndent(indent + 2);
    std::cout << "Update:\n";
    update->print(indent + 4);
    printIndent(indent + 2);
    std::cout << "Body:\n";
    body->print(indent + 4);
}

/**
 * @brief Prints a BinaryOpNode representing arithmetic or logical operations.
 * 
 * @param indent Number of spaces to indent.
 */
void BinaryOpNode::print(int indent) const
{
    printIndent(indent);
    std::cout << "BinaryOp: " << op << "\n";
    printIndent(indent + 1);
    std::cout << "Left:\n";
    left->print(indent + 8);
    printIndent(indent + 1);
    std::cout << "Right:\n";
    right->print(indent + 8);
}

/**
 * @brief Prints a NumberNode (literal integer or float).
 * 
 * @param indent Number of spaces to indent.
 */
void NumberNode::print(int indent) const
{
    printIndent(indent);
    std::cout << "Number: " << value << " (" << (isFloat ? "float" : "int") << ")\n";
}

/**
 * @brief Prints a VariableNode (identifier usage).
 * 
 * @param indent Number of spaces to indent.
 */
void VariableNode::print(int indent) const
{
    printIndent(indent);
    std::cout << "Variable: " << name << "\n";
}
