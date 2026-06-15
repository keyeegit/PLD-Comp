#include <stdio.h>

void maybeputchar(int c) {
    if (c < 0) {
        return;
    }
    putchar(c);
}

int main() {
    maybeputchar(0 - 1);
    maybeputchar(65);
    return 0;
}
