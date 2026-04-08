int x = 120;
int y = 45;

while (x != y)
{
    if (x > y)
    {
        x = x - y;
    }
    else
    {
        y = y - x;
    }
}
int result = x;
