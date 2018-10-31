/********************************************************************
  File name:     process_graph.cc
  Last updated:  October, 2018.
  Description:   The master - slave functions related to the 
                 implementation of the dependency graph problem 
                 in parallel using MPI.
*********************************************************************/

#include "graph.h"
#include "process_graph.h"
#include <mpi.h>
#include <sys/time.h>
#include <iostream>
using namespace std;
#include <cstdlib>

// Master process: input the graph, sends the information to the
// slaves, then calls the dependency function for vertex 0.
void Master(int nr_proc) 
{

    char filename[80];
    Graph gr;
    
    cout << "Enter the graph filename" << endl;
    cin>> filename;
    gr.Read(filename);
    gr.Print();
    if (nr_proc < gr.nr_vertices) {
        cout << "Cannot run the problem on this graph with less than " 
             << gr.nr_vertices << " processes" << endl;
        exit(0);
    }
    cout << endl;
    gr.Send_outgoing(nr_proc);
    gr.Send_incoming(nr_proc);
    Process_dependency(0, gr.nr_outgoing[0], gr.outgoing[0], 
                       gr.nr_incoming[0], gr.incoming[0]);
}

// Slave process: receives the incoming and outgoing arrays from the
// master and calls the dependency resolution function.
void Slave(int id) 
{
    int nr_in, nr_out, *incoming, *outgoing, tag=0, src=0;
    MPI_Status status;
    MPI_Recv(&nr_out, 1, MPI_INT, src, tag, MPI_COMM_WORLD, &status);
    if (nr_out != 0) {
        outgoing = new int[nr_out];
        MPI_Recv(outgoing, nr_out, MPI_INT, src, tag, MPI_COMM_WORLD, &status);
    }
    MPI_Recv(&nr_in, 1, MPI_INT, src, tag, MPI_COMM_WORLD, &status);
    if (nr_in != 0) {
        incoming = new int[nr_in];
        MPI_Recv(incoming, nr_in, MPI_INT, src, tag, MPI_COMM_WORLD, &status);
    }
    if (nr_in || nr_out) 
        Process_dependency(id, nr_out, outgoing, nr_in, incoming);
}

// The process should first receive a message from all the processes
// in its incoming list, then output a message containing the id, then
// send a message to all the vertices in the outgoing dependency list.
void Process_dependency(int id, int nr_out, int *outgoing, 
                        int nr_in, int *incoming)
{
    int tag=0, local=0;
    MPI_Status status;

    // Receive from incoming list array, first incoming vertex is at
    // index 0. To verify correct order, printing what messages have
    // been received.
    for (int i = 0; i < nr_in; i++){
        MPI_Recv(&local, 1, MPI_INT, incoming[i], tag, MPI_COMM_WORLD, &status);
        cout << "Process id " << id << " received from " << incoming[i] << " at " << Time_measure() << ".\n";
    }
    
    // If vertex has no outgoing edges, it has has been visited.
    if (nr_out == 0) 
        cout << "Process id " << id << " VISITED at " << Time_measure() << ".\n\n";

    // Send to outgoing list array, first outgoing vertex is at
    // index 0. To verify correct order, printing what messages have
    // been sent. If the process is done sending, it has been visited.
    for (int i = 0; i < nr_out; i++){
        MPI_Send(&local, 1, MPI_INT, outgoing[i], tag, MPI_COMM_WORLD);
        cout << "Process id " << id << " sent to " << outgoing[i] << " at " << Time_measure() << ".\n";
        if (i == nr_out-1)
            cout << "Process id " << id << " VISITED at " << Time_measure() << ".\n\n";
    }
}

// Returns the time in miliseconds
int Time_measure()
{
    struct timeval now;

    gettimeofday(&now, 0);
    return now.tv_sec*1000*1000 + now.tv_usec;
}