/***************************************************************
  File:          div_conquer.h
  Description:   A program that implements a divide and 
                 conquer function computing the sum of an array. 

****************************************************************/

#ifndef DIV_CONQUER_H
#define DIV_CONQUER_H

// Creates the threads calling the Sum_thread function.
void Create_threads(pthread_t *threads, int nr_threads);

// Joins all of the threads so that the program doesn;t end before
// they have finished the work.
void Synchronise(pthread_t *threads, int nr_threads);

// Inputs the number of ships and initializes the global variables.
void Init_data(int &nthreads);

// A barrier function based on the number of threads implemented using
// a conditional variable. It uses a global counter intialized as
// 0. Each thread coming in increments the counter. If the count is
// not equal to the number of threads, it goes into a wait on the
// conditional variable. The thread that finds the counter equal to
// the number of threads broadcasts the condition that releases all of
// them.
void Barrier(int nr_threads) ;


// The searching thread function. The Master thread is thread 0 and
// the Slave threads get their own unique ID before being called
void * Search_thread(void *arg) ;

// Copies the value of a global id into the reference parameter and
// increments the global id.
void Get_id(int &id);

// Waits for a given delay specified in miliseconds.
void Wait(int miliseconds);

// The Master thread that will coordinate the Slave threads and
// then search for the value itself. When all threads have finished
// the Master thread then prints the output.
void Master_search();

// Slave threads to search for a value within an array.
// If the targetted value is found, all threads are updated
// and the search is finished.
void Slave_search(int id);

// Used to access the global result
void Global_to_local_res(int & my_res);

// Used to access the global result
void Report_data(int & my_res);

#endif
