int n = 27;
int steps = 0;

while (n != 1)
{
    int rem = n;
    while (rem >= 2)
    {
        rem = rem - 2;
    }
    
    if (rem == 0)
    {
        n = n / 2;
    }
    else
    {
        n = 3 * n + 1;
    }
    steps = steps + 1;
}
