int n = 50;
int i;
int j;
int isPrime;

for (i = 2; i <= n; i = i + 1)
{
    isPrime = 1;
    for (j = 2; j < i; j = j + 1)
    {
        int rem = i;
        while (rem >= j)
        {
            rem = rem - j;
        }
        if (rem == 0)
        {
            isPrime = 0;
        }
    }
}
