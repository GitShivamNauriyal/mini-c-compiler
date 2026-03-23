#include <iostream>

extern int yyparse();
extern FILE *yyin;

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
        std::cout << "Compiling file: " << argv[1] << "\n";
    }
    else
    {
        std::cout << "Type your Mini C code below (Press Ctrl+D to finish):\n";
    }

    yyparse();

    return 0;
}