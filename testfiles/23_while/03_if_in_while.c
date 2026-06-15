int main()
{
    int i = 0;
    int somImpairs = 0;
    while (i < 6)
    {
        if (i % 2 != 0)
        {
            somImpairs = somImpairs + i;
        }
        i = i + 1;
    }
    return somImpairs;
}