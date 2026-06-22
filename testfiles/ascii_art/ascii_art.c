#include <stdio.h>

void printSpaces(int n)
{
    int i;
    i = 0;
    while (i < n)
    {
        putchar(' ');
        i = i + 1;
    }
    return;
}

void printStars(int n)
{
    int i;
    i = 0;
    while (i < n)
    {
        putchar('*');
        i = i + 1;
    }
    return;
}

void drawDiamond(int n)
{
    int i;
    i = 0;
    while (i < n)
    {
        printSpaces(n - 1 - i);
        printStars(2 * i + 1);
        putchar(10);
        i = i + 1;
    }
    i = 1;
    while (i < n)
    {
        printSpaces(i);
        printStars(2 * (n - 1 - i) + 1);
        putchar(10);
        i = i + 1;
    }
    return;
}

int main()
{
    drawDiamond(5);
    return 0;
}
