#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *replaceWordInString(const char *str, const char *searchWord, const char *replaceWord)
{
    int searchWordLen = strlen(searchWord);
    int replaceWordLen = strlen(replaceWord);
    int diff = replaceWordLen - searchWordLen;

    int count = 0;
    const char *tmp = str;
    const char *original_str = str;
    while ((tmp = strstr(tmp, searchWord)) != NULL)
    {
        count++;
        tmp += searchWordLen;
    }

    char *newStr = malloc(strlen(str) + count * diff + 1);
    if (newStr == NULL)
    {
        perror("Error allocating memory");
        return NULL;
    }

    char *current = newStr;
    while (*str)
    {
        if (strstr(str, searchWord) == str &&
        (*(str - 1) == ' ' || str == original_str) && 
        (*(str + searchWordLen) == ' ' || *(str + searchWordLen) == '\0' ||
         *(str + searchWordLen) == ',' || *(str + searchWordLen) == '.')) 
        {
            strcpy(current, replaceWord);
            current += replaceWordLen;
            str += searchWordLen;
        }
        else
        {
            *current++ = *str++;
        }
    }

    *current = '\0';

    return newStr;
}

void processFile(FILE *input, FILE *output, const char *searchWord, const char *replaceWord)
{
    size_t bufferSize = 1024;
    char *buffer = malloc(bufferSize * sizeof(char));
    if (buffer == NULL)
    {
        perror("Error: memory allocation failed\n");
        exit(1);
    }
    while (fgets(buffer, bufferSize, input) != NULL)
    {
        char *newBuffer = replaceWordInString(buffer, searchWord, replaceWord);
        if (newBuffer != NULL)
        {
            fputs(newBuffer, output);
            free(newBuffer);
        }
    }
    free(buffer);
}


int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("command takes 3 arguments. [file to search] [word to search for] [word to replace with]\n");
        return 1;
    }

    char *file = argv[1];
    char *searchWord = argv[2];
    char *replaceWord = argv[3];

    if (strlen(searchWord) == 0 || strlen(replaceWord) == 0)
    {
        printf("Error: search word and replace word must not be empty\n");
        return 1;
    }

    if (strlen(searchWord) >= 50 || strlen(replaceWord) >= 50)
    {
        printf("Error: search word and replace word can not be longer than 99 characters\n");
        return 1;
    }

    printf("Searching for '%s' and replacing with '%s' in file '%s'\n", searchWord, replaceWord, file);
    FILE *fp = fopen(file, "r");
    if (fp == NULL)
    {
        perror("Error opening file");
        return 1;
    }
    FILE *temp = fopen("modifiedText.txt", "w");
    if (temp == NULL)
    {
        perror("Error: replace file could not be created\n");
        return 1;
    }
    processFile(fp, temp, searchWord, replaceWord);

    fclose(fp);
    fclose(temp);

    return 0;
}