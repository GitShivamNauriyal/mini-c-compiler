#include "../include/ast.h"
#include <iostream>

/**
 * @brief Helper function to print indentation spaces.
 * 
 * @param indent Number of spaces to print.
 * @param out Output stream.
 */
void printIndent(int indent, std::ostream& out)
{
    for (int i = 0; i < indent; i++)
        out << " ";
}

/**
 * @brief Prints the BlockNode and all its contained statements.
 */
void BlockNode::print(int indent, std::ostream& out) const
{
    printIndent(indent, out);
    out << "Block {\n";
    for (auto &stmt : statements)
    {
        stmt->print(indent + 2, out);
    }
    printIndent(indent, out);
    out << "}\n";
}

/**
 * @brief Prints a DeclarationNode.
 */
void DeclarationNode::print(int indent, std::ostream& out) const
{
    printIndent(indent, out);
    out << "Declaration: " << typeToString(type) << " " << identifier << "\n";
    if (initialization)
    {
        printIndent(indent + 1, out);
        out << "Initialized to:\n";
        initialization->print(indent + 4, out);
    }
}

/**
 * @brief Prints an AssignmentNode.
 */
void AssignmentNode::print(int indent, std::ostream& out) const
{
    printIndent(indent, out);
    out << "Assignment: " << identifier << " =\n";
    expression->print(indent + 4, out);
}

/**
 * @brief Prints an IfNode.
 */
void IfNode::print(int indent, std::ostream& out) const
{
    printIndent(indent, out);
    out << "If Statement:\n";
    printIndent(indent + 2, out);
    out << "Condition:\n";
    condition->print(indent + 4, out);
    printIndent(indent + 2, out);
    out << "If Block:\n";
    ifBlock->print(indent + 4, out);
    if (elseBlock)
    {
        printIndent(indent + 2, out);
        out << "Else Block:\n";
        elseBlock->print(indent + 4, out);
    }
}

/**
 * @brief Prints a WhileNode.
 */
void WhileNode::print(int indent, std::ostream& out) const
{
    printIndent(indent, out);
    out << "While Statement:\n";
    printIndent(indent + 2, out);
    out << "Condition:\n";
    condition->print(indent + 4, out);
    printIndent(indent + 2, out);
    out << "Body:\n";
    body->print(indent + 4, out);
}

/**
 * @brief Prints a ForNode.
 */
void ForNode::print(int indent, std::ostream& out) const
{
    printIndent(indent, out);
    out << "For Statement:\n";
    printIndent(indent + 2, out);
    out << "Initialization:\n";
    initialization->print(indent + 4, out);
    printIndent(indent + 2, out);
    out << "Condition:\n";
    condition->print(indent + 4, out);
    printIndent(indent + 2, out);
    out << "Update:\n";
    update->print(indent + 4, out);
    printIndent(indent + 2, out);
    out << "Body:\n";
    body->print(indent + 4, out);
}

/**
 * @brief Prints a BinaryOpNode.
 */
void BinaryOpNode::print(int indent, std::ostream& out) const
{
    printIndent(indent, out);
    out << "BinaryOp: " << op << "\n";
    printIndent(indent + 1, out);
    out << "Left:\n";
    left->print(indent + 8, out);
    printIndent(indent + 1, out);
    out << "Right:\n";
    right->print(indent + 8, out);
}

/**
 * @brief Prints a NumberNode.
 */
void NumberNode::print(int indent, std::ostream& out) const
{
    printIndent(indent, out);
    out << "Number: " << value << " (" << (isFloat ? "float" : "int") << ")\n";
}

/**
 * @brief Prints a VariableNode.
 */
void VariableNode::print(int indent, std::ostream& out) const
{
    printIndent(indent, out);
    out << "Variable: " << name << "\n";
}
