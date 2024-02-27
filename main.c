#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 4){
        printf("command takes 3 arguments. [file to search] [word to search for] [word to replace with]\n");
    }

    char *file = argv[1];
    char *searchWord = argv[2];
    char *replaceWord = argv[3];

    FILE *fp = fopen(file, "r");
    if (fp == NULL){
        printf("Error: file not found\n");
        return 1;
    }

    FILE *temp = fopen("modifiedText.txt", "w");
    if (temp == NULL){
        printf("Error: replace file could not be created\n");
        return 1;
    }

    char buffer [100];
    while (fgets(buffer, 100, fp) != NULL){
        char *searchArea = strstr(buffer, searchWord);
        while (searchArea != NULL){
            int position = searchArea - buffer;
            int length = strlen(searchWord);
            memmove(searchArea + strlen(replaceWord), searchArea + length, strlen(searchArea) - length + 1);
            memcpy(searchArea, replaceWord, strlen(replaceWord));
            searchArea = strstr(searchArea + strlen(replaceWord), searchWord);
        }
        fputs(buffer, temp);
    }

    fclose(fp);
    fclose(temp);

    return 0;
}