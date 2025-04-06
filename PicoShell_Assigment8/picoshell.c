#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUF_SIZE 1024
#define true (char)1

int main(int argc, char **argv) {
    char buf[BUF_SIZE];
    char currentdir[150];

    while (true) {
        /* Getting the current directory */
        if (getcwd(currentdir, sizeof(currentdir)) == NULL)
            printf("Hello in Pico Shell\n");
        else
             printf("%s:$ ", currentdir);

        /* Read Input from User */
        if (!fgets(buf, BUF_SIZE, stdin)) {
            printf("Error reading input\n");
            continue;
        }

        buf[strlen(buf) - 1] = 0; // Remove newline character

        if (strlen(buf) == 0)
            continue;

        /* Check for the 'exit' command before forking as it isn't external program to execute by execvp*/
        if (strcmp(buf, "exit") == 0) {
            printf("Exiting shell...\n");
            exit(EXIT_SUCCESS);
        }

        /* Tokenization of the input string */
        int argsSize = 10; // initial capacity for tokens
        char **args = malloc(argsSize * sizeof(char *));
        if (args == NULL) {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
        char *token = strtok(buf, " ");
        int i = 0;
        while (token != NULL) {
            if (i >= argsSize - 1) {  // Need more mem for the NULL terminator
                argsSize *= 2;
                char **temp = realloc(args, argsSize * sizeof(char *));
                if (temp == NULL) {
                    perror("realloc failed");
                    free(args);
                    exit(EXIT_FAILURE);
                }
                args = temp;
            }
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        /* Check for the 'cd' command before forking as it isn't also external program to execute*/
        if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                printf("Missing argument for cd\n");
            } else {
                if (chdir(args[1]) != 0) {
                    perror("cd failed");
                }
            }
            free(args);
            continue;
        }

        /* Fork and execute external commands */
        pid_t pid = fork();

        if (pid > 0) {  // Parent process
            int status;
            wait(&status);

            if (!WIFEXITED(status)) {
                printf("Cannot suspend the execution of the current thread\n");
            }
            free(args);
        } 
        else if (pid == 0) {  // Child process
            execvp(args[0], args);
            printf("Execution failed\n");
            free(args);
            exit(EXIT_FAILURE);
        } 
        else {  // Fork failed
            printf("Fork Failed\n");
            free(args);
        }
    }

    return 0;
}
