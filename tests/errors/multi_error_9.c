int a = 10;
int b = 20;

c = a + b; // Undeclared

for (int i = 0; i < 10; i = i + 1) // Declaration in for init not supported?
{
    a = a + i;
}

b = b + ; // Syntax error in expression
