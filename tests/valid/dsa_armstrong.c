int n = 153;
int temp = n;
int sum = 0;
int rem;

while (temp != 0)
{
    rem = temp;
    while (rem >= 10)
    {
        rem = rem - 10;
    }
    sum = sum + rem * rem * rem;
    temp = temp / 10;
}
