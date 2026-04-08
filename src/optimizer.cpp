#include "../include/optimizer.h"
#include <map>
#include <set>
#include <algorithm>

/**
 * @brief Prints all instructions in the current instruction set.
 */
void Optimizer::print() const {
    for (const auto& instr : instructions) {
        instr.print();
    }
}

/**
 * @brief Helper function to check if a string represents a numerical literal.
 * 
 * @param s The string to check.
 * @return bool True if the string is a number, false otherwise.
 */
bool isNumber(const std::string& s) {
    if (s.empty()) return false;
    if (s[0] == '-' && s.length() > 1) return std::isdigit(s[1]);
    return std::isdigit(s[0]);
}

/**
 * @brief Main optimization loop that runs multiple passes.
 * 
 * Continues to run optimization passes as long as changes are detected,
 * up to a maximum of 10 iterations to ensure termination.
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
 * 
 * If a variable is assigned a constant value, subsequent uses of that variable
 * are replaced with the constant until the variable is reassigned.
 */
void Optimizer::constantPropagation() {
    std::map<std::string, std::string> constants;
    for (auto& instr : instructions) {
        // If we see x = 5, record it
        if (instr.op == "=" && isNumber(instr.arg1)) {
            constants[instr.result] = instr.arg1;
        } else {
            // Replace args if they are known constants
            if (constants.count(instr.arg1)) instr.arg1 = constants[instr.arg1];
            if (constants.count(instr.arg2)) instr.arg2 = constants[instr.arg2];
            
            // If the variable is reassigned to something non-constant, remove from map
            if (instr.op != "=" || !isNumber(instr.arg1)) {
                 constants.erase(instr.result);
            }
        }
    }
}

/**
 * @brief Performs constant folding pass.
 * 
 * Evaluates binary operations on constant operands at compile time.
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
 * 
 * Removes identity operations like x + 0, x * 1, x * 0, etc.
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
 * 
 * Identifies redundant calculations (e.g., t1 = a + b; t2 = a + b) and 
 * replaces them with a simple assignment (t2 = t1).
 */
void Optimizer::commonSubexpressionElimination() {
    std::map<std::string, std::string> exprToVar;
    for (auto& instr : instructions) {
        // We only care about binary operations for CSE
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
        
        // If a non-temporary variable is reassigned, clear expressions as they might be invalid.
        if (instr.op == "=" && !instr.result.empty() && instr.result[0] != 't') {
            exprToVar.clear(); 
        }
    }
}

/**
 * @brief Performs Loop Optimization (primarily Loop-Invariant Code Motion).
 * 
 * Identifies loops and moves instructions that do not depend on loop-varying 
 * values outside the loop body.
 */
void Optimizer::loopOptimization() {
    // 1. Identify loops (look for back-edges)
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
                // Back-edge found! Loop is from target to i.
                
                // 2. Find variables modified within the loop
                std::set<std::string> modifiedInLoop;
                for (int j = target; j <= i; j++) {
                    if (!instructions[j].result.empty() && instructions[j].op != "if") {
                        modifiedInLoop.insert(instructions[j].result);
                    }
                }

                // 3. Identify invariant instructions
                std::vector<int> invariantIndices;
                for (int j = target + 1; j < i; j++) {
                    const auto& instr = instructions[j];
                    // An instruction is invariant if its result is not already modified 
                    // in the loop AND its arguments are not modified in the loop.
                    if (!instr.op.empty() && instr.op != "if" && instr.op != "goto" && instr.label.empty()) {
                        bool arg1Invariant = isNumber(instr.arg1) || modifiedInLoop.find(instr.arg1) == modifiedInLoop.end();
                        bool arg2Invariant = instr.arg2.empty() || isNumber(instr.arg2) || modifiedInLoop.find(instr.arg2) == modifiedInLoop.end();
                        
                        if (arg1Invariant && arg2Invariant) {
                            // Result must also not be used before it's defined in the loop 
                            // (simplified: just check if it's in modifiedInLoop)
                            // To be safe, we only move it if its result isn't modified multiple times.
                            invariantIndices.push_back(j);
                            // Important: the result itself is now "invariant" for subsequent instructions in this pass
                            modifiedInLoop.erase(instr.result); 
                        }
                    }
                }

                // 4. Move invariant instructions to before the loop start (target)
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
                    return; // Return to trigger another pass after modification
                }
            }
        }
    }
}

/**
 * @brief Performs dead code elimination on temporary variables.
 * 
 * Removes assignments to temporary variables (t1, t2...) that are never 
 * used as arguments in subsequent instructions.
 */
void Optimizer::deadCodeElimination() {
    std::set<std::string> usedVars;
    for (const auto& instr : instructions) {
        if (!instr.label.empty()) continue;
        if (instr.op == "if") {
            usedVars.insert(instr.arg1);
        } else if (instr.op == "goto") {
            // result is the label, not a variable
        } else {
            if (!isNumber(instr.arg1)) usedVars.insert(instr.arg1);
            if (!isNumber(instr.arg2) && !instr.arg2.empty()) usedVars.insert(instr.arg2);
        }
    }

    std::vector<Instruction> nextInstrs;
    for (const auto& instr : instructions) {
        // Only eliminate temporary variables (t1, t2...)
        if (!instr.result.empty() && instr.result[0] == 't') {
            if (usedVars.find(instr.result) == usedVars.end()) {
                continue;
            }
        }
        nextInstrs.push_back(instr);
    }
    instructions = nextInstrs;
}
