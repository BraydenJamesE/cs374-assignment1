#include <stdio.h>

int main(int argc, char **argv) {
    const int CHAR_SIZE = 1000;
    char fileContent[CHAR_SIZE];
    FILE *fileHandler = fopen(argv[1], "r");

    if (fileHandler == NULL) {
        perror("Error loading file");
    }
    else {
        if (fgets(fileContent, CHAR_SIZE, fileHandler) != NULL) {
            puts(fileContent);
        }
        fclose(fileHandler);
    }



    printf("%d", argc);
    printf("%s", argv[0]);
    printf("\n");
    printf("%p", &fileContent);
    printf("\n");
    printf("Hello, World!\n");
    return 0;
}
