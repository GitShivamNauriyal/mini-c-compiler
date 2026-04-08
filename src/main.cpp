#include <iostream>
#include <memory>
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
 * @param argc Argument count.
 * @param argv Argument vector (argv[1] is the source file path).
 * @return int Exit status (0 for success, non-zero for errors).
 */
int main(int argc, char **argv)
{
    if (argc > 1)
    {
        FILE *file = fopen(argv[1], "r");
        if (!file)
        {
            std::cerr << "Cannot open file: " << argv[1] << std::endl;
            return 1;
        }
        yyin = file;
        std::cout << "\n--- Compiling file: " << argv[1] << " ---\n";
    }

    // Phase 1 & 2: Lexical and Syntax Analysis (handled by Flex/Bison)
    if (yyparse() == 0)
    {
        std::cout << "[SUCCESS] Lexical Analysis completed.\n";
        std::cout << "[SUCCESS] Syntax Analysis completed.\n";

        if (ast_root)
        {
            // Optional: Print AST for visualization
            std::cout << "\n================ ABSTRACT SYNTAX TREE ================\n";
            ast_root->print();
            std::cout << "======================================================\n";

            // Phase 3: Semantic Analysis
            SemanticAnalyzer semantic_analyzer;
            if (semantic_analyzer.analyze(ast_root))
            {
                std::cout << "\n[SUCCESS] Semantic Analysis completed.\n";
                semantic_analyzer.printSymbolTable();

                // Phase 4 & 5: Intermediate Code Generation & Optimization
                std::cout << "[SUCCESS] Generating Intermediate Code...\n";
                CodeGenerator codegen;
                codegen.generate(ast_root);

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
