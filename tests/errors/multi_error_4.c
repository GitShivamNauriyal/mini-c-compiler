int a = 10;
float b = 20.0;
char c = 65;

if (a == b) // Comparison float/int handled?
{
    a = c + b;
}

b = a + d; // Undeclared d
c = 100
a = b; // Type mismatch assignment
