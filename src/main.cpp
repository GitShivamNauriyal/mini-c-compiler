#include <iostream>
#include <fstream>
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
 * Orchestrates compilation and saves intermediate artifacts to the 'output/' directory.
 */
int main(int argc, char **argv)
{
    std::string fileName;
    bool optimize = true;

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "-O0") optimize = false;
        else if (arg == "-O1") optimize = true;
        else if (arg[0] != '-') fileName = arg;
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
        std::cout << "\n--- Compiling: " << fileName << " (Optimization: " << (optimize ? "ON" : "OFF") << ") ---\n";
    }

    if (yyparse() == 0)
    {
        std::cout << "[SUCCESS] Lexical & Syntax Analysis.\n";

        if (ast_root)
        {
            // 1. Save AST
            std::ofstream astFile("output/ast.txt");
            ast_root->print(0, astFile);
            astFile.close();

            SemanticAnalyzer semantic_analyzer;
            if (semantic_analyzer.analyze(ast_root))
            {
                std::cout << "[SUCCESS] Semantic Analysis.\n";

                // 2. Save Symbol Table
                std::ofstream symFile("output/symbol_table.txt");
                semantic_analyzer.printSymbolTable(symFile);
                symFile.close();

                std::cout << "[SUCCESS] Generating Code...\n";
                CodeGenerator codegen;
                
                // 3. Save TAC and MIPS
                std::ofstream tacFile("output/tac.txt");
                std::ofstream optTacFile("output/tac_optimized.txt");
                std::ofstream mipsFile("output/mips.asm");

                codegen.generate(ast_root, optimize, tacFile, optTacFile, mipsFile);

                tacFile.close();
                optTacFile.close();
                mipsFile.close();

                std::cout << "--- Compilation Finished. Artifacts saved in 'output/' ---\n\n";
            }
            else
            {
                std::cout << "\n[FAILED] Semantic errors detected.\n\n";
            }
        }
    }
    else
    {
        std::cout << "[FAILED] Syntax/Lexical errors detected.\n\n";
    }

    return 0;
}
