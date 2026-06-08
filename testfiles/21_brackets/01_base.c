int main() {
    int a = 42;
    {
        int b = 10;
        a = a + b;
    }
    return a;
}