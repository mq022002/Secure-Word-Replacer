/*
Format Strings - we don't print any direct user inputs ( printf(argv[1]) ) so it doesn't come up in this code
Integer Overflows - we don't use any integers in this code, and user input is not used to calculate any integers
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Check if a character is a word character. Only works for words at least 2 characters long
// Returns 1 if the character is a word character, 0 otherwise (True/False)
int isWordChar(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
}

// Replace all occurrences of a word in a string with another word. Only replaces whole words.
// Returns a pointer to the modified string. The original string is modified in place.
char *replaceWordInString(char *str, const char *searchWord, const char *replaceWord) {
    // Start searching from the beginning of the string
    char *searchArea = str;

    // Keep searching while there are still occurrences of the search word
    while ((searchArea = strstr(searchArea, searchWord)) != NULL) {
        // Get the character before and after the found search word
        char prevChar = searchArea == str ? ' ' : *(searchArea - 1);
        char nextChar = *(searchArea + strlen(searchWord));

        // Check if the found search word is a whole word (not part of a larger word)
        if (!isWordChar(prevChar) && !isWordChar(nextChar)) {
            // Calculate the position of the found search word in the string
            int position = searchArea - str;
            // Get the length of the search word
            int length = strlen(searchWord);

            // If the replace word is longer than the search word, reallocate the string to make room for the replace word
            if (strlen(replaceWord) > length) {
                str = realloc(str, strlen(str) - length + strlen(replaceWord) + 1);
                // Update the search area pointer after reallocating the string
                searchArea = str + position;
            }

            // Move the rest of the string to the right to make room for the replace word
            memmove(searchArea + strlen(replaceWord), searchArea + length, strlen(searchArea) - length + 1);
            // Copy the replace word into the space created
            memcpy(searchArea, replaceWord, strlen(replaceWord));
        }

        // Move the search area pointer to the end of the found search word
        searchArea += strlen(searchWord);
    }

    // Return the modified string
    return str;
}

// Process a file line by line, replacing all occurrences of a word with another word.
// Write the modified lines to a new file. exit with code 1 if the file could not be opened
void processFile(FILE *input, FILE *output, const char *searchWord, const char *replaceWord) {
    char *buffer = malloc(100 * sizeof(char));
    if (buffer == NULL) {
        perror("Error: memory allocation failed\n");
        exit(1);
    }
    while (fgets(buffer, 100, input) != NULL) {
        buffer = replaceWordInString(buffer, searchWord, replaceWord);
        fputs(buffer, output);
    }
    free(buffer);
}

int main(int argc, char *argv[]) {
    // error handling for incorrect number of arguments. [file to search] [word to search for] [word to replace with
    if (argc != 4){
        printf("command takes 3 arguments. [file to search] [word to search for] [word to replace with]\n");
        return 1;
    }

    char *file = argv[1];
    char *searchWord = argv[2];
    char *replaceWord = argv[3];

    // error handling for empty search word or replace word
    if (strlen(searchWord) == 0 || strlen(replaceWord) == 0){
        printf("Error: search word and replace word must not be empty\n");
        return 1;
    }

    // print the search and replace words, using %s to print strings and protect against format errors.
    printf("Searching for '%s' and replacing with '%s' in file '%s'\n", searchWord, replaceWord, file);

    // error handling for file not found
    FILE *fp = fopen(file, "r");
    if (fp == NULL){
        perror("Error opening file");
        return 1;
    }

    // error handling for not creating the modified file
    FILE *temp = fopen("modifiedText.txt", "w");
    if (temp == NULL){
        perror("Error: replace file could not be created\n");
        return 1;
    }

    // process the file, call the function to replace the words
    processFile(fp, temp, searchWord, replaceWord);

    fclose(fp);
    fclose(temp);

    return 0;
}