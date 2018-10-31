/**************************************************************
  File:          ride_barrier.cc
  Description:   A program that implements a barrier using a
                 conditional variable from the pthread library.
***************************************************************/

#include <cstdlib>
#include <pthread.h>
#include <sys/time.h>
#include <iostream>
#include <cmath>
using namespace std;
#include "ride_barrier.h"

// global variables
int TOTAL_RIDES = 10;
int no_pass, coaster_max;
int counter = 0, global_id = 0, glb_ride_count = 0;
pthread_mutex_t id_mutex, coaster_mutex;
pthread_cond_t allin_cond;

// Creates the threads calling the Rider_check_in function.
void Create_threads(pthread_t *threads, int no_threads)
{
    for (int i=0; i<no_threads; i++)
        if (pthread_create(&threads[i], NULL, Rider_check_in, NULL) != 0)
            cout << "Pthread_create failed" << endl;
}

// Joins all of the threads so that the program doesn't end before
// they have finished the work.
void Synchronise(pthread_t *threads, int no_threads)
{
    for (int i=0; i<no_threads; i++)
        if (pthread_join(threads[i], NULL) != 0)
            cout << "Pthread_join failed" << endl;
}

// Inputs the maximum capacity of the car, amount of riders, and initializes the global variables.
void Init_data(int &no_threads, int &car_capac)
{
    pthread_mutex_init(&id_mutex, NULL);
    pthread_mutex_init(&coaster_mutex, NULL);
    pthread_cond_init(&allin_cond, NULL);
    cout << "Enter the total number of riders" << endl;
    cin >> no_pass;
    cout << "Enter the maximum capacity of the coaster car "
         << "so that the\nnumber of riders is a multiple of the " 
         << "maximum capacity" << endl;
    cin >> coaster_max;
    car_capac = coaster_max;
    cout << "This ride only runs ten times before it needs a break!\n"
         << "******************************************************\n"
         << "The first ride is now filling up.\n\n";
    no_threads = no_pass;
}

// A barrier function based on the number of threads and capacity of the coaster
// implemented using a conditional variable. It uses a global counter intialized as
// 0. Each thread coming in increments the counter. If the count is
// not equal to the maximum capacity of the coaster, it goes into a wait on the
// conditional variable. The thread that finds the counter equal to
// the number of threads broadcasts the condition that releases all of them.
void Ride(int car_capac, int id)
{
    static int count = 0;
    pthread_mutex_lock(&coaster_mutex);
    count++;
    cout << "Rider " << id << " is ready to ride!\n";
    //cout << endl << car_capac << endl;
    if (count == car_capac) {
        pthread_cond_broadcast(&allin_cond); // last one in lets everyone go
        count = 0;
        glb_ride_count++;                    // update total ride counter
        cout << "\nCoaster car is full and riders are going around!\n" << "There are "
             << TOTAL_RIDES - glb_ride_count << " rides remaining.\n";
        if (TOTAL_RIDES - glb_ride_count > 0) 
            cout << "The ride is ready fill up again!\n\n";
    }
    else
        pthread_cond_wait(&allin_cond, &coaster_mutex);
    pthread_mutex_unlock(&coaster_mutex);
}

// The thread function: gets an id number, runs for a set amount of iterations
// each time calling the barrier and waiting a fixed time for the coaster to
// go around the track. Each thread then waits a random time and this repeats
// until we read the maximum preset ride total
void *Rider_check_in(void *arg)
{
    int id;
    Get_id(id);
    // Run for a set amount of rides
    while (glb_ride_count < TOTAL_RIDES){
        // Set barrier to sync all rider threads
        Ride(coaster_max, id);

        Wait_timed(1000); // wait one second for the coaster to go around
        Wait_rand();      // Each rider waits a random time after the ride is done
        }
    return NULL;
}

// Copies the value of a global id into the reference parameter and
// increments the global id.
void Get_id(int &id)
{
    pthread_mutex_lock(&id_mutex);
    id = global_id;
    global_id++;
    pthread_mutex_unlock(&id_mutex);
}

// Waits for a given delay specified in miliseconds.
void Wait_timed(int delay)
{
    struct timeval before, after;
    int timing = 0;
    gettimeofday(&before, 0);
    while (timing < delay) {
        gettimeofday(&after, 0);
        // Add the difference in seconds * 1000 and difference
        // in microseconds divided by 1000
        timing = (after.tv_sec - before.tv_sec)*1000 +
                 (after.tv_usec - before.tv_usec)/1000;
    }
}

// Waits for a random amount of time between two limits.
void Wait_rand()
{
    static const int LOW = 10000, HIGH=20000;
    int cycles = LOW + rand()%(HIGH - LOW);
    double j;
    for (int i=0; i<cycles; i++)
        j = cos(i);
}
