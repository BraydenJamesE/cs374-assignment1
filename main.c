#define _GNU_SOURCE
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DELIMITER ","
#define LANGUAGE_MAX_LENGTH 21
#define MAX_NUM_OF_LANGUAGES 5


const int CHAR_SIZE = 1001;
int actualNumberOfMovies = 0;


struct movie { // creating the movie struct
    char *title;
    int year;
    char languages[MAX_NUM_OF_LANGUAGES][LANGUAGE_MAX_LENGTH];
    float rating;
    struct movie *rightNeighbor; // This variable creates the linked list, allowing each member to reference their neighbor in the list.
    int amountOfLanguages;
}; // end of movie struct


struct movie *head; // Creating a global variable called head that will allow me to keep track of the head of the linked list.


void freeMovieData() { // This function frees the memory for the entire program with the exception of one variable which is freed in main.
    struct movie *currentNode = head->rightNeighbor; // Creating a current node that is the neigbor of head.
    struct movie *nextNode; // Creating a nextNode which will act as a temp for us to free current node.
    while(currentNode != head) { // looping through the entire linked list.
        nextNode = currentNode->rightNeighbor; // Assigning a value for the temp, freeing parts of the currentNode, and then freeing the entire current node.
        free(currentNode->title);
        free(currentNode);
        currentNode = nextNode;
    }
    free(head); // Once I have freed every item in the linked list (besides the head), I free the head.
} // end of freeMovieData function


int promptUser() { // This function simply prompts the user for their input and then sends it back to main.
    int input;
    printf("\n1. Show movies released in the specified year \n");
    printf("2. Show highest rated movie for each year \n");
    printf("3. Show the title and year of release of all movies in a specific language \n");
    printf("4. Exit from the program \n\n");
    printf("Enter a choice from 1 to 4: ");
    scanf("%d", &input);
    return input;
} // end of promptUser function


void printMoviesFromYear(int yearInput) { // This function loops through the entire linked list and prints the movies that came out in the year (the functions parameter).
    struct movie *currentMovie = head;
    int numberOfMovies = 1;
    for(int i = 0; i < actualNumberOfMovies; i++) {
        if(currentMovie->year == yearInput) { // Output the movie if the year matches the parameter.
            printf("%s\n", currentMovie->title);
            numberOfMovies++;
        }
        currentMovie = currentMovie->rightNeighbor; // iterate through the linked list by one
    }
    if (numberOfMovies == 1) { // if no movies matched the desired year, notifiy the user.
        printf("No data about movies released in the year %d\n", yearInput);
    }
} // end of printMoviesFromYear function


void getHighestRatedMoviePerYear() {
    int yearVisited[122] = {0}; // initializing an array with 0. This array will act as a boolean. When a movie of a unvisited year has been visited, change the value in this array to 1.
    struct movie *firstLoopMovie = head;
    struct movie *secondLoopMovie;
    struct movie *movieToPrint;
    int year = 0;
    int yearIndex = 0;
    for(int i = 0; i < actualNumberOfMovies; i++) { // This function holds a nest loop. The first loop, loops through all the movies in the file. When an year is encounter that has not been visited before, the year is marked as visited and the second loop begins. The second loop loops through all the movies in the file and if a higher rating is found, that movie is marked as the highest rated for that year. Once highest rating for a given year is found, it no longer checks that year again.
        year = firstLoopMovie->year;
        yearIndex = year - 1900; // The way years are indexed is by taking the minimum year (1900) and subtracting it to the year specified. Thus, the year 1901 turns into the index 1.
        if(yearVisited[yearIndex] == 0) { // if that year has not been visited
            yearVisited[yearIndex] = 1;
            movieToPrint = firstLoopMovie;
            secondLoopMovie = firstLoopMovie->rightNeighbor;
            for(int j = 0; j < actualNumberOfMovies - (i + 1); j++) {
                if(secondLoopMovie->rating > movieToPrint->rating && secondLoopMovie->year == movieToPrint->year) {
                    movieToPrint = secondLoopMovie;
                }
                secondLoopMovie = secondLoopMovie->rightNeighbor;
            }
            printf("%d %.1f %s \n", movieToPrint->year, movieToPrint->rating, movieToPrint->title); // once the best rated movie is found for the given year, output it to the user.
        }
        firstLoopMovie = firstLoopMovie->rightNeighbor; // iterate the linked list
    }
} // end of getHighestRatedMoviePerYear function


void printMoviesWithSpecificLanguage(char *language) { // This function prints all the movies with the specified language.
    struct movie *currentNode = head;
    bool movieFound = false; // This boolean will allow us to notify the user if there is no movie found in the desired language.
    for(int i = 0; i < actualNumberOfMovies; i++) { // Looping through each movie
        int amountOfLanguagesInCurrentMovie = currentNode->amountOfLanguages; // Accessing the number of languages assocaited with the current movie.
        for(int j = 0; j < amountOfLanguagesInCurrentMovie; j++) { // Looping through all the languages of the current movie.
            if (strcmp(currentNode->languages[j], language) == 0) { // if the desired movie == the language
                if(!movieFound)
                    movieFound = true;
                printf("%d %s \n", currentNode->year, currentNode->title); // output the movie to the user
                j = amountOfLanguagesInCurrentMovie; // When checking the languages of a given movie, if the language is found, we no longer need to check the remaining languages. Thus, end the loop.
            }
        }
        currentNode = currentNode->rightNeighbor; // iterate the linked list
    }
    if (!movieFound) { // if we never find a movie with the desired language, output that information to the user.
        printf("No data about movies released in %s \n", language);
    }
} // end of printMoviesWithSpecificLanguage function


int main(int argc, char **argv) {
    struct movie *fakeHead = malloc(sizeof(struct movie)); // Creating a movie struct
    head = fakeHead; // Assigning our head for the linked list.
    FILE *fileHandler = fopen(argv[1], "r"); // opening file
    char fileContent[CHAR_SIZE]; // creating a string to hold the file information
    if (fileHandler == NULL) { // ensuring that the file is not null
        perror("Error loading file");
    }
    else {
        bool headerRead = false; // This boolean ensures that we don't read the header as a actual movie.
        int index = 0;
        struct movie *currentMovie = malloc(sizeof(struct movie)); // Creating a new member of the linked list
        while (true) { // Loop until all the movies are read.
            if (fgets(fileContent, CHAR_SIZE, fileHandler) != NULL) { // get the file content
                if (index == 0) { // upon first iteration, the current movie is the head.
                    currentMovie = head;
                } else { // if it is not the first iteration, create a new member of the linked list, attach it to the linked list, and assign it as the current movie.
                    struct movie *newMovie = malloc(sizeof(struct movie));
                    currentMovie->rightNeighbor = newMovie;
                    currentMovie = newMovie;
                }

                if (headerRead == false) { // skipping the header of the file
                    headerRead = true;
                    continue;
                }

                char *token;
                char *savePtr;
                savePtr = NULL;
                token = strtok_r(fileContent, DELIMITER, &savePtr); // reading the title
                if(token != NULL) {
                    size_t titleLength = strlen(token); // storing the title length in a variable
                    char *title = (char *)malloc((titleLength + 1) * sizeof(char)); // allocating the needed memory to store the title
                    strcpy(title, token); // assigning the title, read from the file, to a variable
                    currentMovie->title = title; // setting the title in the current movie struct to the title read from the file
                }

                token = strtok_r(NULL, DELIMITER, &savePtr); // reading the year
                if(token != NULL){
                    currentMovie->year = atoi(token); // assigning the year to the struct
                }

                token = strtok_r(NULL, DELIMITER, &savePtr); // reading the languages
                if(token != NULL) {
                    int languageCharIndex = 0; // There are various integers needed for the indexing of the langauges
                    int numberOfLanguagesIndex = 0;
                    int startIndex = 1;
                    int endIndex = 0;
                    bool breakLoop = false;
                    while (true) {
                        if (token[languageCharIndex] == ';' || token[languageCharIndex] == ']') {
                            if (token[languageCharIndex] == ']') // if an ']' is encounter, break out of the loop. We have reached the end of the languages.
                                breakLoop = true;
                            int substringLength = endIndex - startIndex; // Track the number of chars in the current language
                            char *substringStart = token + startIndex; // setting the substring to the start of the current language.
                            char substring[substringLength + 1];
                            strncpy(substring, substringStart, substringLength); // copying the current language using the indexing above
                            substring[substringLength] = '\0'; // adding the end to the string
                            strcpy(currentMovie->languages[numberOfLanguagesIndex], substring); // assigning the read language to the struct at using an array in the struct.
                            numberOfLanguagesIndex++; // iterating the index we use to store the languages in the array.
                            startIndex = endIndex + 1; // Making the new start index, the value after the semi-colon.
                        }
                        endIndex++; // Iterating the end index
                        languageCharIndex++; // iterating the absolute index.
                        if (breakLoop) // break the loop.
                            break;
                    }
                    currentMovie->amountOfLanguages = numberOfLanguagesIndex; // assign the struct value that tracks the number of languages
                }
                token = strtok_r(NULL, DELIMITER, &savePtr); // reading the rating
                if(token != NULL) {
                    float rating = atof(token);
                    currentMovie->rating = rating; // assigning the rating to the struct
                }

                index++;
            } else {
                actualNumberOfMovies = index; // tracking the number of movies that were in the file
                currentMovie->rightNeighbor = head; // closing the linked list so that the final value in the linked list can access the head
                break;
            }
        }
        fclose(fileHandler);
    }

    while (true) { // Keeping the prompt for the user up until they enter 4 using a loop.
        int userRequest = promptUser(); // Calling the promptUser function which asks the user to enter their desired choice from the options provided.
        int year = 0; // variable for tracking the year they may want to use (option 1)
        char * language = malloc(20 * sizeof(char)); // Variable for tracking the language they want to use (option 3).

        if(userRequest == 1) { // user enters option 1.
            printf("Enter the year for which you want to see movies: "); // prompting the user for the desired year
            scanf("%d", &year); // reading in the user input
            printMoviesFromYear(year); // calling the printMoviesFromYear function which outputs all the movies from the desired year.
        }
        else if(userRequest == 2) { // user enters option 2.
            getHighestRatedMoviePerYear(); // Calling the function that outputs all the highest rated movies for each year
        }
        else if(userRequest == 3) { // user enters option 3.
            printf("Enter the language for which you want to see movies: "); // prompting the user to enter the language
            scanf("%s", language); // getting the language from the user
            printMoviesWithSpecificLanguage(language); // calling the function that outputs all the movies with the desired language.
        }
        else if(userRequest == 4) { // user enters option 4.
            freeMovieData(); // Freeing all the memory in the linked list, including in each struct.
            free(language); // Freeing the language variable used above.
            return 0; // ending the program
        }
        else { // if the user doesn't enter a valid input, inform them, and loop.
            printf("\nYou entered an incorrect choice. Try again.\n\n");
        }
    }
}