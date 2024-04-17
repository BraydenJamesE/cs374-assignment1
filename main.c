#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DELIMITER ","

const int NUMBER_OF_MOVIES = 50;
const int CHAR_SIZE = 1001;
const int LANGUAGE_MAX_LENGTH = 21;
const int MAX_NUM_OF_LANGUAGES = 5;
int actualNumberOfMovies = 0;

struct MovieData {
    char *title;
    int year;
    char languages[MAX_NUM_OF_LANGUAGES][LANGUAGE_MAX_LENGTH];
    float rating;
    struct MovieData *leftNeighbor;
    struct MovieData *rightNeighbor;
};

int main(int argc, char **argv) {
    struct MovieData *head = malloc(sizeof(struct MovieData));
    //struct MovieData *movies = malloc(NUMBER_OF_MOVIES * sizeof(struct MovieData));
    char fileContent[CHAR_SIZE];
    FILE *fileHandler = fopen(argv[1], "r");

    if (fileHandler == NULL) {
        perror("Error loading file");
    }
    else {
        bool headerRead = false;
        int index = 0;
        struct MovieData *currentMovie = malloc(sizeof(struct MovieData));
        while (true) {
            if (fgets(fileContent, CHAR_SIZE, fileHandler) != NULL) {
                if (index == 0) {
                    currentMovie = head;
                } else {
                    struct MovieData *newMovie = malloc(sizeof(struct MovieData));
                    currentMovie->rightNeighbor = newMovie;
                    newMovie->leftNeighbor = currentMovie;
                    currentMovie = newMovie;
                }

                if (headerRead == false) { // skipping the header of the file
                    headerRead = true;
                    continue;
                }

                char *token = strtok(fileContent, DELIMITER); // reading the title
                if(token != NULL) {
                    size_t titleLength = strlen(token);
                    char *title = (char *)malloc((titleLength + 1) * sizeof(char));
                    strcpy(title, token);
                    currentMovie->title = title;
                }

                token = strtok(NULL, DELIMITER); // reading the year
                if(token != NULL){
                    currentMovie->year = atoi(token);
                }

                token = strtok(NULL, DELIMITER); // reading the languages
                if(token != NULL) {
                    int languageCharIndex = 0;
                    int numberOfLanguagesIndex = 0;
                    int startIndex = 1;
                    int endIndex = 0;
                    bool breakLoop = false;
                    while (true) {
                        if (token[languageCharIndex] == ';' || token[languageCharIndex] == ']') {
                            if (token[languageCharIndex] == ']')
                                breakLoop = true;
                            int substringLength = endIndex - startIndex;
                            char *substringStart = token + startIndex;
                            char substring[substringLength + 1];
                            strncpy(substring, substringStart, substringLength);
                            substring[substringLength] = '\0';
                            strcpy(currentMovie->languages[numberOfLanguagesIndex], substring);
                            numberOfLanguagesIndex++;
                            startIndex = endIndex + 1;
                        }
                        endIndex++;
                        languageCharIndex++;
                        if (breakLoop)
                            break;
                    }
                }

                token = strtok(NULL, DELIMITER); // reading the rating
                if(token != NULL) {
                    float rating = atof(token);
                    currentMovie->rating = rating;
                }
                index++;
            } else {
                actualNumberOfMovies = index;
                head->leftNeighbor = currentMovie;
                currentMovie->rightNeighbor = head;
                break;
            }
        }
        fclose(fileHandler);
    }
    struct MovieData *printMovie = head->leftNeighbor;
    for(int i = 0; i < actualNumberOfMovies; i++) {
        printf("%d: %s \n", i + 1, printMovie->title);
        printMovie = printMovie->leftNeighbor;
    }
//    printf("%s -> %d -> %s -> %.1f", head->leftNeighbor->title, head->year, head->languages[1], head->rating);
//    printf("\n Actual Number of Movies: %d", actualNumberOfMovies);

    return 0;
}
