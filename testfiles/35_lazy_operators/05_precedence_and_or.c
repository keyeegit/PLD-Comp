#include <stdio.h>

int side(int c) {
    putchar(c);
    return c;
}

int main() {
    return 0 || 1 && side('A');
}
