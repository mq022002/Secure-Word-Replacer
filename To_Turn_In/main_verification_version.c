#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Code by MQ and MH
// 1-1: Buffer Overruns
// 3-1: Failure to Handle Errors Correctly
/*
This block of code is a helper function that replaces a word in a string with another word.
It takes a pointer to the original string, the word to search for, and the word to replace it with.
Upon successful replacement, it returns an updated string with the replaced word.
*/
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

// Code by MQ
// 1-2: Buffer Overruns
// 3-2: Failure to Handle Errors Correctly
/*
This block of code processes a file by reading each line, and making calls to a helper function to replace the target word.
It takes a file pointer for the input file, a file pointer for the output file, the word to search for, and the word to 
*/
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


// Code by MH
// 2-1: Format String Problems
// 3-3: Failure to Handle Errors Correctly
// 4-1: Information Leakage
// 5-1: Poor Usability
/*
The main function takes command line arguments for the file to search, the word to search for, and the word to replace it with.
It does some basic error checking on the input arguments, opens the input file, and creates a new file to write the modified text.
It also provides some simple user feedback on the search and replace operation.
*/
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