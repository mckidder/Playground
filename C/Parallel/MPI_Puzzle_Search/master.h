/************************************************************************
  File:          master.h                         
  Description:   Functions for the master.                       
  Date:          November 2018
************************************************************************/

#ifndef MASTER_H
#define MASTER_H

#include "puzzle.h"

// The master that generates tasks and assigns them to worker
// processes.
void Master(int nr_proc);

// Sends all of the workers a stopping signal.
void Finish_all(int nr_workers, char *word, Puzzle &the_puzzle);

// The master must receive the result from a worker consisting of 5
// integers, one for the size of the word the worker has been looking
// for, two for the position where it was found, and two for the
// direction in which it was found. The master must then call the
// Delete method from the puzzle with these five parameters, and then
// return the identity of the worker.
// A size of 0 should mean that the word was not found in
// the grid. The function should return the identity of the process it
// has received the result from.
int Recv_result(Puzzle &the_puzzle);

#endif
