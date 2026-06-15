int sign(int x) {
    if (x > 0) {
        return 1;
    } else {
        if (x < 0) {
            return 255;
        } else {
            return 0;
        }
    }
}

int main() {
    return sign(0);
}
