#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUF_SIZE 1024
#define true (char)1

int main(int argc, char **argv) {
    char buf[BUF_SIZE];

    while (true) {
        // Display the shell prompt
        printf("Nano Shell Prompt > ");

        /* Read Input from User */
        if (!fgets(buf, BUF_SIZE, stdin)) {
            printf("Error reading input\n");
            continue;
        }

        // Remove the newline character at the end of the input, if present
        size_t len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n') {
            buf[len - 1] = '\0';
        }

        // Skip empty input
        if (strlen(buf) == 0)
            continue;

        /* Check for the 'exit' command before forking */
        if (strcmp(buf, "exit") == 0) {
            printf("Exiting shell...\n");
            exit(EXIT_SUCCESS);
        }

        /* Tokenize the input string */
        char *args[100];
        int i = 0;
        char *token = strtok(buf, " ");
        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;  // NULL-terminate the arguments array

    /*Check for putting space when setting variable*/
        if (i > 1 && strcmp(args[1], "=") == 0) {
            printf("Usage: Variable=Value (no spaces allowed)\n");
            continue;
        }

        /* Variable assignment without spaces: e.g., x=5 */
        if (strchr(args[0], '=') != NULL) {
            // If there is more than one token, it means spaces were used
            if (i > 1) {
                printf("Usage: Variable=Value (no spaces allowed)\n");
            } else {
                char *eq = strchr(args[0], '=');
                *eq = '\0';  // Split into variable name and value
                char *var = args[0];
                char *val = eq + 1;
                if (setenv(var, val, 1) != 0) {
                    perror("setenv failed");
                }
            }
            continue;
        }

        /* Check for the 'cd' command before forking */
        if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                printf("Missing argument for cd\n");
            } else {
                if (chdir(args[1]) != 0) {
                    perror("cd failed");
                }
            }
            continue;
        }

        /* Handle export variable assignment: export x=5 */
        if (strcmp(args[0], "export") == 0) {
            if (args[1] == NULL) {
                printf("Usage: export Variable=Value (no spaces allowed)\n");
            } else if (strchr(args[1], '=') != NULL) {
                // If there are extra tokens, it means spaces were used
                if (i > 2) {
                    printf("Usage: export Variable=Value (no spaces allowed)\n");
                } else {
                    char *eq = strchr(args[1], '=');
                    *eq = '\0';  // Split into variable name and value
                    char *var = args[1];
                    char *val = eq + 1;
                    if (setenv(var, val, 1) != 0) {
                        perror("setenv failed");
                    }
                }
            } else {
                printf("Usage: export Variable=Value (no spaces allowed)\n");
            }
            continue;
        }

        /* Variable Expansion: Replace tokens starting with '$' with their environment value */
        for (int j = 0; j < i; j++) {
            if (args[j][0] == '$') {
                char *env_value = getenv(args[j] + 1);
                if (env_value) {
                    args[j] = env_value;
                } else {
                    args[j] = "";
                }
            }
        }

        /* Fork and execute external commands */
        pid_t pid = fork();
        if (pid > 0) {  // Parent process
            int status;
            wait(&status);
            if (!WIFEXITED(status)) {
                printf("Child process did not exit normally\n");
            }
        } else if (pid == 0) {  // Child process
            execvp(args[0], args);
            // If execvp returns, an error occurred
            printf("Execution failed\n");
            exit(EXIT_FAILURE);
        } else {  // Fork failed
            printf("Fork Failed\n");
        }
    }
    return 0;
}
