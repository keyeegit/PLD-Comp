int search(int target) {
    int i;
    i = 0;
    while (i < 20) {
        if (i == target) {
            return 1;
        }
        i = i + 1;
    }
    return 0;
}

int main() {
    int a;
    int b;
    a = search(15);
    b = search(25);
    return a + b;
}
