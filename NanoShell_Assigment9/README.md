# NanoShell

NanoShell is a simple Unix shell implemented in C as part of a system programming assignment. It provides a minimal interactive command-line interface with support for built-in commands, environment variable assignments, variable expansion, and the execution of external commands through forking.

## Table of Contents
- [Features](#features)
- [Usage](#usage)
- [Built-in Commands](#built-in-commands)
- [Compilation and Installation](#compilation-and-installation)
- [Testing](#testing)
- [Example Output](#example-output)

## Features
- **Interactive Shell Prompt:** Displays `Nano Shell Prompt > ` for user input.
- **Command Parsing and Execution:** Reads input, tokenizes commands, and executes external commands using `fork()` and `execvp()`.
- **Built-in Command Support:** Implements built-in commands such as:
  - `exit` to quit the shell.
  - `cd` to change the current directory.
  - Variable assignment (e.g., `x=5`) without export.
  - Exporting environment variables with `export x=5`.
- **Environment Variable Expansion:** Replaces tokens starting with `$` with their corresponding environment variable values.
- **Error Handling:** Provides appropriate error messages for invalid commands, missing arguments, and execution failures.

## Usage
1. Run NanoShell from the terminal.
2. The shell displays the prompt:  
   `Nano Shell Prompt > `
3. Type your commands at the prompt. Example commands:
   - Change directory:  
     `cd <directory_path>`
   - Assign a variable:  
     `VAR=value`
   - Export a variable:  
     `export VAR=value`
   - Execute an external command (e.g., listing files):  
     `ls -l`
   - Exit the shell:  
     `exit`
4. NanoShell will process commands until you exit.

## Built-in Commands
- **cd**: Changes the current working directory.
  - Usage: `cd <directory_path>`
  - Displays an error if the directory is not provided or the change fails.
- **exit**: Exits NanoShell.
  - Usage: `exit`
- **Variable Assignment (Local)**: Assigns a value to a variable without exporting it.
  - Usage: `VAR=value` (do not include spaces around `=`)
- **Export**: Exports a variable to the environment.
  - Usage: `export VAR=value` (do not include spaces around `=`)
- **Variable Expansion**: References to environment variables can be made using the `$` sign (e.g., `$VAR`).

## Compilation and Installation
To compile NanoShell, use GCC or any standard C compiler. From the terminal, run:
```bash
gcc -o nanoshell nanoshell.c
```
After successful compilation, run NanoShell with:
```bash
./nanoshell
```

## Testing
- **Interactive Testing:** Launch NanoShell and test commands such as `ls`, `pwd`, `cd`, variable assignments, and environment variable expansion.
- **Built-in Command Testing:** Verify that `cd`, `exit`, variable assignments, and export commands behave as expected.
- **Error Testing:** Test invalid command scenarios, such as missing arguments for `cd` or incorrect variable assignment syntax, to observe error handling.

## Example Output

``` bash

    Nano Shell Prompt > echo Hello nano
    Hello nano

    Nano Shell Prompt > export folder=\home
    Nano Shell Prompt > ls $folder
    files and folder in home dir

    Nano Shell Prompt > exit
    Exiting the Shell ... 

```