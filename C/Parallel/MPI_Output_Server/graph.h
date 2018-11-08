/********************************************************************
   File name:     graph.h
  Last updated:  October, 2018.
  Description:   Definition of a class that implements 
                 a directed graph.
*********************************************************************/

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>
#include <string>
using namespace std;

class Graph {
  
public:
    vector<int *> incoming;
    vector<int *> outgoing;
    vector<int> nr_incoming;
    vector<int> nr_outgoing;
    
    int nr_vertices, nr_edges;
    
    // default constructor - makes an empty undirected graph.
    Graph();
    // destructor
    ~Graph();
    
    // Starts the graph over.
    void Make_empty();
    
    // Initializes the edge_list vector based on the number of vertices.
    void Init(int vertexnr);
    
    // Adds an edge between vertices identified by their inde.x
    void Add_edge(const int index1, const int index2);

    // Reads the graph from a file.
    void Read(const char *filename);
    
    void Print();
  
    // Sends to each slave process first the number of outgoing
    // vertices, then the array of vertices.
    void Send_outgoing(int nr_proc);
        
    // Sends to each slave process first the number of incoming
    // vertices, then the array of vertices.
    void Send_incoming(int nr_proc);
};

#endif
