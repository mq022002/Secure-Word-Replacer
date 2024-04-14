# CS 455 - Secure Software Development Course Project
# Matthew Hammond and Matthew Quijano

## Project Description
This is a simple program that allows the user to replace words in a text file. The user is able to enter the name of the file they wish to scan, the word they wish to replace, and what the target word is changed to. Our program uses these general steps to accomplish its task:
1) The program starts by checking if the correct number of command-line arguments are provided.
2) It validates the search word and replace word.
3) It opens the input file for reading and a new file for writing.
4) It reads the input file line by line.
5) For each line, it calls a function to replace all occurrences of the search word with the replace word.
6) It writes the modified line to the output file.
7) It repeats steps 4-6 until all lines in the input file are processed.
7) It closes both files and exits.

## Local Instance Instructions

1) Open Visual Studio Code
2) Clone this GitHub repository: 
```bash
git clone https://github.com/Abbalah19/CS455_Final_Project.git
```
3) When prompted, open project directory
4) Open a Git Bash terminal
5) Usage for our program
    - In your Git Bash, compile our program using the following command:
    ```bash
    gcc main_analysis_version.c -o main
    ```

    - The following command is how we use our program. Please replace [parameters] with your specified parameters:
    ```bash
    ./main [file to search] [word to search for] [word to replace with]
    ```

    - Example usage:
    ```bash
    ./main test_text.txt for yummy
    ```