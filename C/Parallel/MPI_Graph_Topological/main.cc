/********************************************************************
  File name:     main.cc
  Last updated:  October, 2018.
  Description:   A program that simulates a dependancy graph 
                 for a set of processes.
*********************************************************************/

#include <mpi.h>
#include <cstdlib>
#include <iostream>
using namespace std;
#include "process_graph.h"

int main(int argc, char **argv)
{

    int my_id, no_proc;
    int n = 0;
    const int source_proc = 2;

    /* Initialize MPI */
    MPI_Init (&argc, &argv);
    
    /* Get the rank of this process */
    MPI_Comm_rank (MPI_COMM_WORLD, &my_id);
    
    /* Get nb of processes */
    MPI_Comm_size (MPI_COMM_WORLD, &no_proc);
    
    if (my_id == 0) 
        Master(no_proc);
    else
        Slave(my_id);

    /* Finalize MPI */
    MPI_Finalize ();
    return 0;

}
