#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int fileExists(const char *filename)
{
    struct _stat buffer;
    return (_stat(filename, &buffer) == 0);
}

int isTxtFile(const char *filename)
{
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename)
        return 0;
    return strcmp(dot, ".txt") == 0;
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
        exit(1);
    }
    while (fgets(buffer, bufferSize, input) != NULL)
    {
        if (strlen(buffer) >= bufferSize)
        {
            printf("Error: Buffer overflow\n");
            free(buffer);
            return;
        }
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

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("command takes 4 arguments. [file to search] [word to search for] [word to replace with] [output file]\n");
        return 1;
    }

    char *file = argv[1];
    char *searchWord = argv[2];
    char *replaceWord = argv[3];
    char *outputFile = argv[4];

    if (fileExists(outputFile))
    {
        char response[4];
        printf("The file %s already exists. Do you want to overwrite it? (yes/no): ", outputFile);
        fgets(response, sizeof(response), stdin);
        response[strcspn(response, "\n")] = 0;
        if (strcmp(response, "yes") != 0)
        {
            printf("Aborted by user.\n");
            return 1;
        }
    }

    if (!isTxtFile(file) || !isTxtFile(outputFile))
    {
        printf("Error: Only .txt files are supported\n");
        return 1;
    }

    if (strlen(searchWord) == 0 || strlen(replaceWord) == 0)
    {
        printf("Error: search word and replace word must not be empty\n");
        return 1;
    }

    if (strlen(searchWord) >= 70 || strlen(replaceWord) >= 70)
    {
        printf("Error: search word and replace word can not be longer than 69 characters\n");
        return 1;
    }

    printf("Searching for '%s' and replacing with '%s' in file '%s'\n", searchWord, replaceWord, file);

    FILE *fp = fopen(file, "r");
    if (fp == NULL)
    {
        printf("Error opening file");
        return 1;
    }
    FILE *temp = fopen(outputFile, "w");
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