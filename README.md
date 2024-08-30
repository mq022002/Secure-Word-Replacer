# THIS REPOSITORY HAS BEEN ARCHIVED AFTER SUCCESSFUL COMPLETION OF THS PROJECT

# CS 455 - Secure Software Development Course Project

## Project Description
1) The program starts by checking if the correct number of command-line arguments are provided.
2) It validates the search word and replace word.
3) It checks if the output file already exists. If it does, it asks the user for confirmation before overwriting it.
4) It opens the input file for reading and the output file for writing.
5) It reads the input file line by line.
6) For each line, it calls a function to replace all occurrences of the search word with the replace word.
7) It writes the modified line to the output file.
8) It repeats steps 5-7 until all lines in the input file are processed.
9) It closes both files and exits.

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
    gcc main.c -o main
    ```
    - The following command is how we use our program. Please replace [parameters] with your specified parameters:
    ```bash
    ./main [file to search] [word to search for] [word to replace with] [output file]
    ```
    - Example usage:
    ```bash
    ./main text.txt for yummy output.txt
    ```
