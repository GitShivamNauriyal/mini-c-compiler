int a = 1;
int d = 3;
int n = 20;
int sum = 0;
int i;
int current = a;

for (i = 0; i < n; i = i + 1)
{
    sum = sum + current;
    current = current + d;
}
