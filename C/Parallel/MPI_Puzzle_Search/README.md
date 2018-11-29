# Parallel Programming: MPI Word Puzzle Search

Simple program using MPI to search a provided
grid containing a word puzzle. Each 8-direction
for each positon, defined by row position and 
column position, are searched for known words
contained in the puzzle and then overwritten
with a space. The remaining letters will show
a secret message hidden within the grid.

run "make" and then "make run" to execute
