# This is a simple program that is able to display the truth table for a given equation

## Valid Operations (in order of precedence)
- `()` Parentheses for grouping
- `!` NOT
- `*` AND
- `+` OR

## Building and Running
`table-gen` requires an ISO compliant C++11 compiler to compile and `make` to run the Makefile. To build using the Makefile
simply use
```
make build
```
and a new file called `table-gen` will be made in the directory that `make` was called in. 
To run `table-gen`, simple type `./table-gen` in the directory that the executable is in. You can pass in a command-line
argument for the equation while calling `table-gen` (note this will only read the first argument and not any others). You
can also call `table-gen` without arguments, and it will ask for you an equation.

## Features
- Whitespace does not matter
- You can use variables within the equation and the program will populate the values for those variables in the truth
table accordingly
