/**********************************************************
 File:          barrier_bcast.cc                        
 Description:   Every process writes hello and its rank 
                Implementing the barrier and broadcast  
        		functions based on a simple send and 
                receive.

 Last update:   October 2018
**********************************************************/

#include <mpi.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
using namespace std;

// Function prototypes.
void Barrier(int no_proc, int my_id);
void Broadcast(int &data, int source, int no_proc, int my_id);
void Write_message(int rank, char *message);
void Final_Print(int my_id, int nr_proc, double time);

// Main function, needs the arguments.
int main(int argc, char **argv)
{
    double time;
    int my_id, nr_proc;
    int n=0;
    const int source_proc=2;
    char cpu_name[80];
    
    /* Initialize MPI */
    MPI_Init (&argc, &argv);
    
    /* Get the rank of this process */
    MPI_Comm_rank (MPI_COMM_WORLD, &my_id);
    
    /* Get nb of processes */
    MPI_Comm_size (MPI_COMM_WORLD, &nr_proc);
    
    time = MPI_Wtime();
    // Testing the send and receive just to make sure they work. You can
    // comment this out if you want. 
    //Test_send_recv(nr_proc, my_id);
    
    // An example of finding out which machine we are running on.
    gethostname(cpu_name,80);
    Write_message(my_id, cpu_name);
    
    // Testing the barrier. The before and after messages should not be
    // mixed on the screen.
    Write_message(my_id, "before barrier");
    Barrier(nr_proc, my_id);
    Write_message(my_id, "after barrier");
    
    // This will be the source process having a different value for the
    // variable n to test the broadcast.
    if (my_id == source_proc)
        n = 5;

    // Testing the broadcast. All values of n should be 5 if the number
    // of processes is greater than the id of the source_proc, 0
    // otherwise.
    Broadcast(n, source_proc, nr_proc, my_id);

    // Set barrier so n-value is printed correctly
    Barrier(nr_proc, my_id);
    cout << "n is " << n << endl;

    // Call so that process 0 is the last to finalize and print
    Final_Print(my_id, nr_proc, time);
    
    /* Finalize MPI */
    MPI_Finalize();
    return 0;
}

// The function that writes the message on the screen. rank is the
// identity of the current process obtained from MPI_Comm_rank.
void Write_message(int rank, char *message)
{
    cout << message << " from process " << rank << endl;
}

// Reimplementation of the barrier function. The nr_proc is the number
// of processes in the program, obtained by calling MPI_Comm_size. 
// my_id is the identity of the current process, obtained with
// MPI_Comm_rank. This function should block all the processes inside
// it until all of them have made a call to it. Then it must release
// all of them.
void Barrier(int nr_proc, int my_id)
{   
    // Generic value for requirement of message function calls, has no actual
    // meaning beyond meeting call requirements
    int local = 0;

    if (my_id != 0 && nr_proc > 1){
        MPI_Status status;

        // Send first to master process 0 to be "counted" as checking
        // into the barrier
        MPI_Send(&local, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        // Now recieve and block until the master process 0 releases
        // all processes after all have reported in
        MPI_Recv(&local, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    if (my_id == 0 && nr_proc > 1){
        MPI_Status status;

        // Wait for all processes to report into the barrier
        for (int i = 1; i < nr_proc; i++)
            MPI_Recv(&local, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);

        // After all processes have reported, now release them
        for (int i = 1; i < nr_proc; i++)
            MPI_Send(&local, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
}

// Reimplementation of the broadcast function with the data being
// transmitted being a simple integer. The first parameter is the
// variable to be broadcast to all the processes. The source is the
// identity of the process who must send the value of the variable to
// all the others. The last two parameters are the same as for the
// previous function. Note: if the identity of the source process is
// greater than nr_proc -1, or less than 0, then the broadcast should
// not be doing anything.
void Broadcast(int &data, int source, int nr_proc, int my_id)
{
    // Error protection if source process value is outside of range
    if (source > nr_proc-1 || source < 0)
        return;

    // Source process sends data value to every process    
    else if (source == my_id){
        for (int i = 0; i < nr_proc; i++)
                MPI_Send(&data, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
    
    // All other processes wait to receive from source process
    else; {
        MPI_Status status;
        MPI_Recv(&data, 1, MPI_INT, source, 0, MPI_COMM_WORLD, &status);
    }
}

// Using token-ring example to force process 0 to be the last
// process for an ordered print. Code altered for use in this program.
// Original obtained from http://mpitutorial.com/tutorials/mpi-send-and-receive/
// Source program exists at:
// https://github.com/wesleykendall/mpitutorial/blob/gh-pages/tutorials/mpi-send-and-receive/code/ring.c
// made available for use from the author's github under the MIT license
void Final_Print(int my_id, int nr_proc, double time){
    int token;

    // Receive from the lower process and send to the higher process. Take care
    // of the special case when you are the first process to prevent deadlock.
    if (my_id != 0) {
        MPI_Recv(&token, 1, MPI_INT, my_id - 1, 0,
             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } 
    
    else
    // Set the token's value if you are process 0
        token = -1;
        
    MPI_Send(&token, 1, MPI_INT, (my_id + 1) % nr_proc,
             0, MPI_COMM_WORLD);

    // Now process 0 can receive from the last process. This makes sure that at
    // least one MPI_Send is initialized before all MPI_Recvs (again, to prevent
    // deadlock)
    if (my_id == 0) {
        MPI_Recv(&token, 1, MPI_INT, nr_proc - 1, 0,
             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
             time = MPI_Wtime() - time;
        cout << "This program has run on " << nr_proc
             << " processes in " << time << " seconds." << endl;
    }
}