#include <stdio.h>

int isUpper(int c)
{
    return c > 64 & c < 91;
}

int isLower(int c)
{
    return c > 96 & c < 123;
}

int shift(int c, int n)
{
    if (isUpper(c))
    {
        return 65 + (c - 65 + n) % 26;
    }
    if (isLower(c))
    {
        return 97 + (c - 97 + n) % 26;
    }
    return c;
}

int main()
{
    int n;
    int c;
    n = 3;
    c = getchar();
    while (c != -1)
    {
        putchar(shift(c, n));
        c = getchar();
    }
    return 0;
}
