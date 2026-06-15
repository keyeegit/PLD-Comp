int main() {
    int x;
    x = 5;
    {
        int y;
        y = x + 3;
        if (y > 7) {
            return y;
        }
    }
    return 0;
}
