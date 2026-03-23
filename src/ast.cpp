#include "ast.h"

// Helper function to quickly generate spaces for tree indentation
std::string getIndent(int indent)
{
    return std::string(indent, ' ');
}

void BlockNode::print(int indent) const
{
    std::cout << getIndent(indent) << "Block {" << "\n";
    for (const auto &stmt : statements)
    {
        if (stmt)
            stmt->print(indent + 2);
    }
    std::cout << getIndent(indent) << "}" << "\n";
}

void DeclarationNode::print(int indent) const
{
    std::string typeStr = (type == TYPE_INT) ? "int" : (type == TYPE_FLOAT) ? "float"
                                                                            : "char";
    std::cout << getIndent(indent) << "Declaration: " << typeStr << " " << identifier << "\n";
}

void VariableNode::print(int indent) const
{
    std::cout << getIndent(indent) << "Variable: " << name << "\n";
}

void NumberNode::print(int indent) const
{
    std::cout << getIndent(indent) << "Number: " << value
              << (isFloat ? " (float)" : " (int)") << "\n";
}

void BinaryOpNode::print(int indent) const
{
    std::cout << getIndent(indent) << "BinaryOp: " << op << "\n";
    std::cout << getIndent(indent) << " Left:\n";
    if (left)
        left->print(indent + 4);
    std::cout << getIndent(indent) << " Right:\n";
    if (right)
        right->print(indent + 4);
}

void AssignmentNode::print(int indent) const
{
    std::cout << getIndent(indent) << "Assignment: " << identifier << " =\n";
    if (expression)
        expression->print(indent + 2);
}

void IfNode::print(int indent) const
{
    std::cout << getIndent(indent) << "If Statement\n";
    std::cout << getIndent(indent) << " Condition:\n";
    if (condition)
        condition->print(indent + 2);

    std::cout << getIndent(indent) << " Then:\n";
    if (ifBlock)
        ifBlock->print(indent + 2);

    if (elseBlock)
    {
        std::cout << getIndent(indent) << " Else:\n";
        elseBlock->print(indent + 2);
    }
}

void WhileNode::print(int indent) const
{
    std::cout << getIndent(indent) << "While Loop\n";
    std::cout << getIndent(indent) << " Condition:\n";
    if (condition)
        condition->print(indent + 2);

    std::cout << getIndent(indent) << " Body:\n";
    if (body)
        body->print(indent + 2);
}

void ForNode::print(int indent) const
{
    std::cout << getIndent(indent) << "For Loop\n";
    std::cout << getIndent(indent) << " Init:\n";
    if (initialization)
        initialization->print(indent + 2);

    std::cout << getIndent(indent) << " Condition:\n";
    if (condition)
        condition->print(indent + 2);

    std::cout << getIndent(indent) << " Update:\n";
    if (update)
        update->print(indent + 2);

    std::cout << getIndent(indent) << " Body:\n";
    if (body)
        body->print(indent + 2);
}