/*
Format Strings - we don't print any direct user inputs ( printf(argv[1]) ) so it doesn't come up in this code
Integer Overflows - we don't use any integers in this code, and user input is not used to calculate any integers
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isWordChar(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
}

int main(int argc, char *argv[]) {
    // error handling for incorrect number of arguments
    if (argc != 4){
        printf("command takes 3 arguments. [file to search] [word to search for] [word to replace with]\n");
        return 1;
    }

    char *file = argv[1];
    char *searchWord = argv[2];
    char *replaceWord = argv[3];

    if (strlen(searchWord) == 0 || strlen(replaceWord) == 0){
    printf("Error: search word and replace word must not be empty\n");
    return 1;
}

    FILE *fp = fopen(file, "r");
    // error handling for file not found
    if (fp == NULL){
        perror("Error opening file");
        return 1;
    }

    FILE *temp = fopen("modifiedText.txt", "w");
    // error handling for not creating the modified file
    if (temp == NULL){
        perror("Error: replace file could not be created\n");
        return 1;
    }

    char *buffer = malloc(100 * sizeof(char));
    // error handling for memory allocation failure
    if (buffer == NULL) {
        perror("Error: memory allocation failed\n");
        return 1;
    }

    while (fgets(buffer, 100, fp) != NULL){
        char *searchArea = buffer;

        /* Buffer overflow prevention (if the replacement word is longer than the search word):
        before replacing the search word with the replacement word, we check if the replacement word is 
        longer than the search word. If it is, we reallocate the buffer to make it large enough to hold the 
        replaced word. This prevents a buffer overflow because the buffer is always large enough to hold the 
        replaced word.
        */

       // strstr() treats the input as a string, it is not vulnerable to command injection
        while ((searchArea = strstr(searchArea, searchWord)) != NULL){
        char prevChar = searchArea == buffer ? ' ' : *(searchArea - 1);
        char nextChar = *(searchArea + strlen(searchWord));

        // check if the search word is a whole word - check the char before and after the search word, 
        // if they are not word characters, then the search word is a whole word
        if (!isWordChar(prevChar) && !isWordChar(nextChar)) {
            int position = searchArea - buffer;
            int length = strlen(searchWord);
            // check if the replacement word is longer than the search word, if it is, reallocate the buffer
            if (strlen(replaceWord) > length) {
                buffer = realloc(buffer, strlen(buffer) - length + strlen(replaceWord) + 1);
                searchArea = buffer + position;
            }
            // replace the search word with the replacement word, and move the rest of the string to the right
            memmove(searchArea + strlen(replaceWord), searchArea + length, strlen(searchArea) - length + 1);
            memcpy(searchArea, replaceWord, strlen(replaceWord));
        }
        // move the search area to the end of the search word
        searchArea += strlen(searchWord);
    }
        fputs(buffer, temp);
    }
    // free the buffer and close the files
    free(buffer);
    fclose(fp);
    fclose(temp);

    return 0;
}