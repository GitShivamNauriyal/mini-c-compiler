#include "../include/optimizer.h"
#include <map>
#include <set>
#include <algorithm>

void Optimizer::print() const
{
    for (const auto &instr : instructions)
    {
        instr.print();
    }
}

bool isNumber(const std::string &s)
{
    if (s.empty())
        return false;
    if (s[0] == '-' && s.length() > 1)
        return std::isdigit(s[1]);
    return std::isdigit(s[0]);
}

void Optimizer::optimize()
{
    bool changed = true;
    int iterations = 0;
    while (changed && iterations < 10)
    {
        changed = false;
        std::vector<Instruction> oldInstrs = instructions;

        constantPropagation();
        constantFolding();
        algebraicSimplification();
        deadCodeElimination();

        if (instructions.size() != oldInstrs.size())
        {
            changed = true;
        }
        else
        {
            for (size_t i = 0; i < instructions.size(); i++)
            {
                if (instructions[i].op != oldInstrs[i].op ||
                    instructions[i].arg1 != oldInstrs[i].arg1 ||
                    instructions[i].arg2 != oldInstrs[i].arg2 ||
                    instructions[i].result != oldInstrs[i].result)
                {
                    changed = true;
                    break;
                }
            }
        }
        iterations++;
    }
}

void Optimizer::constantPropagation()
{
    std::map<std::string, std::string> constants;
    for (auto &instr : instructions)
    {
        // If we see x = 5, record it
        if (instr.op == "=" && isNumber(instr.arg1))
        {
            constants[instr.result] = instr.arg1;
        }
        else
        {
            // Replace args if they are known constants
            if (constants.count(instr.arg1))
                instr.arg1 = constants[instr.arg1];
            if (constants.count(instr.arg2))
                instr.arg2 = constants[instr.arg2];

            // If the variable is reassigned to something non-constant, remove from map
            if (instr.op != "=" || !isNumber(instr.arg1))
            {
                constants.erase(instr.result);
            }
        }
    }
}

void Optimizer::constantFolding()
{
    for (auto &instr : instructions)
    {
        if (isNumber(instr.arg1) && isNumber(instr.arg2))
        {
            double a = std::stod(instr.arg1);
            double b = std::stod(instr.arg2);
            double res = 0;
            bool folded = false;

            if (instr.op == "+")
            {
                res = a + b;
                folded = true;
            }
            else if (instr.op == "-")
            {
                res = a - b;
                folded = true;
            }
            else if (instr.op == "*")
            {
                res = a * b;
                folded = true;
            }
            else if (instr.op == "/")
            {
                if (b != 0)
                {
                    res = a / b;
                    folded = true;
                }
            }

            if (folded)
            {
                std::string resStr = std::to_string(res);
                if (resStr.find('.') != std::string::npos)
                {
                    resStr.erase(resStr.find_last_not_of('0') + 1, std::string::npos);
                    if (resStr.back() == '.')
                        resStr.pop_back();
                }
                instr.arg1 = resStr;
                instr.op = "=";
                instr.arg2 = "";
            }
        }
    }
}

void Optimizer::algebraicSimplification()
{
    for (auto &instr : instructions)
    {
        if (instr.op == "+")
        {
            if (instr.arg1 == "0")
            {
                instr.op = "=";
                instr.arg1 = instr.arg2;
                instr.arg2 = "";
            }
            else if (instr.arg2 == "0")
            {
                instr.op = "=";
                instr.arg2 = "";
            }
        }
        else if (instr.op == "*")
        {
            if (instr.arg1 == "1")
            {
                instr.op = "=";
                instr.arg1 = instr.arg2;
                instr.arg2 = "";
            }
            else if (instr.arg2 == "1")
            {
                instr.op = "=";
                instr.arg2 = "";
            }
            else if (instr.arg1 == "0" || instr.arg2 == "0")
            {
                instr.op = "=";
                instr.arg1 = "0";
                instr.arg2 = "";
            }
        }
    }
}

void Optimizer::deadCodeElimination()
{
    std::set<std::string> usedVars;
    for (const auto &instr : instructions)
    {
        if (!instr.label.empty())
            continue;
        if (instr.op == "if")
        {
            usedVars.insert(instr.arg1);
        }
        else if (instr.op == "goto")
        {
            // result is the label, not a variable
        }
        else
        {
            if (!isNumber(instr.arg1))
                usedVars.insert(instr.arg1);
            if (!isNumber(instr.arg2) && !instr.arg2.empty())
                usedVars.insert(instr.arg2);
        }
    }

    std::vector<Instruction> nextInstrs;
    for (const auto &instr : instructions)
    {
        // Only eliminate temporary variables (t1, t2...)
        // Never eliminate assignments to user variables (x, y...) unless we have full data flow
        if (!instr.result.empty() && instr.result[0] == 't')
        {
            if (usedVars.find(instr.result) == usedVars.end())
            {
                continue;
            }
        }
        nextInstrs.push_back(instr);
    }
    instructions = nextInstrs;
}
