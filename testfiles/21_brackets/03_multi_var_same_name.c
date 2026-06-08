int main() {
    int a = 10;
    {
        int a = 99;
        a = a + 1;
    }
    return a;
}