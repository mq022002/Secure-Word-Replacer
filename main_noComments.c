#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isWordChar(char c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_') {
        return 1;
    }
    if (c == '-') {
        return 0;
    }
    return 0;
}

char *replaceWordInString(char *str, const char *searchWord, const char *replaceWord) {
    char *searchArea = str;

    while ((searchArea = strstr(searchArea, searchWord)) != NULL) {

        char prevChar = searchArea == str ? ' ' : *(searchArea - 1);

        char nextChar = *(searchArea + strlen(searchWord)) != '\0' ? *(searchArea + strlen(searchWord)) : ' ';

        if (!isWordChar(prevChar) && !isWordChar(nextChar)) {
            int position = searchArea - str;
            int length = strlen(searchWord);


            if (strlen(replaceWord) > length) {
            size_t newSize = strlen(str) - length + strlen(replaceWord) + 1;
            char *newStr = realloc(str, newSize);
            if (newStr == NULL) {
                free(str);
                perror("Error reallocating memory");
                return NULL;
            }
            str = newStr;
            searchArea = str + position;
            }

            memmove(searchArea + strlen(replaceWord), searchArea + length, strlen(searchArea) - length + 1);
            memcpy(searchArea, replaceWord, strlen(replaceWord));
        }

        searchArea += strlen(searchWord);
    }

    return str;
}

void processFile(FILE *input, FILE *output, const char *searchWord, const char *replaceWord) {
    size_t bufferSize = 1024;
    char *buffer = malloc(bufferSize * sizeof(char));
    if (buffer == NULL) {
        perror("Error: memory allocation failed\n");
        exit(1);
    }
    while (fgets(buffer, bufferSize, input) != NULL) {
        char *newBuffer = replaceWordInString(buffer, searchWord, replaceWord);
        if (newBuffer != buffer) {
            free(buffer);
            buffer = newBuffer;
        }
        fputs(buffer, output);
    }
    free(buffer);
}

int main(int argc, char *argv[]) {
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

    if (strlen(searchWord) >= 50 || strlen(replaceWord) >= 50){
        printf("Error: search word and replace word can not be longer than 99 characters\n");
        return 1;
    }

    printf("Searching for '%s' and replacing with '%s' in file '%s'\n", searchWord, replaceWord, file);
    FILE *fp = fopen(file, "r");
    if (fp == NULL){
        perror("Error opening file");
        return 1;
    }
    FILE *temp = fopen("modifiedText.txt", "w");
    if (temp == NULL){
        perror("Error: replace file could not be created\n");
        return 1;
    }
    processFile(fp, temp, searchWord, replaceWord);

    fclose(fp);
    fclose(temp);

    return 0;
}