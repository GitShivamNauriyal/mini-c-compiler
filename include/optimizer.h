#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <string>
#include <vector>
#include <iostream>

struct Instruction {
    std::string result;
    std::string op;
    std::string arg1;
    std::string arg2;
    std::string label; 

    Instruction(std::string r, std::string o, std::string a1, std::string a2)
        : result(r), op(o), arg1(a1), arg2(a2) {}
    
    Instruction(std::string l) : label(l) {}

    void print(std::ostream& out = std::cout) const {
        if (!label.empty()) {
            out << label << ":\n";
        } else if (op == "=") {
            out << result << " = " << arg1 << "\n";
        } else if (op == "if") {
            out << "if " << arg1 << " goto " << result << "\n";
        } else if (op == "goto") {
            out << "goto " << result << "\n";
        } else {
            out << result << " = " << arg1 << " " << op << " " << arg2 << "\n";
        }
    }
};

class Optimizer {
public:
    std::vector<Instruction> instructions;

    void addInstruction(Instruction instr) {
        instructions.push_back(instr);
    }

    void optimize();
    void print(std::ostream& out = std::cout) const;

private:
    void constantFolding();
    void constantPropagation();
    void deadCodeElimination();
    void algebraicSimplification();
    void commonSubexpressionElimination();
    void loopOptimization();
};

#endif
