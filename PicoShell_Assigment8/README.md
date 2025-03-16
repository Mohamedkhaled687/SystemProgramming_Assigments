# PicoShell

PicoShell is a simple Unix shell implemented in C as part of a system programming assignment. It demonstrates basic shell functionalities such as reading commands from standard input, tokenizing commands, handling built-in commands like `cd` and `exit`, and executing external commands using `fork()` and `execvp()`.

## Table of Contents
- [Features](#features)
- [Usage](#usage)
- [Built-in Commands](#built-in-commands)
- [Compilation and Installation](#compilation-and-installation)
- [Testing](#testing)
- [Example Output](#example-output)


## Features
- **Interactive Shell:** Displays the current working directory and a prompt to the user.
- **Command Parsing:** Reads user input and tokenizes the command and its arguments.
- **Built-in Command Support:** Executes internal commands like `cd` (change directory) and `exit` (to exit the shell).
- **External Command Execution:** Uses `fork()` to create a child process and `execvp()` to execute external commands.
- **Error Handling:** Provides feedback if an error occurs during command reading, directory changes, or command execution.

## Usage
1. Run PicoShell from the terminal.
2. The shell will display your current working directory followed by `:$ `.
3. Type any supported command. For example:
   - To change the directory:  
     `cd <directory_path>`
   - To execute an external command such as listing files:  
     `ls -l`
   - To exit the shell:  
     `exit`
4. Commands are processed until you type `exit`.

## Built-in Commands
- **cd:** Changes the current directory.
  - Usage: `cd <directory_path>`
  - If no path is provided, PicoShell will display an error message.
- **exit:** Exits the PicoShell session.
  - Usage: `exit`

## Compilation and Installation
To compile PicoShell, use GCC or any standard C compiler. From the terminal, run:
```bash
gcc -o picoshell picoshell.c
```
After successful compilation, run the shell with:
```bash
./picoshell
```

## Testing
- **Interactive Testing:** Run the shell and try commands like `ls`, `pwd`, `cd`, and `exit`.
- **Error Cases:** Test scenarios such as using `cd` without an argument or entering an invalid command to see the error handling in action.

## Example Output

``` bash
    \home\username\currentdir:$ echo Hello, World!
    Hello, World!

    \home\username\currentdir:$ pwd
    \home\username\currentdir

    \home\username\currentdir:$ exit
    Exiting shell...

```