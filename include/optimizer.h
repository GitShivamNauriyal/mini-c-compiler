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
    std::string label; // For labels like L1:

    Instruction(std::string r, std::string o, std::string a1, std::string a2)
        : result(r), op(o), arg1(a1), arg2(a2) {}
    
    Instruction(std::string l) : label(l) {} // Label constructor

    void print() const {
        if (!label.empty()) {
            std::cout << label << ":\n";
        } else if (op == "=") {
            std::cout << result << " = " << arg1 << "\n";
        } else if (op == "if") {
            std::cout << "if " << arg1 << " goto " << result << "\n";
        } else if (op == "goto") {
            std::cout << "goto " << result << "\n";
        } else {
            std::cout << result << " = " << arg1 << " " << op << " " << arg2 << "\n";
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
    void print() const;

private:
    void constantFolding();
    void constantPropagation();
    void deadCodeElimination();
    void algebraicSimplification();
};

#endif
