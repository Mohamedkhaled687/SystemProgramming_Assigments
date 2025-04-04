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
        printf("Nano Shell Prompt > ");
        fflush(stdout);

        /* Read Input from User */
        if (!fgets(buf, BUF_SIZE, stdin))
        {
            perror("Error reading input");
            continue;
        }

        // Remove the newline character at the end of the input
        buf[strcspn(buf, "\n")] = '\0';

        // Skip empty input
        if (strlen(buf) == 0)
            continue;

        /* Check for the 'exit' command before processing */
        if (strcmp(buf, "exit") == 0)
        {
            printf("Exiting shell...\n");
            exit(EXIT_SUCCESS);
        }

        /* Tokenization of the input string */
        char *args[BUF_SIZE / 2 + 1];
        char *token = strtok(buf, " ");
        int i = 0;
        while (token != NULL)
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL; // NULL-terminate the arguments array

        /* Check for the 'cd' command before forking */
        if (strcmp(args[0], "cd") == 0)
        {
            if (args[1] == NULL)
            {
                fprintf(stderr, "cd: missing argument\n");
            }
            else
            {
                if (chdir(args[1]) != 0)
                {
                    perror("cd failed");
                }
            }
            continue;
        }

        /* Handle variable assignment of the form x=5 (without export) */
        if (strchr(args[0], '=') != NULL)
        {
            if (args[1] != NULL)
            {
                fprintf(stderr, "Usage: Variable=Value (no spaces allowed)\n");
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
            continue;
        }

        /* Handle export variable assignment: export x=5 */
        if (strcmp(args[0], "export") == 0)
        {
            if (args[1] == NULL)
            {
                fprintf(stderr, "Usage: export Variable=Value (no spaces allowed)\n");
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
                fprintf(stderr, "Usage: export Variable=Value (no spaces allowed)\n");
            }
            continue;
        }

        for (int j = 0; j < i; j++)
        {
            if (args[j][0] == '$')
            {
                char *env_value = getenv(args[j] + 1);
                args[j] = env_value ? env_value : "";
            }
        }

        /* Fork and execute external commands */
        pid_t pid = fork();
        if (pid > 0)
        { // Parent process
            int status;
            wait(&status);
        }
        else if (pid == 0)
        { // Child process

            /* IO redirection (performed in the child process) */
            for (int j = 0; j < i; j++)
            {
                /* Output redirection: >filename */
                if (args[j][0] == '>')
                {
                    char *output_file = args[j] + 1; // Skip the '>'
                    int output_fd;
                    if ((output_fd = open(output_file, O_CREAT | O_WRONLY | O_TRUNC, 0644)) < 0)
                    {
                        perror("Can't redirect the output");
                        exit(EXIT_FAILURE);
                    }
                    dup2(output_fd, 1); // Redirect stdout to output_fd
                    close(output_fd);
                }
                /* Input redirection: <filename */
                else if (args[j][0] == '<')
                {
                    char *input_file = args[j] + 1; // Skip the '<'
                    int input_fd;
                    if ((input_fd = open(input_file, O_RDONLY)) < 0)
                    {
                        perror("Can't redirect the input");
                        exit(EXIT_FAILURE);
                    }
                    dup2(input_fd, 0); // Redirect stdin to input_fd
                    close(input_fd);
                }
                /* Error redirection: 2>filename */
                else if (args[j][0] == '2' && args[j][1] == '>')
                {
                    char *error_file = args[j] + 2; // Skip the "2>"
                    int error_fd;
                    if ((error_fd = open(error_file, O_CREAT | O_WRONLY | O_TRUNC, 0644)) < 0)
                    {
                        perror("Can't redirect the error file");
                        exit(EXIT_FAILURE);
                    }
                    dup2(error_fd, 2); // Redirect stderr to error_fd
                    close(error_fd);
                }
            }
            execvp(args[0], args);
            perror("Execution failed");
            exit(EXIT_FAILURE);
        }
        else
            perror("Fork failed");
    }
    return 0;
}
