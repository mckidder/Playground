/**************************************************************
  File:          main.cc
  Description:   A program that implements a barrier using a
                 conditional variable from the pthread library.

***************************************************************/

#include <cstdlib>
#include <pthread.h>
#include <ctime>
#include <iostream>
using namespace std;
#include "ride_barrier.h"

int main()
{
    int no_threads, car_capac;
    pthread_t *threads;
    srand(time(NULL));
    Init_data(no_threads, car_capac);
    threads = new pthread_t[no_threads];
    Create_threads(threads, no_threads);
    Synchronise(threads, no_threads);
    cout << "Ten rides have been given, time for a break.\n" 
         << "******************************************************\n";
    return 0;
}

/***************** Example of output *************************
Enter the total number of riders
25
Enter the maximum capacity of the coaster car so that the
number of riders is a multiple of the maximum capacity
5
This ride only runs ten times before it needs a break!
******************************************************
The first ride is now filling up.

Rider 0 is ready to ride!
Rider 1 is ready to ride!
Rider 2 is ready to ride!
Rider 3 is ready to ride!
Rider 4 is ready to ride!

Coaster car is full and riders are going around!
There are 9 rides remaining.
The ride is ready fill up again!

Rider 5 is ready to ride!
Rider 6 is ready to ride!
Rider 7 is ready to ride!
Rider 8 is ready to ride!
Rider 9 is ready to ride!

Coaster car is full and riders are going around!
There are 8 rides remaining.
The ride is ready fill up again!

Rider 11 is ready to ride!
Rider 12 is ready to ride!
Rider 13 is ready to ride!
Rider 14 is ready to ride!
Rider 15 is ready to ride!

Coaster car is full and riders are going around!
There are 7 rides remaining.
The ride is ready fill up again!

Rider 10 is ready to ride!
Rider 16 is ready to ride!
Rider 17 is ready to ride!
Rider 18 is ready to ride!
Rider 19 is ready to ride!

Coaster car is full and riders are going around!
There are 6 rides remaining.
The ride is ready fill up again!

Rider 20 is ready to ride!
Rider 21 is ready to ride!
Rider 23 is ready to ride!
Rider 22 is ready to ride!
Rider 24 is ready to ride!

Coaster car is full and riders are going around!
There are 5 rides remaining.
The ride is ready fill up again!

Rider 0 is ready to ride!
Rider 1 is ready to ride!
Rider 2 is ready to ride!
Rider 7 is ready to ride!
Rider 5 is ready to ride!

Coaster car is full and riders are going around!
There are 4 rides remaining.
The ride is ready fill up again!

Rider 15 is ready to ride!
Rider 19 is ready to ride!
Rider 11 is ready to ride!
Rider 9 is ready to ride!
Rider 6 is ready to ride!

Coaster car is full and riders are going around!
There are 3 rides remaining.
The ride is ready fill up again!

Rider 16 is ready to ride!
Rider 3 is ready to ride!
Rider 13 is ready to ride!
Rider 4 is ready to ride!
Rider 12 is ready to ride!

Coaster car is full and riders are going around!
There are 2 rides remaining.
The ride is ready fill up again!

Rider 10 is ready to ride!
Rider 17 is ready to ride!
Rider 8 is ready to ride!
Rider 14 is ready to ride!
Rider 18 is ready to ride!

Coaster car is full and riders are going around!
There are 1 rides remaining.
The ride is ready fill up again!

Rider 24 is ready to ride!
Rider 23 is ready to ride!
Rider 21 is ready to ride!
Rider 22 is ready to ride!
Rider 20 is ready to ride!

Coaster car is full and riders are going around!
There are 0 rides remaining.
Ten rides have been given, time for a break.
******************************************************
**************************************************************/
