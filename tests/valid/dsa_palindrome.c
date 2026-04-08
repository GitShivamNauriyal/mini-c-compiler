int n = 12321;
int reversed = 0;
int temp = n;
int rem;

while (temp != 0)
{
    rem = temp;
    while (rem >= 10)
    {
        rem = rem - 10;
    }
    reversed = reversed * 10 + rem;
    temp = temp / 10;
}
