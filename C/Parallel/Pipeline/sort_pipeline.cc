/**************************************************************
  File:          sort_pipeline.cc
  Description:   A program that implements a sorting function
                 for an array using a pipeline model. 

***************************************************************/

#include <cstdlib>
#include <pthread.h>
#include <sys/time.h>
#include <iostream>
using namespace std;
#include "sort_pipeline.h"

// global variables 
int insert_part=0, size, limit=10, no_threads, *index; 
int counter = 0, global_id = 0, threads_done=0, global_sum = 0;; 
int *a = NULL, *result=NULL;
pthread_mutex_t id_mutex, barrier_mutex, cout_mutex, *pipe_mutex, p_mutex, allin_mutex;
pthread_cond_t allin_cond, *pipe_cond;

// Creates the threads calling the Ship_check_in function.
void Create_threads(pthread_t *threads, int no_threads)
{
    for (int i=0; i<no_threads; i++)
        if (pthread_create(&threads[i], NULL, Parallel_insertion_sort, NULL) != 0)
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

// Inputs the number of ships and initializes the global variables.
void Init_data(int &nthreads)
{
    int i;
    pthread_mutex_init(&id_mutex, NULL);
    pthread_mutex_init(&barrier_mutex, NULL);
    pthread_mutex_init(&cout_mutex, NULL);
    pthread_mutex_init(&p_mutex, NULL);
    pthread_mutex_init(&allin_mutex, NULL);
    pthread_cond_init(&allin_cond, NULL);
    cout << "Enter the size of the array" << endl;
    cin >> size;
    cout << "Enter the upper limit for the elements of the array" << endl;
    cin >> limit;
    a = new int[size];
    for (i=0; i<size; i++) {
        a[i] = rand() % limit;
        cout << a[i] << ' ';
    }
    cout << endl;
    cout << "Enter the number of threads" << endl;
    cin >> no_threads;
    nthreads = no_threads;
    result = new int[no_threads];
    pipe_mutex = new pthread_mutex_t[no_threads];
    pipe_cond = new pthread_cond_t[no_threads];
    index = new int[no_threads];
    for (i=0; i<no_threads; i++) {
        pthread_mutex_init(&pipe_mutex[i], NULL);
        pthread_cond_init(&pipe_cond[i], NULL);
        index[i] = -1;
    }
}

// A barrier function based on the number of threads implemented using
// a conditional variable. It uses a global counter intialized as
// 0. Each thread coming in increments the counter. If the count is
// not equal to the number of threads, it goes into a wait on the
// conditional variable. The thread that finds the counter equal to
// the number of threads broadcasts the condition that releases all of
// them.
void Barrier(int no_threads) 
{
    static int count = 0;
    pthread_mutex_lock(&barrier_mutex);
    count++;
    if (count == no_threads) {
        pthread_cond_broadcast(&allin_cond); // last one in lets everyone go
        count = 0;
    }
    else
        pthread_cond_wait(&allin_cond, &barrier_mutex); 
    pthread_mutex_unlock(&barrier_mutex);
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
void Wait(int delay)
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

// Swaps two integer variables.
void Swap(int &a, int &b) 
{
    int temp = a;
    a = b;
    b = temp;
}

// Loops through a local array once and swaps any adjacent elements
// that are not in the correct order.
void Local_loop(int my_start, int my_end, int id)
{
    int enrty_point = 0;
    int i;

    // Set entry_point for while loop to the global
    // insertion variable only if it is inbetween
    // my_start and my_end
    if (insert_part >= my_start){

        if (insert_part <= my_end)        
            enrty_point = insert_part;
    }
    
    // If global insertion variable is not in between
    // my_start and my_end, begin at the end assuming
    // the start is within the already sorted part of
    // the insertion sort array
    else 
        enrty_point = my_end;
    
    // Update local iterator so the while loop knows where to start
    i = enrty_point-1;

    // Loop runs while entry_point is greater than the start and while
    // the current value is larger than the next.
    while (enrty_point >= my_start && a[i] > a[i+1]){

        // **************************************************************************
        // Left following test code in to verify multiple threads are moving multiple
        // values as needed to be certain the programe is actually parallel
        // **************************************************************************

       /*pthread_mutex_lock(&cout_mutex);
        cout << "Thread ID " << id << " is between index " << my_start << " and index " << my_end <<
                " and is swapping " << a[i] << " at index " << i << " with " << a[i+1] << " at index " <<  i+1 << " from entry point " << enrty_point << ".\n";
       pthread_mutex_unlock(&cout_mutex);*/
       
        Swap(a[i], a[i+1]);
        i--;
    }
}

// Multi-threaded insertion sort implemented as a pipeline. Each thread
// handles an equal portion of the array with an overlap of 1 element
// between each adjacent threads. In the first iteration, only the
// thread 0 loops over its array once. In the second iteration, both
// thread 0 and thread 1 can do their local loop and so on. A barrier
// insures that the operations are done in the corrcet order.
void *Parallel_insertion_sort(void *arg)
{
    int id, i;
    Get_id(id);
    
    int lsize = size/no_threads;
    if (size % no_threads != 0)
        lsize++;
    int my_start = id*lsize;
    int my_end = min(size-1, (id+1)*lsize);
    
    // i starts with a negative value to delay when each thread starts
    // their own local loop. The upper bound is set up such that all
    // threads will keep coming into the barrier until all of them have
    // finished doing the loop, to avoid a deadlock.
    for (i=-id; i<size+no_threads-id; i++) {
        
        // Start local loop for all threads
        Local_loop(my_start, my_end, id);
        
        // Iterate global only if global is currently in between
        // my_start and my_end
        if (insert_part >= my_start && insert_part < my_end){
            pthread_mutex_lock(&p_mutex);
            insert_part++;
            pthread_mutex_unlock(&p_mutex);
        }

        // Use barrier to be certain all threads have finished doing
        // their local loop to avoid deadlock
        Barrier(no_threads);
    } 

    if (id == no_threads-1) {
        cout << "Here is the sorted array:" << endl;
        Output_array();
    }

    return NULL;
}

// Quick verification of if the array is sorted using int pointers.
// Disable call in Output_array() testing for efficiency.
bool isSorted(const int *a) {

    const int *previous_value=a;
    int lsize=size;

    while (lsize)
    {
       if (*a < *previous_value)
             return false;
       previous_value= a;

       ++a;
       --lsize;
     }
     return true;
}

// Outputs all the elements in the array to the console.
void Output_array()
{
    for (int i=0; i<size; i++) 
        cout << a[i] << ' ';
    cout << endl;

    // Test if array is sorted correctly, comment out if desired
    cout << "The answer to if the array is sorted is "; 
    if(isSorted(a)) cout << "true.\n";
    else cout << "false.\n";
}

