/************************************************************************
  File:          worker.cc                             
  Description:   Functions for the workers.
                                                         
  Author:        Dana Vrajitoru, IUSB 
  Course:        B424 B524 Parallel Programming                        
  Date:          November 2018
************************************************************************/

#include <iostream>
using namespace std;
#include <mpi.h>
#include "worker.h"
#include "common_decl.h"
#include "puzzle.h"

// Loops over waiting for a task (a word to search for), executing it,
// and sending the result to the master, until it receives a task from
// the master indicating that the job is finished (an empty string).
void Worker(int proc_id)
{
    static MPI_Status status;
    MPI_Request request;
    Puzzle the_puzzle;
    char word[WORDSIZE];
    int flag = 0;
    
    the_puzzle.Broadcast(MASTER_ID, proc_id);
    
    Send_empty_result();
    
    MPI_Irecv(word, WORDSIZE, MPI_CHAR, MASTER_ID, 0, MPI_COMM_WORLD, &request);
    do { 
        MPI_Test(&request, &flag, &status);
        if (flag) {
            if (word[0] != '\0') 
                Process_word(word, the_puzzle);
            MPI_Irecv(word, WORDSIZE, MPI_CHAR, MASTER_ID, 0, MPI_COMM_WORLD, &request);
        }
    } while (!flag || word[0] != '\0');
}

// Send the master 5 integer values, all being 0. This will cause the
// master to ignore the data. 
void Send_empty_result()
{
    int value = 0;

    for(int i = 0; i < 5; i++){
        MPI_Send(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
}

// Find the word in the puzzle, then send the master 5 integer values:
// the size of the word, the position where it was found, and the
// direction in which it was found. If the word is not found in the
// puzzle, then the worker must send the master an empty result by
// calling the function defined earlier in this file.
void Process_word(char *word, Puzzle &the_puzzle)
{
    int size, posr, posc, rdir, cdir, src, rows, cols;
    int tag=0;
    bool found = false;
    MPI_Status status;
    
    size = strlen(word);
    rows = the_puzzle.Rows();
    cols = the_puzzle.Cols();

    for (int i=0; i<rows; i++) {
        posr = i;
        for (int j=0; j< cols; j++)
            posc = j;
            if (the_puzzle.Find_word(word, posr, posc, rdir, cdir)){
                MPI_Send(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                MPI_Send(&posr, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                MPI_Send(&posc, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                MPI_Send(&rdir, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                MPI_Send(&cdir, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                found = true;
            }

    }
    if (!found)
        Send_empty_result();        

}
