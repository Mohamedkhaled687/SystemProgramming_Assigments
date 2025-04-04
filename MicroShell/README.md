# MicroShell

MicroShell is a simple Unix shell implemented in C as part of a system programming assignment. It provides a minimal interactive command-line interface with support for built-in commands, environment variable assignments, variable expansion, I/O redirection, and the execution of external commands through forking.

## Table of Contents
- [Features](#features)
- [Usage](#usage)
- [Built-in Commands](#built-in-commands)
- [Compilation and Installation](#compilation-and-installation)
- [Testing](#testing)
- [Example Output](#example-output)

## Features
- **Interactive Shell Prompt:** Displays `Nano Shell Prompt > ` for user input.  
  *(Note: The prompt remains "Nano Shell Prompt >" as defined in the source code.)*
- **Command Parsing and Execution:** Reads input from the user, tokenizes commands, and executes external commands using `fork()` and `execvp()`.
- **Built-in Command Support:** Implements built-in commands such as:
  - `exit` to quit the shell.
  - `cd` to change the current directory.
  - Variable assignment (e.g., `x=5`) without export.
  - Exporting environment variables with `export x=5`.
- **Environment Variable Expansion:** Replaces tokens starting with `$` with their corresponding environment variable values.
- **I/O Redirection:** Supports:
  - Standard output redirection using `>filename`
  - Standard input redirection using `<filename`
  - Standard error redirection using `2>filename`
- **Error Handling:** Provides error messages for invalid commands, missing arguments, redirection issues, and execution failures.

## Usage
1. Run MicroShell from the terminal.
2. The shell displays the prompt:  
   `Nano Shell Prompt > `  
   *(Although the shell is named MicroShell, the prompt text is defined as "Nano Shell Prompt >" in the source code.)*
3. Type your commands at the prompt. Example commands:
   - Change directory:  
     `cd <directory_path>`
   - Assign a variable:  
     `VAR=value`
   - Export a variable:  
     `export VAR=value`
   - Execute an external command (e.g., listing files):  
     `ls -l`
   - Redirect output:  
     `command >output.txt`
   - Redirect input:  
     `command <input.txt`
   - Redirect error output:  
     `command 2>error.txt`
   - Exit the shell:  
     `exit`
4. MicroShell will process commands until you exit.

## Built-in Commands
- **cd**: Changes the current working directory.
  - **Usage:** `cd <directory_path>`
  - Displays an error if the directory is not provided or the change fails.
- **exit**: Exits MicroShell.
  - **Usage:** `exit`
- **Variable Assignment (Local):** Assigns a value to a variable without exporting it.
  - **Usage:** `VAR=value` (do not include spaces around `=`)
- **Export:** Exports a variable to the environment.
  - **Usage:** `export VAR=value` (do not include spaces around `=`)
- **Variable Expansion:** References to environment variables can be made using the `$` sign (e.g., `$VAR`).

## Compilation and Installation
To compile MicroShell, use GCC or any standard C compiler. From the terminal, run:
```bash
gcc -o microshell microshell.c

```


## Example Output

``` bash

Micro Shell Prompt > echo hello > output.txt
Micro Shell Prompt > ls
(content of current directory) output.txt

Micro Shell Prompt > cat output.txt
hello

Micro Shell Prompt > export folder=/home
Micro Shell Prompt > ls $folder
username

```