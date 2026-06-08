int main() {
    int x = 10;
    int y = 20;
    int res = 0;

    if (x > 5) {
        if (y < 15) {
            res = 1;
        } else {
            res = 2;
        }
    } else {
        res = 3;
    }
    return res;
}