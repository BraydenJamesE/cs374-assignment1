#include <stdbool.h>
#include <stdio.h>

int main(int argc, char **argv) {
    const int CHAR_SIZE = 1000;
    char fileContent[CHAR_SIZE];
    FILE *fileHandler = fopen(argv[1], "r");

    if (fileHandler == NULL) {
        perror("Error loading file");
    }
    else {
        int count = 0;
        while (true) {
            if (fgets(fileContent, CHAR_SIZE, fileHandler) != NULL) {
                count++;
                printf("%d. %s", count, fileContent);
                //puts(fileContent);
            } else {
                break;
            }
        }
        fclose(fileHandler);

    }



    printf("%d", argc);
    printf("%s", argv[0]);
    printf("\n");
    printf("%p", fileContent);
    printf("\n");
    printf("Hello, World!\n");
    return 0;
}
