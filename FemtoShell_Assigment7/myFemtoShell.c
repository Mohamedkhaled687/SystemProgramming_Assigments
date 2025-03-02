#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 1024
#define true 1

void echo_command(char *s) {
    if (write(STDOUT_FILENO, s, strlen(s)) < 0) {
        printf("Couldn't write the string after echo\n");
    }
    write(STDOUT_FILENO, "\n", 1);  // Add newline after output
}

int main(int argc, char *argv[]) {
    char buf[BUF_SIZE];
    char command[10];
    char arguments[100];

    while (true) {
        printf("Femto Shell Prompt > ");

        // Read user input
        if (!fgets(buf, BUF_SIZE, stdin)) {
            printf("\nError reading input\n");
            continue;
        }

        // Remove newline from fgets since fgets automatically adds newline when the user presses Enter
        buf[strcspn(buf, "\n")] = '\0';

        // Extract the first word as the command
        char *ptr = buf;
        sscanf(ptr, "%s", command);

        // Move ptr past the command
        ptr += strlen(command);

        // Skip any spaces after the command
        while (*ptr == ' ') {
            ptr++;
        }

        // Extract arguments using strcpy (newline already removed from buf)
        strcpy(arguments, ptr);

        // Handle commands
        if (strcmp(command, "echo") == 0) {
            echo_command(arguments);
        } else if (strcmp(command, "exit") == 0) {
            printf("Goodbye! :)\n");
            exit(EXIT_SUCCESS);
        } else {
            printf("Invalid Command\n");
        }
    }
}

