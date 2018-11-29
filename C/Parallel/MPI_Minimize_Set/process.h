/********************************************************************
  File name:     process.h
  Last updated:  November 2018.
  Description:   A program that generates an ideal playlist. 
                 Functions for the master and slave processes.
*********************************************************************/

#ifndef PROCESS_H
#define PROCESS_H

#include "songs.h"

// Creates a custom MPI data type to be able to send and receive the
// struct Song and commits it so that we can use it.
void create_data_type(MPI_Datatype &dt_song);

// Master function. Generates the songs, send the information to the
// slaves, generates its own subset of permutations and finds the
// optimal one, then collects the results from the slaves.
void Master(int nr_proc);

// Slave function. It should receive the number of songs and their
// attribute arrays from the master. Then it should generate the
// initial permutation based the id. Then it should call the function
// generate permutations in a similar way to the master. At the end it
// should send the minimal shuffle to the master.
void Slave(int id);

// Function generating the next permutation in lexicographic order.
void next_permutation(int a[], int n);

// Generates the initial permutation in the subgroup given by id.
void first_permutation(int a[], int n, int id);

// Generates the last permutation, meaning an array sorted backwards.
void last_permutation(int a[], int n);

// Copies the array b into the array a.
void copy_array(int a[], int b[], int n);

// Outputs the array.
void output_permutation(int a[], int n);

// Outputs the given shuffle as a list of songs
void output_songs(int shuffle[], Song *songs, int nr_songs);

// Generate all the permutations allocated to this process. The
// function should evaluate the distance in the starting permutation
// and store it in a local variable. Then it should generate new
// permutations until the first element (a[0]) has changed. For each
// of them, it should compare the sum of the distances between songs
// to the stored one, and if the new one is smaller, copy the
// permutation into the mina array. At the end it should also return
// the minimal distance.
float generate_permutations(int a[], int mina[], Song *songs, int n);

// This function should receive the minimum permutation and the
// distance from each slave and compute the minimum of shuffle of all
// the received ones. A local array will be needed to receive the
// permutations.
void collect_results(int min_shuffle[], float &min_distance, int nr_songs, 
                     int nr_proc);

// Computes the distance between songs in a playlist
float distance_songs(int *playlist, Song *songs, int nr_songs);

#endif
