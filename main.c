#include <stdio.h>

int main(int argc, char **argv) {
    printf("%d", argc);
    printf("%s", argv[0]);
    printf("\n");
    printf("%s", argv[1]);
    printf("\n");
    printf("Hello, World!\n");
    return 0;
}
