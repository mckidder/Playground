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
#include <iostream>
using namespace std;
#include <cstdlib>

// Master process: input the graph, sends the information to the
// slaves, then calls the dependency function for vertex 0.
void Master(int nr_proc) 
{

    char filename[80];
    int nr_verts;
    int tag=0;
    Graph gr;
    cout << "Enter the graph filename" << endl;
    cin>> filename;
    gr.Read(filename);
    nr_verts = gr.nr_vertices;
    // Send the amount of processes to the output server
    MPI_Send(&nr_verts, 1, MPI_INT, nr_proc-1, tag, MPI_COMM_WORLD);
    gr.Print();
    if (nr_proc < gr.nr_vertices) {
        cout << "Cannot run the problem on this graph with less than " 
             << gr.nr_vertices << " processes" << endl;
        exit(0);
    }
     if (nr_proc < gr.nr_vertices+1) {
        cout << "Cannot run the problem and output server on this graph with less than " 
             << gr.nr_vertices+1 << " processes. Rerun  with a number of processes equal " 
             << "to at least 1 plus the number of vertices in the graph." << endl;
        exit(0);
    }
    cout << endl;
    gr.Send_outgoing(nr_proc);
    gr.Send_incoming(nr_proc);
    Process_dependency(0, gr.nr_outgoing[0], gr.outgoing[0], 
                       gr.nr_incoming[0], gr.incoming[0], nr_proc);
}

// Slave process: receives the incoming and outgoing arrays from the
// master and calls the dependency resolution function.
void Slave(int id, int nr_proc) 
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
        Process_dependency(id, nr_out, outgoing, nr_in, incoming, nr_proc);
}

// The process should first receive a message from all the processes
// in its incoming list, then output a message containing the id, then
// send a message to all the vertices in the outgoing dependency list.
void Process_dependency(int id, int nr_out, int *outgoing, 
                        int nr_in, int *incoming, int nr_proc)
{
    int tag=0, local=0, proc_id;
    MPI_Status status;
    proc_id = id;
    // Receive from incoming list array, first incoming vertex is at
    // index 0. To verify correct order, printing what messages have
    // been received.
    for (int i = 0; i < nr_in; i++){
        MPI_Recv(&local, 1, MPI_INT, incoming[i], tag, MPI_COMM_WORLD, &status);
    }
    

    MPI_Send(&proc_id, 1, MPI_INT, nr_proc-1, tag, MPI_COMM_WORLD);
    // Send to outgoing list array, first outgoing vertex is at
    // index 0. To verify correct order, printing what messages have
    // been sent. If the process is done sending, it has been visited.
    for (int i = 0; i < nr_out; i++){
        MPI_Send(&local, 1, MPI_INT, outgoing[i], tag, MPI_COMM_WORLD);
    }
}

// Output server that will wait for a first receive from the master process (0)
// so that it knows the total number of vertices. The server will then create an
// output array that will store the correct order of each process as the index
// number of the output array. After the server has received from all vertices,
// a simple print of the output array is performed.
void Output_server(){
    int tag=0, nr_verts=0, datum;
    int *arry;

    MPI_Status status;

    MPI_Recv(&nr_verts, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

    // Create output array based upon the number of vertices
    arry = new int[nr_verts];

    // Receive from all processes until all vertices have reported in.
    // Store correct order (sorted by array index number) in output array
    // with the vertice number.
    for(int i = 0; i < nr_verts; i++){
        MPI_Recv(&datum, 1, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
        arry[i] = datum;
    }

    // Simple print of output array.
    for(int i =0; i < nr_verts; i++)
        cout << arry[i] << " ";
        
    // Add final endl and deallocate output array. 
    cout << endl;
    delete[] arry; 
}