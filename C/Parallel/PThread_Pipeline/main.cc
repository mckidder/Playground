/**************************************************************
  File:          main.cc
  Description:   A program that implements a sorting function
                 for an array using a pipeline model. 

***************************************************************/

#include <cstdlib>
#include <pthread.h>
#include <ctime>
#include <iostream>
using namespace std;
#include "sort_pipeline.h"

int main() 
{
    int nthreads;
    pthread_t *threads;
    srand(time(NULL));
    Init_data(nthreads);
    threads = new pthread_t[nthreads];
    Create_threads(threads, nthreads);
    Synchronise(threads, nthreads);
    return 0;
}
