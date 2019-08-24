# SUDOKU GENERATOR

This program generate a sudoku and a solution for it. You can specify
options:

- The number of sudokus to generate
- Show or not the solutions
- Force a puzzle to have an unique solution
- The number of empty cells to have

There is also a function you can use to just solve a sudoku called `solveBoard`.

## Usage

If you run the program with no arguments, it will produce a
sudoku with 35 empty cells, unique solution and will show
the solution.

You can add edit all of this (but you have to specify all of
them).  

`./genSudoku [options]`  

Options are:  
-n \<N> : Generate N puzzles  
-d \<D> : The puzzles have D empty cells  
-u : DO NOT Require the puzzles to have an unique solution  
-s : DO NOT Show the solutions of the puzzles

You can see this in the program with `-h` or `--help` arguments too.

## Procedure

To generate a sudoku I follow the following steps:

1. Start with a empty board.
2. Fill the diagonal boxes randomly (as they are independent
 from the other boxes, we have less checks to do).
3. Using the random generated boxes, solve the board as it
 were a puzzle.
4. Remove values one by one randomly. If the solution has to be
 unique, count the number of solutions and if it is greater
 than one, restore the value and try removing another one.
