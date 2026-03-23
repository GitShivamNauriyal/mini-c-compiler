#include <iostream>
#include <memory>
#include "ast.h"

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
            std::cout << "======================================================\n\n";
        }
    }
    else
    {
        std::cout << "[FAILED] Compilation halted due to syntax/lexical errors.\n";
    }

    return 0;
}