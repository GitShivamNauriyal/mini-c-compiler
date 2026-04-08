int n = 496;
int sum = 1;
int i;

for (i = 2; i < n; i = i + 1)
{
    int rem = n;
    while (rem >= i)
    {
        rem = rem - i;
    }
    if (rem == 0)
    {
        sum = sum + i;
    }
}
