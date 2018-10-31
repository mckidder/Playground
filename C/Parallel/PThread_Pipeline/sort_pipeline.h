/***************************************************************
  File:          sort_pipeline.h
  Description:   A program that implements a sorting function
                 for an array using a pipeline model. 

****************************************************************/

#ifndef SORT_PIPELINE_H
#define SORT_PIPELINE_H

// Creates the threads calling the Ship_check_in function.
void Create_threads(pthread_t *threads, int no_threads);

// Joins all of the threads so that the program doesn;t end before
// they have finished the work.
void Synchronise(pthread_t *threads, int no_threads);

// Inputs the number of ships and initializes the global variables.
void Init_data(int &nthreads);

// A barrier function based on the number of threads implemented using
// a conditional variable. It uses a global counter intialized as
// 0. Each thread coming in increments the counter. If the count is
// not equal to the number of threads, it goes into a wait on the
// conditional variable. The thread that finds the counter equal to
// the number of threads broadcasts the condition that releases all of
// them.
void Barrier(int no_threads) ;

// Copies the value of a global id into the reference parameter and
// increments the global id.
void Get_id(int &id);

// Waits for a given delay specified in miliseconds.
void Wait(int miliseconds);

// Swaps two integer variables.
void Swap(int &a, int &b);

// Loops through a local array once and swaps any adjacent elements
// that are not in the correct order.
void Local_loop(int my_start, int my_end, int id);

// Multi-threaded bubble sort implemented as a pipeline. Each thread
// handles an equal portion of the array with an overlap of 1 element
// between each adjacent threads. In the first iteration, only the
// thread 0 loops over its array once. In the second iteration, both
// thread 0 and thread 1 can do their local loop and so on. A barrier
// insures that the operations are done in the corrcet order.
void *Parallel_insertion_sort(void *arg);

// Quick verification of if the array is sorted using int pointers.
// Disable call in Output_array() testing for efficiency.
bool isSorted(const int *ar);

// Outputs all the elements in the array to the console.
void  Output_array();

#endif
