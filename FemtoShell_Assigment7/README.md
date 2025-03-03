# Femto Shell

○ Femto Shell is a simple command-line shell written in C. It demonstrates basic command parsing and handling with support for the echo and exit commands. The shell reads user input, extracts the command and its arguments, and performs the appropriate action.

## Feature

○ Echo Command: Prints the string provided after the echo command.

○ Exit Command: Exits the shell with a goodbye message.

○ Input Handling: Uses fgets() for input and removes the newline character for clean processing.


## Compilation
○ To Compile the program use the following commands .

``` bash 
 gcc FemtoShell.c -o myFemtoShell

```
This command compiles the code in FemtoShell.c and produces an executable named myFemtoShell.

## Usage 

○ Run the executable from the terminal:

``` bash 

./myFemtoShell

```

## Example Output

``` bash

Femto Shell Prompt > echo Hello, World!
Hello, World!
Femto Shell Prompt > exit
Goodbye! :)

```




