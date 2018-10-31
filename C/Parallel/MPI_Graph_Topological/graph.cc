/********************************************************************
  File name:     graph.cc
  Last updated:  October, 2018.
  Description:   Implementation of a class defining a directed graph.
*********************************************************************/

#include "graph.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <mpi.h>

// Default constructor - makes an empty undirected graph.
Graph::Graph()
{
    nr_vertices = 0;
    nr_edges = 0;
}

// Destructor - empty the lists and the vector.
Graph::~Graph()
{
    Make_empty();
}

// Starts the graph over.
void Graph::Make_empty()
{
    if (nr_vertices) {
        for (int i=0; i<nr_vertices; i++) {
            delete [] incoming[i];
            delete [] outgoing[i];
        }
        incoming.clear();
        outgoing.clear();
        nr_incoming.clear();
        nr_outgoing.clear();
        nr_vertices = 0;
        nr_edges = 0;
    }
}

// Initializes the edge_list vector based on the number of vertices.
void Graph::Init(int vertexnr)
{
    nr_vertices = vertexnr;
    int *arr;
    for (int i=0; i<vertexnr; i++) {
        arr = new int[nr_vertices];
        incoming.push_back(arr);
        nr_incoming.push_back(0);
        arr = new int[nr_vertices];
        outgoing.push_back(arr);
        nr_outgoing.push_back(0);
    }
}

// Adds an edge from index1 towards index2.
void Graph::Add_edge(const int index1, const int index2)
{
    if (index1 < nr_vertices && index2 < nr_vertices) {
        // Add index2 as an outgoing edge for index1:
        outgoing[index1][nr_outgoing[index1]] = index2;
        nr_outgoing[index1]++;
        
        // Add index1 as an incoming for index2
        incoming[index2][nr_incoming[index2]] = index1;
        nr_incoming[index2]++;
        
        nr_edges++;
    }
    else
        cout << "Attempting to add an edge for a nonexisting vertex." << endl;
}

// Reads the graph from a file
void Graph::Read(const char *filename)
{
    ifstream fin(filename);
    int nrv, v1 = -1, v2 = -1;    
    fin >> nrv;
    Init(nrv);
    while (fin.good() && !fin.eof()) {
        fin >> v1 >> v2;
        if (fin.good())
            Add_edge(v1, v2);
    }
    fin.close();
}

// Simple print of the graph.
void Graph::Print()
{
    int i, j;
    cout << "The graph contains " << nr_vertices
         << " vertices and " << nr_edges << " edges" << endl;
    if (nr_vertices) {
        cout << "The outgoing edges for each vertex are:" << endl;
        for (i=0; i<nr_vertices; i++) {
            cout << "vertex " << i << ": ";
            for (j=0; j<nr_outgoing[i]; j++) 
                cout << outgoing[i][j] << ' ';
            cout << endl;
        }
        cout << "The incoming edges for each vertex are:" << endl;
        for (i=0; i<nr_vertices; i++) {
            cout << "vertex " << i << ": ";
            for (j=0; j<nr_incoming[i]; j++) 
                cout << incoming[i][j] << ' ';
            cout << endl;
        }
    }
                  
}

// Sends to each slave process first the number of outgoing
// vertices, then the array of vertices.
void Graph::Send_outgoing(int nr_proc) 
{
    int i, tag = 0;
    for (i=1; i<nr_proc; i++) 
        if (i < nr_vertices) {
            // Send the count of outgoing vertices first
            MPI_Send(&nr_outgoing[i], 1, MPI_INT, i, tag, MPI_COMM_WORLD);
            // Then send the array of outgoing vertices for i. Note that
            // outgoing[i] is already a pointer
            if (nr_outgoing[i] > 0)
                MPI_Send(outgoing[i], nr_outgoing[i], MPI_INT, i, tag, MPI_COMM_WORLD);
        }
        else 
            MPI_Send(&tag, 1, MPI_INT, i, tag, MPI_COMM_WORLD);


}

// Sends to each slave process first the number of incoming vertices,
// then the array of vertices.
void Graph::Send_incoming(int nr_proc)
{
     int i, tag = 0;
    for (i=1; i<nr_proc; i++) 
        if (i < nr_vertices) {
            // Send the count of incoming vertices first
            MPI_Send(&nr_incoming[i], 1, MPI_INT, i, tag, MPI_COMM_WORLD);
            // Then send the array of incoming vertices for i. Note that
            // incoming[i] is already a pointer
            if (nr_incoming[i] > 0)
                MPI_Send(incoming[i], nr_incoming[i], MPI_INT, i, tag, MPI_COMM_WORLD);              
        }
        else 
            MPI_Send(&tag, 1, MPI_INT, i, tag, MPI_COMM_WORLD);    
}
