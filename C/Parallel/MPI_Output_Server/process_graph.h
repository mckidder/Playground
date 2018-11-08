/********************************************************************
  File name:     process_graph.h
  Last updated:  October, 2018.
  Description:   The master - slave functions related to the 
                 implementation of the dependency graph problem 
                 in parallel using MPI.
*********************************************************************/

#ifndef PROCESS_GRAPH_H
#define PROCESS_GRAPH_H

// Master process: input the graph, sends the information to the
// slaves, then calls the dependency function for vertex 0.
void Master(int nr_proc);

// Slave process: receives the incoming and outgoing arrays from the
// master and calls the dependency resolution function.
void Slave(int id, int nr_proc);

// The process should first receive a message from all the processes
// in its incoming list, then output a message containing the id, then
// send a message to all the vertices in the outgoing dependency list.
void Process_dependency(int id, int nr_out, int *outgoing, 
                        int nr_in, int *incoming, int nr_proc);
// Output server that will wait for a first receive from the master process (0)
// so that it knows the total number of vertices. The server will then create an
// output array that will store the correct order of each process as the index
// number of the output array. After the server has received from all vertices,
// a simple print of the output array is performed.
void Output_server();

#endif
