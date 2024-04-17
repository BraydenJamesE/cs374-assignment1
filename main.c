#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DELIMITER ","

const int CHAR_SIZE = 1001;
const int LANGUAGE_MAX_LENGTH = 21;
const int MAX_NUM_OF_LANGUAGES = 5;
int actualNumberOfMovies = 0;

struct MovieData {
    char *title;
    int year;
    char languages[MAX_NUM_OF_LANGUAGES][LANGUAGE_MAX_LENGTH];
    float rating;
    struct MovieData *rightNeighbor;
    int amountOfLanguages;
};

struct MovieData *head;

int promptUser() {
    int input = 0;
    printf("\n1. Show movies released in the specified year \n");
    printf("2. Show highest rated movie for each year \n");
    printf("3. Show the title and year of release of all movies in a specific language \n");
    printf("4. Exit from the program \n");
    printf("Enter a choice from 1 to 4: ");
    scanf("%d", &input);
    return input;
}

void printMoviesFromYear(int yearInput) {
    struct MovieData *currentMovie = head;
    int numberOfMovies = 0;
    printf("\n");
    for(int i = 0; i < actualNumberOfMovies; i++) {
        if(currentMovie->year == yearInput) {
            printf("%s\n", currentMovie->title);
            numberOfMovies++;
        }
        currentMovie = currentMovie->rightNeighbor;
    }
    if (numberOfMovies == 0) {
        printf("No data about movies released in the year %d\n", yearInput);
    }
}

void getHighestRatedMoviePerYear() {
    int yearVisited[122] = {0};
    struct MovieData *firstLoopMovie = head;
    struct MovieData *secondLoopMovie;
    struct MovieData *movieToPrint;
    int year = 0;
    int yearIndex = 0;
    printf("\n");
    for(int i = 0; i < actualNumberOfMovies; i++) {
        year = firstLoopMovie->year;
        yearIndex = year - 1900;
        if(yearVisited[yearIndex] == 0) {
            yearVisited[yearIndex] = 1;
            movieToPrint = firstLoopMovie;
            secondLoopMovie = firstLoopMovie->rightNeighbor;
            for(int j = 0; j < actualNumberOfMovies - (i + 1); j++) {
                if(secondLoopMovie->rating > movieToPrint->rating && secondLoopMovie->year == movieToPrint->year) {
                    movieToPrint = secondLoopMovie;
                }
                secondLoopMovie = secondLoopMovie->rightNeighbor;
            }
            printf("%d %.1f %s \n", movieToPrint->year, movieToPrint->rating, movieToPrint->title);
        }
        firstLoopMovie = firstLoopMovie->rightNeighbor;
    }
    printf("\n");
}

void printMoviesWithSpecificLanguage(char *language) {
    struct MovieData *currentNode = head;
    int numberOfMoviesWithLanguage = 1;
    for(int i = 0; i < actualNumberOfMovies; i++) {
        for(int j = 0; j < currentNode->amountOfLanguages; j++) {
            if (strcmp(currentNode->languages[j], language)) {
                if(numberOfMoviesWithLanguage == 1)
                    printf("\n");
                printf("%d %s \n", currentNode->year, currentNode->title);
                numberOfMoviesWithLanguage++;
            }
        }
        currentNode = currentNode->rightNeighbor;
    }
    printf("\n");
    if (numberOfMoviesWithLanguage == 0) {
        printf("No data about movies released in %s \n", language);
    }
}


int main(int argc, char **argv) {
    struct MovieData *fakeHead = malloc(sizeof(struct MovieData));
    head = fakeHead;
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
                    currentMovie->amountOfLanguages = numberOfLanguagesIndex;
                }

                token = strtok(NULL, DELIMITER); // reading the rating
                if(token != NULL) {
                    float rating = atof(token);
                    currentMovie->rating = rating;
                }

                index++;
            } else {
                actualNumberOfMovies = index;
                currentMovie->rightNeighbor = head;
                break;
            }
        }
        fclose(fileHandler);
    }

    while (true) {
        int userRequest = promptUser();
        int year = 0;
        char * language;
        switch(userRequest) {
            case 1: // show movie released in specific year
                printf("Enter the year for which you want to see movies: ");
                scanf("%d", &year);
                printMoviesFromYear(year);
                year = 0;
                break;
            case 2: // show highest rated movie for each year
                getHighestRatedMoviePerYear();
                break;
            case 3: // show the title and year of release of all movies in a specific language
                printf("Enter the language for which you want to see movies: ");
                scanf("%s", language);
                printMoviesWithSpecificLanguage(language);
                break;
            case 4: // Exit from the program
                return 0;
            default:
                printf("\nYou entered an incorrect choice. Try again.\n\n");
        }
    }

    struct MovieData *printMovie = head;
    for(int i = 0; i < actualNumberOfMovies; i++) {
        printf("%d: %s \n", i + 1, printMovie->title);
        printMovie = printMovie->rightNeighbor;
    }




//    printf("%s -> %d -> %s -> %.1f", head->leftNeighbor->title, head->year, head->languages[1], head->rating);
//    printf("\n Actual Number of Movies: %d", actualNumberOfMovies);

    return 0;
}
