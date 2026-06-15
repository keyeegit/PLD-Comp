int main()
{
    int numbers[5];
    int i = 0;

    while (i < 5)
    {
        numbers[i] = i * 2;
        i = i + 1;
    }

    int somme = numbers[0] + numbers[1] + numbers[2] + numbers[3] + numbers[4];
    return somme;
}