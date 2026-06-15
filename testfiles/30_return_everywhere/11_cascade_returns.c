int classify(int x) {
    if (x < 0) {
        return 1;
    }
    if (x == 0) {
        return 2;
    }
    if (x < 10) {
        return 3;
    }
    if (x < 100) {
        return 4;
    }
    return 5;
}

int main() {
    return classify(50);
}
