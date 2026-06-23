#include <stdio.h>

void renderMandelbrot()
{
    int width;
    int height;
    int maxIter;
    int y;

    width = 78;
    height = 36;
    maxIter = 30;
    y = 0;

    while (y < height)
    {
        int x;
        x = 0;
        while (x < width)
        {
            int cr;
            cr = -8192 + (x * 10240) / width;

            int ci;
            ci = -4505 + (y * 9011) / height;

            int zr;
            int zi;
            int iter;
            int escaped;
            int escapeIter;

            zr = 0;
            zi = 0;
            iter = 0;
            escaped = 0;
            escapeIter = 0;

            while (iter < maxIter)
            {
                if (escaped == 0)
                {
                    int zr2;
                    int zi2;
                    zr2 = (zr * zr) / 4096;
                    zi2 = (zi * zi) / 4096;

                    if (zr2 + zi2 > 16384)
                    {
                        escaped = 1;
                        escapeIter = iter;
                    }
                    else
                    {
                        int ziNext;
                        ziNext = (2 * zr * zi) / 4096 + ci;
                        
                        zr = zr2 - zi2 + cr;
                        zi = ziNext;
                    }
                }
                iter = iter + 1;
            }

            if (escaped == 0)
            {
                putchar('#');
            }
            else
            {
                if (escapeIter < 3)
                {
                    putchar(' ');
                }
                else
                {
                    if (escapeIter < 6)
                    {
                        putchar('.');
                    }
                    else
                    {
                        if (escapeIter < 10)
                        {
                            putchar('-');
                        }
                        else
                        {
                            if (escapeIter < 15)
                            {
                                putchar('+');
                            }
                            else
                            {
                                if (escapeIter < 22)
                                {
                                    putchar('*');
                                }
                                else
                                {
                                    putchar('%');
                                }
                            }
                        }
                    }
                }
            }
            x = x + 1;
        }
        putchar(10);
        y = y + 1;
    }
}

int main()
{
    renderMandelbrot();
    return 0;
}
