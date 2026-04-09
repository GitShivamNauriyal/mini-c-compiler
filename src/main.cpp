#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "../include/ast.h"
#include "../include/semantic.h"
#include "../include/codegen.h"

extern int yyparse();
extern FILE *yyin;
extern ASTNodePtr ast_root;

/**
 * @brief Entry point for the Mini-C compiler.
 * 
 * Orchestrates the compilation phases: Lexical analysis, Syntax analysis, 
 * Semantic analysis, and Intermediate Code Generation with optimization.
 * 
 * Supports -O0 (no optimization) and -O1 (optimization enabled, default).
 * 
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return int Exit status (0 for success, non-zero for errors).
 */
int main(int argc, char **argv)
{
    std::string fileName;
    bool optimize = true;

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "-O0")
        {
            optimize = false;
        }
        else if (arg == "-O1")
        {
            optimize = true;
        }
        else if (arg[0] != '-')
        {
            fileName = arg;
        }
    }

    if (!fileName.empty())
    {
        FILE *file = fopen(fileName.c_str(), "r");
        if (!file)
        {
            std::cerr << "Cannot open file: " << fileName << std::endl;
            return 1;
        }
        yyin = file;
        std::cout << "\n--- Compiling file: " << fileName << " (Optimization: " << (optimize ? "ON" : "OFF") << ") ---\n";
    }

    if (yyparse() == 0)
    {
        std::cout << "[SUCCESS] Lexical Analysis completed.\n";
        std::cout << "[SUCCESS] Syntax Analysis completed.\n";

        if (ast_root)
        {
            std::cout << "\n================ ABSTRACT SYNTAX TREE ================\n";
            ast_root->print();
            std::cout << "======================================================\n";

            SemanticAnalyzer semantic_analyzer;
            if (semantic_analyzer.analyze(ast_root))
            {
                std::cout << "\n[SUCCESS] Semantic Analysis completed.\n";
                semantic_analyzer.printSymbolTable();

                std::cout << "[SUCCESS] Generating Intermediate Code...\n";
                CodeGenerator codegen;
                codegen.generate(ast_root, optimize);

                std::cout << "--- Compilation Finished Successfully ---\n\n";
            }
            else
            {
                std::cout << "\n[FAILED] Compilation halted due to semantic errors.\n\n";
            }
        }
    }
    else
    {
        std::cout << "[FAILED] Compilation halted due to syntax/lexical errors.\n\n";
    }

    return 0;
}
