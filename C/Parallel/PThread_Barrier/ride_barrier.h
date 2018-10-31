/***************************************************************
  File:          ride_barrier.h
  Description:   A program that implements a barrier using a
                 conditional variable from the pthread library.

****************************************************************/

#ifndef RIDE_BARRIER_H
#define RIDE_BARRIER_H


// Creates the threads calling the Rider_check_in function.
void Create_threads(pthread_t *threads, int no_threads);

// Joins all of the threads so that the program doesn't end before
// they have finished the work.
void Synchronise(pthread_t *threads, int no_threads);

// Inputs the maximum capacity of the car, amount of riders, and initializes the global variables.
void Init_data(int &no_threads, int &car_capac);

// A barrier function based on the number of threads and capacity of the coaster
// implemented using a conditional variable. It uses a global counter intialized as
// 0. Each thread coming in increments the counter. If the count is
// not equal to the maximum capacity of the coaster, it goes into a wait on the
// conditional variable. The thread that finds the counter equal to
// the number of threads broadcasts the condition that releases all of them.
void Ride(int car_capac, int id);

// The thread function: gets an id number, runs for a set amount of iterations
// each time calling the barrier and waiting a fixed time for the coaster to
// go around the track. Each thread then waits a random time and this repeats
// until we read the maximum preset ride total
void *Rider_check_in(void *arg);

// Copies the value of a global id into the reference parameter and
// increments the global id.
void Get_id(int &id);

// Waits for a given delay specified in miliseconds.
void Wait_timed(int miliseconds);

// Wait for a random amount of time
void Wait_rand();

#endif
