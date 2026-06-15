int absval(int x) {
    if (x < 0) {
        return 0 - x;
    }
    return x;
}

int main() {
    return absval(0 - 7);
}
