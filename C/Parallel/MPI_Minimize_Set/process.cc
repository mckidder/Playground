/********************************************************************
  File name:     process.cc
  Last updated:  November 2018.
  Description:   A program that generates an ideal playlist. 
                 Functions for the master and slave processes.
*********************************************************************/

#include <iostream>
using namespace std;
#include <mpi.h>
#include "process.h"

// Creates a custom MPI data type to be able to send and receive the
// struct Song and commits it so that we can use it.
void create_data_type(MPI_Datatype &dt_song)
{
    int          blocklengths[5] = {1,1,1,1,1};
    MPI_Datatype types[5] = {MPI_INT, MPI_INT, MPI_INT, 
                             MPI_FLOAT, MPI_FLOAT};
    MPI_Aint     offsets[5];

    offsets[0] = offsetof(Song, title1);
    offsets[1] = offsetof(Song, title2);
    offsets[2] = offsetof(Song, title3);
    offsets[3] = offsetof(Song, att1);
    offsets[4] = offsetof(Song, att2);

    MPI_Type_create_struct(5, blocklengths, offsets, types, &dt_song);
    MPI_Type_commit(&dt_song);
}

// Master function. Generates the songs, send the information to the
// slaves, generates its own subset of permutations and finds the
// optimal one, then collects the results from the slaves.
void Master(int nr_proc)
{
    MPI_Datatype mpi_song;
    int nr_songs;
    Song *songs;
    int *perm, *min_shuffle;
    float min_dist = -1;
    
    create_data_type(mpi_song);

    cout << "Enter the number of songs" << endl;
    cin >> nr_songs;
    songs = new Song[nr_songs];
    perm = new int[nr_songs];
    min_shuffle = new int[nr_songs];
    generate_songs(songs, nr_songs);

    // Broadcast the songs list and number of songs to slaves
    MPI_Bcast(&nr_songs, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(songs, nr_songs, mpi_song, 0, MPI_COMM_WORLD);

    // Perform first permutation for master process
    first_permutation(perm, nr_songs, 0);
    cout << "Original list of songs: ";
    output_songs(perm, songs, nr_songs);
    
    // Set master minimum distance
    min_dist = generate_permutations(perm, min_shuffle, songs, nr_songs);
    
    // Use master results as init and then compare to what the
    // slaves have come up with.
    collect_results(min_shuffle, min_dist, nr_songs, nr_proc);

    // Output identified min shuffle and what the permutation
    // looked like that it was found in
    output_permutation(min_shuffle, nr_songs);
    output_songs(min_shuffle, songs, nr_songs);

    // free the data type once we're done using it. 
    MPI_Type_free(&mpi_song);
}

// Slave function. It should first call the function creating the data
// structure that can be used to receive data of type Song. Then it
// should receive the number of songs and the array of songs from the
// master. Then it should generate the initial permutation based on
// the id. Then it should call the function generate_permutations in a
// similar way to the master. At the end it should send the minimal
// shuffle to the master.
void Slave(int id) 
{
    MPI_Datatype mpi_song;
    create_data_type(mpi_song);
    int nr_songs, tag=0;
    float min_dist;
    int *min_shuffle, *perm;
    Song *songs;

    // Get broadcast from master for the number of songs
    MPI_Bcast(&nr_songs, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Create songs structure to receive songs from master
    songs = new Song[nr_songs];

    // Get broadcast of songs from master
    MPI_Bcast(songs, nr_songs, mpi_song, 0, MPI_COMM_WORLD);

    // Run initial permutation and alloc storage arrays
    perm = new int[nr_songs];
    min_shuffle = new int[nr_songs];
    first_permutation(perm, nr_songs, id);

    // Find the min total distance for all permutations that this
    // slave process is responsible for
    min_dist = generate_permutations(perm, min_shuffle, songs, nr_songs);

    // Send found min shuffle and min distance to master process
    MPI_Send(min_shuffle, nr_songs, MPI_INT, 0, tag, MPI_COMM_WORLD);
    MPI_Send(&min_dist, 1, MPI_FLOAT, 0, tag, MPI_COMM_WORLD);
}

// Function generating the next permutation in lexicographic order.
void next_permutation(int a[], int n) 
{
    int i, j, k, m;
    for (k=n-2; k>=0; k--) 
        if (a[k] < a[k+1]) 
            break;
    if (k >= 0) {
        for (m=n-1; a[m]<a[k]; m--);
        swap(a[k], a[m]);
    }
    for(i=k+1, j=n-1; i<j; i++, j--)
        swap(a[i], a[j]);
}

// Generates the initial permutation in the subgroup given by id.
void first_permutation(int a[], int n, int id) 
{
    int i;
    for (i=0; i<n; i++)
        a[i] = i;
    int temp = a[id];
    for (i=id-1; i>=0; i--)
        a[i+1] = a[i];
    a[0] = temp;
}

// Generates the last permutation, meaning an array sorted backwards.
void last_permutation(int a[], int n) 
{
    int i;
    for (i=0; i<n; i++)
        a[i] = n-i-1;
}

// Copies the array b into the array a.
void copy_array(int a[], int b[], int n)
{
    for (int i=0; i<n; i++)
        a[i] = b[i];
}

// Outputs the array.
void output_permutation(int a[], int n)
{
    for (int i=0; i<n; i++)
        cout << a[i] << ' ';
    cout << endl;
}

// Outputs the given shuffle as a list of songs
void output_songs(int shuffle[], Song *songs, int nr_songs)
{
    cout << "Here is the shuffle:" << endl;
    for (int i=0; i<nr_songs; i++) 
        output_song(songs[shuffle[i]]);
}

// Generate all the permutations allocated to this process. The
// function should evaluate the distance in the starting permutation
// and store it in a local variable. Then it should generate new
// permutations until the first element (a[0]) has changed. For each
// of them, it should compare the sum of the distances between songs
// to the stored one, and if the new one is smaller, copy the
// permutation into the mina array. At the end it should also return
// the minimal distance.
float generate_permutations(int a[], int mina[], Song *songs, int n)
{
    float curr_d, min_d;
    // Set initial a[0] value and initialize current a[0] with it
    int init_a = a[0];
    int curr_a = init_a;

    // Set inital distances and array
    min_d = distance_songs(a, songs, n);
    copy_array(mina, a, n);
    
    // Run until the current value of a[0] is no longer equal to the init
    // which will indicate that a[0] has been changed
    while(curr_a == init_a){
        // Generate the next permutation
        next_permutation(a, n);

        // Get the current distance for this permuation
        curr_d = distance_songs(a, songs, n);

        // If the distance for this permutation is smaller than the
        // previous smallest then update the new min distance and array
        if(curr_d < min_d){
            min_d = curr_d;
            // Also update min array for min distance permutation
            copy_array(mina, a, n);
        }

        // Update current value of a[0]
        curr_a = a[0];
    }

    return min_d;
  
}

// This function should receive the minimum permutation and the
// distance from each slave and compute the minimum of shuffle of all
// the received ones. A local array will be needed to receive the
// permutations.
void collect_results(int min_shuffle[], float &min_distance, int nr_songs, 
                     int nr_proc)
{
    int *min_perm = new int[nr_songs];
    int tag=0;
    float min_total;

    MPI_Status status;
    MPI_Datatype mpi_song;

    create_data_type(mpi_song);

    // Collect results from slave processes for the min permutation and distance
    // Master is only process to call this function therefore, its values are 
    // initially stored in min_shuffle[] and min_distance and each slave
    // process will compare their results to the masters
    for(int i = 1; i < nr_proc; i++){

        MPI_Recv(min_perm, nr_songs, MPI_INT, i, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&min_total, 1, MPI_FLOAT, i, tag, MPI_COMM_WORLD, &status);

        // New min found, update min array and min distance
        if (min_total < min_distance){
            min_distance = min_total;
            copy_array(min_shuffle, min_perm, nr_songs);
        }
    }
}

// Computes the distance between songs in a playlist
float distance_songs(int *playlist, Song *songs, int nr_songs)
{
    float dist_total=0.0;

    // Find the total sum of distances for the songs within the playlist
    for (int i=0; i<nr_songs-1; i++)
        // Sum distances for current array index and the next (unless last index) 
        dist_total = dist_total + distance_song(songs[playlist[i]], songs[playlist[i+1]]);

    return dist_total;
}
