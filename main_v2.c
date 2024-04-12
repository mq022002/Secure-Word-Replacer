/*
Integer Overflows - we don't use any integers in this code, and user input is not used to calculate any integers
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Check if a character is a word character. Only works for words at least 2 characters long
// Returns 1 if the character is a word character, else 0 (True/False)
int isWordChar(char c) {
    // Check if the character is a letter, a digit, or an underscore
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_') {
        return 1;
    }
    // If the character is a hyphen, it's not a word character
    if (c == '-') {
        return 0;
    }
    // If the character is not a letter, a digit, or an underscore, it's not a word character
    return 0;
}

// Replace all occurrences of a word in a string with another word. Only replaces whole words.
// Returns a pointer to the modified string. The original string is modified in place.
char *replaceWordInString(char *str, const char *searchWord, const char *replaceWord) {
    // Start searching from the beginning of the string
    char *searchArea = str;

    // Keep searching while there are still occurrences of the search word
    while ((searchArea = strstr(searchArea, searchWord)) != NULL) {

        // Get the character before the found search word
        char prevChar = searchArea == str ? ' ' : *(searchArea - 1);

        /*
        1-1 --- check if searchWord is at the end of str before trying to access the next character. without the check,
        the program may try to access memory outside the bounds of the string, which can lead to undefined behavior.
        if the check finds there is no next character, it sets nextChar to a space character to avoid accessing
        */
        char nextChar = *(searchArea + strlen(searchWord)) != '\0' ? *(searchArea + strlen(searchWord)) : ' ';

        // Check if the found search word is a whole word (not part of a larger word)
        if (!isWordChar(prevChar) && !isWordChar(nextChar)) {
            // Calculate the position of the found search word in the string
            int position = searchArea - str;
            // Get the length of the search word
            int length = strlen(searchWord);


            /* 
            1-2 --- If the replacement word is longer than the search word, the code reallocates the string to 
            make room for the replacement word. The new size of the string is calculated as the current length 
            of the string, minus the length of the search word, plus the length of the replacement word, plus one 
            for the null terminator. This ensures that the buffer is always large enough to hold the new string, 
            preventing a buffer overflow.
            */
            if (strlen(replaceWord) > length) {
            // Calculate the new size of the string
            size_t newSize = strlen(str) - length + strlen(replaceWord) + 1;
            // Reallocate the string
            char *newStr = realloc(str, newSize);
            // Check if realloc failed
            if (newStr == NULL) {           
            /*
            3-1 --- memset is used to clear the memory of the string before freeing it. This can help prevent
            sensitive data from being left in memory after the string is freed. This helps stop information leakage
            by ensuring that the memory is cleared before it is released back to the system. Basically it overwrites
            the memory with 0s before freeing it.
            */
                memset(str, 0, strlen(str));
                free(str);
                perror("Error reallocating memory");
                return NULL;
            }
            // If realloc succeeded, update the str and searchArea pointers
            str = newStr;
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

// This function processes a file line by line, replacing all occurrences of a word with another word.
// It writes the modified lines to a new file. If the file could not be opened, it exits with code 1.
void processFile(FILE *input, FILE *output, const char *searchWord, const char *replaceWord) {
    // Set the initial buffer size to 100 characters
    size_t bufferSize = 1024;
    // Allocate memory for the buffer
    char *buffer = malloc(bufferSize * sizeof(char));
    // If memory allocation failed, print an error message and exit with code 1
    if (buffer == NULL) {
        perror("Error: memory allocation failed\n");
        exit(1);
    }
    // Read the file line by line
    while (fgets(buffer, bufferSize, input) != NULL) {
        // Replace all occurrences of the search word with the replace word in the current line
        char *newBuffer = replaceWordInString(buffer, searchWord, replaceWord);
        // If the buffer was reallocated, free the old buffer and update the buffer pointer
        if (newBuffer != buffer) {
            free(buffer);
            buffer = newBuffer;
        }
        // Write the modified line to the output file
        fputs(buffer, output);
    }
    // Free the buffer
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

    /*
     --- error handling for empty search and replace words. The program checks if either the search word or
    the replace word is empty, and prints an error message if either is empty. This prevents the program from
    attempting to search for or replace an empty string, which could lead to unexpected behavior.
    */
    if (strlen(searchWord) == 0 || strlen(replaceWord) == 0){
        printf("Error: search word and replace word must not be empty\n");
        return 1;
    }

    /*
    1-1 --- error handling for search and replace words that are unreasonably long. The program checks if either
    the search word or the replace word is longer than 50 characters, and prints an error message if either is too
    long. This prevents the program from attempting to process excessively long strings, which could lead to
    buffer overflows or other memory-related issues.
    */
    if (strlen(searchWord) >= 50 || strlen(replaceWord) >= 50){
        printf("Error: search word and replace word can not be longer than 99 characters\n");
        return 1;
    }

    /*
    2-1 --- The program prints a message indicating the search word, replace word, and file being processed. This
    is a safe use of printf, as the program is printing fixed strings and the values of command-line arguments
    */
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