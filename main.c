#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DELIMITER ","

const int NUMBER_OF_MOVIES = 50;
const int CHAR_SIZE = 1000;
const int LANGUAGE_MAX_LENGTH = 20;
const int MAX_NUM_OF_LANGUAGES = 5;

struct MovieData {
    char *title;
    int year;
    char languages[MAX_NUM_OF_LANGUAGES][LANGUAGE_MAX_LENGTH];
    float rating;
};

int main(int argc, char **argv) {
    struct MovieData *movies = malloc(NUMBER_OF_MOVIES * sizeof(struct MovieData));
    char fileContent[CHAR_SIZE];
    FILE *fileHandler = fopen(argv[1], "r");

    if (fileHandler == NULL) {
        perror("Error loading file");
    }
    else {
        bool headerRead = false;
        int index = 0;
        while (true) {
            if (fgets(fileContent, CHAR_SIZE, fileHandler) != NULL) {
                if (headerRead == false) { // skipping the header of the file
                    headerRead = true;
                    continue;
                }

                char *token = strtok(fileContent, DELIMITER); // reading the title
                if(token != NULL) {
                    size_t titleLength = strlen(token);
                    char *title = (char *)malloc((titleLength + 1) * sizeof(char));
                    strcpy(title, token);
                    movies[index].title = title;
                }

                token = strtok(NULL, DELIMITER); // reading the year
                if(token != NULL){
                    movies[index].year = atoi(token);
                    printf("%d->", movies[index].year);
                }

                token = strtok(NULL, DELIMITER); // reading the languages
                if(token != NULL)
                    printf("%s    ", token);

                token = strtok(NULL, DELIMITER); // reading the rating
                if(token != NULL)
                    printf("%s    \n", token);
                index++;
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


    // free memory
    //free(movies);
    //free(movies[0].title);
    return 0;
}
