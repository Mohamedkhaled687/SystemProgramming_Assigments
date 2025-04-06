#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

#define BUF_SIZE 1024
#define true (char)1

int main(int argc, char **argv)
{
    char buf[BUF_SIZE];

    while (true)
    {
        // Display the shell prompt
        printf("Micro Shell Prompt > ");
        fflush(stdout);

        /* Read Input from User */
        if (!fgets(buf, BUF_SIZE, stdin))
        {
            perror("Error reading input");
            continue;
        }

        // Remove the newline character from the input
        buf[strcspn(buf, "\n")] = '\0';

        // Skip empty input
        if (strlen(buf) == 0)
            continue;

        // Check for the "exit" command
        if (strcmp(buf, "exit") == 0)
        {
            printf("Exiting shell...\n");
            exit(EXIT_SUCCESS);
        }

        /* Tokenize the input string using the custom loop */
        int argsSize = 10;  // initial capacity for tokens
        char **args = malloc(argsSize * sizeof(char *));
        if (args == NULL) {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
        char *input_file = NULL;
        char *output_file = NULL;
        char *error_file = NULL;
        int arg_count = 0;

        char *token = strtok(buf, " ");
        while (token)
        {
            if (strcmp(token, "<") == 0)
            {
                token = strtok(NULL, " ");
                input_file = token;
            }
            else if (strcmp(token, ">") == 0)
            {
                token = strtok(NULL, " ");
                output_file = token;
            }
            else if (strcmp(token, "2>") == 0)
            {
                token = strtok(NULL, " ");
                error_file = token;
            }
            else
            {
                if (arg_count >= argsSize - 1) {  // Ensure space for the NULL terminator
                    argsSize *= 2;
                    char **temp = realloc(args, argsSize * sizeof(char *));
                    if (temp == NULL) {
                        perror("realloc failed");
                        free(args);
                        exit(EXIT_FAILURE);
                    }
                    args = temp;
                }
                args[arg_count++] = token;
            }
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL; // Terminate the argument list

        if (strcmp(args[0], "cd") == 0)
        {
            if (args[1] == NULL)
            {
                perror("cd: missing argument\n");
            }
            else
            {
                if (chdir(args[1]) != 0)
                {
                    perror("cd failed");
                }
            }
            free(args);
            continue;
        }

        /* Handle variable assignment of the form x=5 (without export) */
        if (strchr(args[0], '=') != NULL)
        {
            if (args[1] != NULL)
            {
                perror("Usage: Variable=Value (no spaces allowed)\n");
            }
            else
            {
                char *eq = strchr(args[0], '=');
                *eq = '\0';          // Terminate the variable name string
                char *var = args[0]; // "x"
                char *val = eq + 1;  // "5"
                if (setenv(var, val, 1) != 0)
                {
                    perror("setenv failed");
                }
            }
            free(args);
            continue;
        }

        /* Handle export variable assignment: export x=5 */
        if (strcmp(args[0], "export") == 0)
        {
            if (args[1] == NULL)
            {
                perror("Usage: export Variable=Value (no spaces allowed)\n");
            }
            else if (strchr(args[1], '=') != NULL)
            {
                char *eq = strchr(args[1], '=');
                *eq = '\0';
                char *var = args[1]; // "x"
                char *val = eq + 1;  // "5"
                if (setenv(var, val, 1) != 0)
                {
                    perror("setenv failed");
                }
            }
            else
            {
                perror("Usage: export Variable=Value (no spaces allowed)\n");
            }
            free(args);
            continue;
        }

        for (int j = 0; j < arg_count; j++)
        {
            if (args[j][0] == '$')
            {
                char *env_value = getenv(args[j] + 1); // Skip the '$'
                args[j] = env_value ? env_value : "";
            }
        }

        /* Fork and execute external commands */
        pid_t pid = fork();
        if (pid > 0)
        { // Parent process
            int status;
            wait(&status); // Wait for the child process to finish
            free(args);
        }
        else if (pid == 0)
        { // Child process

            /* Perform IO redirection in the child process */
            if (input_file != NULL)
            {
                int fd_in = open(input_file, O_RDONLY);
                if (fd_in < 0)
                {
                    perror("Error opening input file");
                    exit(EXIT_FAILURE);
                }
                dup2(fd_in, STDIN_FILENO);
                close(fd_in);
            }
            if (output_file != NULL)
            {
                int fd_out = open(output_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
                if (fd_out < 0)
                {
                    perror("Error opening output file");
                    exit(EXIT_FAILURE);
                }
                dup2(fd_out, STDOUT_FILENO);
                close(fd_out);
            }
            if (error_file != NULL)
            {
                int fd_err = open(error_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
                if (fd_err < 0)
                {
                    perror("Error opening error file");
                    exit(EXIT_FAILURE);
                }
                dup2(fd_err, STDERR_FILENO);
                close(fd_err);
            }

            execvp(args[0], args);
            perror("execvp failed");
            free(args);
            exit(EXIT_FAILURE);
        }
        else
        { // Fork failed
            perror("Fork failed");
            free(args);
        }
    }

    return 0;
}
