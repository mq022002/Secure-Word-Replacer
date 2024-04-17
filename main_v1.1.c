#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isWordChar(char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_')
    {
        return 1;
    }
    if (c == '-')
    {
        return 0;
    }
    return 0;
}

char *replaceWordInString(const char *str, const char *searchWord, const char *replaceWord)
{
    int searchWordLen = strlen(searchWord);
    int replaceWordLen = strlen(replaceWord);
    int diff = replaceWordLen - searchWordLen;

    int count = 0;
    const char *tmp = str;
    while ((tmp = strstr(tmp, searchWord)) != NULL)
    {
        count++;
        tmp += searchWordLen;
    }

    int multiplication = count * diff;
    if ((diff > 0 && (multiplication < 0 || multiplication / diff != count)) ||
        (diff < 0 && multiplication > 0))
    {
        fprintf(stderr, "Error: Integer overflow\n");
        return NULL;
    }

    int newLength = strlen(str) + multiplication + 1;
    if (newLength < 0)
    {
        fprintf(stderr, "Error: Integer overflow\n");
        return NULL;
    }

    char *newStr = malloc(newLength);
    if (newStr == NULL)
    {
        printf("Error allocating memory");
        return NULL;
    }

    char *current = newStr;
    int remainingSpace = newLength;
    while (*str)
    {
        if (strstr(str, searchWord) == str)
        {
            if (replaceWordLen > remainingSpace)
            {
                printf("Not enough space in the buffer");
                free(newStr);
                return NULL;
            }
            strncpy(current, replaceWord, replaceWordLen);
            current += replaceWordLen;
            remainingSpace -= replaceWordLen;
            str += searchWordLen;
        }
        else
        {
            *current++ = *str++;
            remainingSpace--;
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
        printf("Error: memory allocation failed\n");
        return;
    }
    while (fgets(buffer, bufferSize, input) != NULL)
    {
        char *newBuffer = replaceWordInString(buffer, searchWord, replaceWord);
        if (newBuffer != NULL)
        {
            fputs(newBuffer, output);
            free(newBuffer);
        }
        else
        {
            free(buffer);
            return;
        }
    }
    free(buffer);
}

int getUserInput(char *file, int fileLen, char *searchWord, int searchWordLen, char *replaceWord, int replaceWordLen)
{
    printf("Enter the file name: ");
    if (fgets(file, fileLen, stdin) == NULL || strlen(file) == 0 || file[strlen(file) - 1] != '\n')
    {
        printf("Error: file name must not be empty and can not be longer than 99 characters\n");
        return 1;
    }
    file[strlen(file) - 1] = '\0';

    printf("Enter the search word: ");
    if (fgets(searchWord, searchWordLen, stdin) == NULL || strlen(searchWord) == 0 || searchWord[strlen(searchWord) - 1] != '\n')
    {
        printf("Error: search word must not be empty and can not be longer than 49 characters\n");
        return 1;
    }
    searchWord[strlen(searchWord) - 1] = '\0';

    printf("Enter the replace word: ");
    if (fgets(replaceWord, replaceWordLen, stdin) == NULL || strlen(replaceWord) == 0 || replaceWord[strlen(replaceWord) - 1] != '\n')
    {
        printf("Error: replace word must not be empty and can not be longer than 49 characters\n");
        return 1;
    }
    replaceWord[strlen(replaceWord) - 1] = '\0';

    if (strlen(searchWord) == 0 || strlen(replaceWord) == 0)
    {
        printf("Error: search word and replace word must not be empty\n");
        return 1;
    }

    return 0;
}

int processUserInput(const char *file, const char *searchWord, const char *replaceWord)
{
    printf("Searching for '%s' and replacing with '%s' in file '%s'\n", searchWord, replaceWord, file);
    FILE *fp = fopen(file, "r");
    if (fp == NULL)
    {
        printf("Error opening file");
        return 1;
    }
    FILE *temp = fopen("modifiedText.txt", "w");
    if (temp == NULL)
    {
        printf("Error: replace file could not be created\n");
        fclose(fp);
        return 1;
    }
    processFile(fp, temp, searchWord, replaceWord);

    fclose(fp);
    fclose(temp);

    return 0;
}

int main()
{
    char file[100];
    char searchWord[50];
    char replaceWord[50];

    if (getUserInput(file, sizeof(file), searchWord, sizeof(searchWord), replaceWord, sizeof(replaceWord)) != 0)
    {
        return 1;
    }

    if (processUserInput(file, searchWord, replaceWord) != 0)
    {
        return 1;
    }

    return 0;
}