/**************************************************************
  File:          div_conquer.cc
  Description:   A program that implements a divide and 
                 conquer function computing the sum of an array. 

***************************************************************/

#include <cstdlib>
#include <pthread.h>
#include <sys/time.h>
#include <iostream>
using namespace std;
#include "div_conquer.h"

// global variables 
int size, limit, nr_thr, global_res=-1, target_val; 
int counter = 0, global_id = 0, threads_done = 0, global_sum = 0;
int *a = NULL, *result = NULL;
pthread_mutex_t id_mutex, barrier_mutex, data_mutex, glob_mutex;
pthread_cond_t allin_cond;

// Creates the threads calling the Sum_thread function.
void Create_threads(pthread_t *threads, int nr_threads)
{
    for (int i=0; i<nr_threads; i++)
        if (pthread_create(&threads[i], NULL, Search_thread, NULL) != 0)
            cout << "Pthread_create failed" << endl;
}

// Joins all of the threads so that the program doesn't end before
// they have finished the work.
void Synchronise(pthread_t *threads, int nr_threads)
{
    for (int i=0; i<nr_threads; i++)
        if (pthread_join(threads[i], NULL) != 0)
            cout << "Pthread_join failed" << endl;
}

// Inputs array size and range, gets search target, and initializes the global variables.
void Init_data(int &nr_threads)
{
    pthread_mutex_init(&id_mutex, NULL);
    pthread_mutex_init(&barrier_mutex, NULL);
    pthread_mutex_init(&data_mutex, NULL);
    pthread_mutex_init(&glob_mutex, NULL);
    pthread_cond_init(&allin_cond, NULL);

    cout << "Enter the size of the array" << endl;
    cin >> size;
    a = new int[size];
    cout << "Enter the range of values (from zero to your choice)" << endl;
    cin >> limit;
    cout << "\nPrinting array\n\n";
    for (int i=0; i<size; i++) {
        a[i] = rand() % limit;
        cout << a[i] << ' ';
        if (i !=0 && i % 9 == 0)
            cout << endl;
    }
    cout << endl << endl;
    cout << "Enter the target value to search for" << endl;
    cin >> target_val;
    cout << "Enter the number of threads" << endl;
    cin >> nr_threads;
    nr_thr = nr_threads;
    result = new int[nr_threads];
}

// A barrier function based on the number of threads implemented using
// a conditional variable. It uses a global counter intialized as
// 0. Each thread coming in increments the counter. If the count is
// nrt equal to the number of threads, it goes into a wait on the
// conditional variable. The thread that finds the counter equal to
// the number of threads broadcasts the condition that releases all of
// them.
void Barrier(int nr_threads) 
{
    static int count = 0;
    pthread_mutex_lock(&barrier_mutex);
    count++;
    if (count == nr_threads) {
        pthread_cond_broadcast(&allin_cond); // last one in lets everyone go
        count = 0;
    }
    else
        pthread_cond_wait(&allin_cond, &barrier_mutex); 
    pthread_mutex_unlock(&barrier_mutex);
}

// The searching thread function. The Master thread is thread 0 and
// the Slave threads get their own unique ID before being called
void * Search_thread(void *arg) 
{
    int id;

    Get_id(id);
    if (id == 0)
        Master_search();
    else
        Slave_search(id);

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

// The Master thread that will coordinate the Slave threads and
// then search for the value itself. When all threads have finished
// the Master thread then prints the output.
void Master_search() { 
    int ary_max = size;
    global_res = true;  
    Barrier(nr_thr);  
    int my_res = true; // local 
    for (int i=0; i<=ary_max && my_res; i++) {
        Global_to_local_res(my_res);
        if (a[i] == target_val){
            my_res = false;
            pthread_mutex_lock(&data_mutex);   
            global_res = i;
            pthread_mutex_unlock(&data_mutex);
        }
    }   
    Report_data(my_res);  
    Barrier(nr_thr); 
    cout << "The target value " << target_val << " was ";

    if(my_res == false)
        cout << "found at index " << global_res << ".\n";
    else
        cout << "not found.\n";
}

// Slave threads to search for a value within an array.
// If the targetted value is found, all threads are updated
// and the search is finished.
void Slave_search(int id) {   
    Barrier(nr_thr);  
    int my_res = true; // locals
    int target = target_val;
    int lim = size;  
    for (int i=0; i<=lim && my_res; i++) {
        Global_to_local_res(my_res);  
       if (a[i] == target_val){
            my_res = false;   
            pthread_mutex_lock(&data_mutex);   
            global_res = i;
            pthread_mutex_unlock(&data_mutex);
        } 
    }
    Report_data(my_res);  
    Barrier(nr_thr);
    
}

// Used to access the global result
void Global_to_local_res(int & my_res) {   
    pthread_mutex_lock(&glob_mutex);
    if (!global_res)      
        my_res = false;     
    pthread_mutex_unlock(&glob_mutex);
}

// Used to coordinate thread reporting
void Report_data(int & my_res) {   
    pthread_mutex_lock(&glob_mutex);
    if (!global_res)      
        my_res = false;     
    pthread_mutex_unlock(&glob_mutex);
}