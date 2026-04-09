#include "../include/optimizer.h"
#include <map>
#include <set>
#include <algorithm>

/**
 * @brief Prints all instructions in the current instruction set.
 */
void Optimizer::print(std::ostream& out) const {
    for (const auto& instr : instructions) {
        instr.print(out);
    }
}

/**
 * @brief Helper function to check if a string represents a numerical literal.
 */
bool isNumber(const std::string& s) {
    if (s.empty()) return false;
    if (s[0] == '-' && s.length() > 1) return std::isdigit(s[1]);
    return std::isdigit(s[0]);
}

/**
 * @brief Main optimization loop that runs multiple passes.
 */
void Optimizer::optimize() {
    bool changed = true;
    int iterations = 0;
    while (changed && iterations < 10) {
        changed = false;
        std::vector<Instruction> oldInstrs = instructions;
        
        loopOptimization();
        constantPropagation();
        constantFolding();
        algebraicSimplification();
        commonSubexpressionElimination();
        deadCodeElimination();
        
        // Check if any change occurred in the instructions
        if (instructions.size() != oldInstrs.size()) {
            changed = true;
        } else {
            for (size_t i = 0; i < instructions.size(); i++) {
                if (instructions[i].op != oldInstrs[i].op || 
                    instructions[i].arg1 != oldInstrs[i].arg1 || 
                    instructions[i].arg2 != oldInstrs[i].arg2 ||
                    instructions[i].result != oldInstrs[i].result ||
                    instructions[i].label != oldInstrs[i].label) {
                    changed = true;
                    break;
                }
            }
        }
        iterations++;
    }
}

/**
 * @brief Performs constant propagation pass.
 */
void Optimizer::constantPropagation() {
    std::map<std::string, std::string> constants;
    for (auto& instr : instructions) {
        if (instr.op == "=" && isNumber(instr.arg1)) {
            constants[instr.result] = instr.arg1;
        } else {
            if (constants.count(instr.arg1)) instr.arg1 = constants[instr.arg1];
            if (constants.count(instr.arg2)) instr.arg2 = constants[instr.arg2];
            
            if (instr.op != "=" || !isNumber(instr.arg1)) {
                 constants.erase(instr.result);
            }
        }
    }
}

/**
 * @brief Performs constant folding pass.
 */
void Optimizer::constantFolding() {
    for (auto& instr : instructions) {
        if (isNumber(instr.arg1) && isNumber(instr.arg2)) {
            double a = std::stod(instr.arg1);
            double b = std::stod(instr.arg2);
            double res = 0;
            bool folded = false;

            if (instr.op == "+") { res = a + b; folded = true; }
            else if (instr.op == "-") { res = a - b; folded = true; }
            else if (instr.op == "*") { res = a * b; folded = true; }
            else if (instr.op == "/") { if (b != 0) { res = a / b; folded = true; } }

            if (folded) {
                std::string resStr = std::to_string(res);
                if (resStr.find('.') != std::string::npos) {
                    resStr.erase(resStr.find_last_not_of('0') + 1, std::string::npos);
                    if (resStr.back() == '.') resStr.pop_back();
                }
                instr.arg1 = resStr;
                instr.op = "=";
                instr.arg2 = "";
            }
        }
    }
}

/**
 * @brief Performs algebraic simplification pass.
 */
void Optimizer::algebraicSimplification() {
    for (auto& instr : instructions) {
        if (instr.op == "+") {
            if (instr.arg1 == "0") { instr.op = "="; instr.arg1 = instr.arg2; instr.arg2 = ""; }
            else if (instr.arg2 == "0") { instr.op = "="; instr.arg2 = ""; }
        } else if (instr.op == "*") {
            if (instr.arg1 == "1") { instr.op = "="; instr.arg1 = instr.arg2; instr.arg2 = ""; }
            else if (instr.arg2 == "1") { instr.op = "="; instr.arg2 = ""; }
            else if (instr.arg1 == "0" || instr.arg2 == "0") { instr.op = "="; instr.arg1 = "0"; instr.arg2 = ""; }
        }
    }
}

/**
 * @brief Performs Common Subexpression Elimination (CSE).
 */
void Optimizer::commonSubexpressionElimination() {
    std::map<std::string, std::string> exprToVar;
    for (auto& instr : instructions) {
        if (!instr.op.empty() && instr.op != "=" && instr.op != "if" && instr.op != "goto" && instr.label.empty()) {
            std::string exprKey = instr.arg1 + " " + instr.op + " " + instr.arg2;
            
            if (exprToVar.count(exprKey)) {
                instr.op = "=";
                instr.arg1 = exprToVar[exprKey];
                instr.arg2 = "";
            } else {
                exprToVar[exprKey] = instr.result;
            }
        }
        
        if (instr.op == "=" && !instr.result.empty() && instr.result[0] != 't') {
            exprToVar.clear(); 
        }
    }
}

/**
 * @brief Performs Loop Optimization (primarily Loop-Invariant Code Motion).
 */
void Optimizer::loopOptimization() {
    std::map<std::string, int> labelToPos;
    for (int i = 0; i < (int)instructions.size(); i++) {
        if (!instructions[i].label.empty()) {
            labelToPos[instructions[i].label] = i;
        }
    }

    for (int i = 0; i < (int)instructions.size(); i++) {
        if (instructions[i].op == "goto" && labelToPos.count(instructions[i].result)) {
            int target = labelToPos[instructions[i].result];
            if (target < i) {
                std::set<std::string> modifiedInLoop;
                for (int j = target; j <= i; j++) {
                    if (!instructions[j].result.empty() && instructions[j].op != "if") {
                        modifiedInLoop.insert(instructions[j].result);
                    }
                }

                std::vector<int> invariantIndices;
                for (int j = target + 1; j < i; j++) {
                    const auto& instr = instructions[j];
                    if (!instr.op.empty() && instr.op != "if" && instr.op != "goto" && instr.label.empty()) {
                        bool arg1Invariant = isNumber(instr.arg1) || modifiedInLoop.find(instr.arg1) == modifiedInLoop.end();
                        bool arg2Invariant = instr.arg2.empty() || isNumber(instr.arg2) || modifiedInLoop.find(instr.arg2) == modifiedInLoop.end();
                        
                        if (arg1Invariant && arg2Invariant) {
                            invariantIndices.push_back(j);
                            modifiedInLoop.erase(instr.result); 
                        }
                    }
                }

                if (!invariantIndices.empty()) {
                    std::vector<Instruction> invariants;
                    std::set<int> toRemove(invariantIndices.begin(), invariantIndices.end());
                    
                    for (int idx : invariantIndices) {
                        invariants.push_back(instructions[idx]);
                    }

                    std::vector<Instruction> nextInstrs;
                    for (int j = 0; j < target; j++) nextInstrs.push_back(instructions[j]);
                    for (const auto& inv : invariants) nextInstrs.push_back(inv);
                    for (int j = target; j < (int)instructions.size(); j++) {
                        if (toRemove.find(j) == toRemove.end()) {
                            nextInstrs.push_back(instructions[j]);
                        }
                    }
                    instructions = nextInstrs;
                    return; 
                }
            }
        }
    }
}

/**
 * @brief Performs dead code elimination on temporary variables.
 */
void Optimizer::deadCodeElimination() {
    std::set<std::string> usedVars;
    for (const auto& instr : instructions) {
        if (!instr.label.empty()) continue;
        if (instr.op == "if") {
            usedVars.insert(instr.arg1);
        } else if (instr.op == "goto") {
        } else {
            if (!isNumber(instr.arg1)) usedVars.insert(instr.arg1);
            if (!isNumber(instr.arg2) && !instr.arg2.empty()) usedVars.insert(instr.arg2);
        }
    }

    std::vector<Instruction> nextInstrs;
    for (const auto& instr : instructions) {
        if (!instr.result.empty() && instr.result[0] == 't') {
            if (usedVars.find(instr.result) == usedVars.end()) {
                continue;
            }
        }
        nextInstrs.push_back(instr);
    }
    instructions = nextInstrs;
}
