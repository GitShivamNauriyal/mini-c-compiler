#include <iostream>
#include <memory>
#include "../include/ast.h"
#include "../include/semantic.h"
#include "../include/codegen.h"

extern int yyparse();
extern FILE *yyin;
extern ASTNodePtr ast_root;

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

                // INTERMEDIATE CODE GENERATION
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