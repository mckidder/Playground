/********************************************************************
 Description:   Testing the graph class.
 *********************************************************************/

#include "graph.h"
#include <iostream>
using namespace std;

/********************************************************************
 Prototypes
 *********************************************************************/

// Build the graph 
void makeGraph1(Graph &myGraph);

// The main loop of the program.
void mainLoop(Graph &myGraph);
// Prints the menu options.
void printMenu();
// Executes the menu options.
bool executeMenu(Graph &myGraph, char option);

/********************************************************************
 Functions
 *********************************************************************/

int main()
{
    Graph aGraph;
    makeGraph1(aGraph);
    aGraph.print();
    aGraph.makeEmpty();
    aGraph.read("graph1.txt");
    aGraph.print();
    mainLoop(aGraph);
    return 0;
} // main()

// Build the graph
void makeGraph1(Graph &myGraph)
{
    myGraph.init(5);
    myGraph.addEdge("A", "B");
    myGraph.addEdge("A", "D");
    myGraph.addEdge("A", "E");
    myGraph.addEdge("B", "C");
    myGraph.addEdge("B", "D");
    myGraph.addEdge("C", "D");
    myGraph.addEdge("C", "E");
} // makeGraph1()

// The main loop of the program.
void mainLoop(Graph &myGraph)
{
    bool quit=false;
    char answer;
    while (!quit) {
        printMenu();
        cin >> answer;
        cout << endl << endl;
        quit = executeMenu(myGraph, answer);
        cout << endl;
    }
} // mainLoop()

// Prints the menu options.
void printMenu()
{
    cout << "You can perform the following actions:" << endl
    << "R : read the graph from a file" << endl
    << "P : print the graph" << endl
    << "D : print the graph in depth-first order" << endl
    << "E : empty the graph" << endl
    << "W : write a graph to a file" << endl
    << "I : input a graph manually" << endl
    << "Q : quit" << endl
    << endl << "Enter your choice: ";
} // printMenu()

// Executes the menu options.
bool executeMenu(Graph &myGraph, char option)
{
    char buffer[80];
    int index;
    switch (option) {
        case 'r':
        case 'R':
            cout << "Enter the input file name" << endl;
            cin >> buffer;
            if (!myGraph.read(buffer))
                cout << "Could not read the graph from the file" << endl;
            break;
        case 'p':
        case 'P':
            myGraph.print();
            break;
        case 'd':
        case 'D':
            cout << "Enter the index or the origin vertex" << endl;
            cin >> index;
            if (!cin)
            {
                cout << "\nInvalid input, enter an integer value only\n";
                cin.clear();
                cin.ignore(1024, '\n');
                break;
            }
            myGraph.printDf(index);
            break;
        case 'e':
        case 'E':
            myGraph.makeEmpty();
            break;
        case 'w':
        case 'W':
            cout << "Enter a file name to write" << endl;
            cin >> buffer;
            myGraph.write(buffer);
            break;
        case 'i':
        case 'I':
            myGraph.input();
            break;
        case 'q':
        case 'Q':
            return true;
        default:
            cout << "Unknown option, try again" << endl;
    }
    return false;
} // executeMenu()



/********************************************************************
 Description:   Implementation of the graph class.
 *********************************************************************/

#include "graph.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstring>

// A sort of hash function for the names - works only if the names are
// in sequence, all of them starting with an uppercase, and none of
// them having the same starting letter. Like, 'A', 'B', 'C', etc.
int Graph::index(const string name)
{
    return int(name[0] - 'A');
} // Graph::index()

// Default constructor - makes an empty undirected graph.
Graph::Graph(int nv)
{
    numVertices = 0;
    numEdges = 0;
    directed = false;
    if (nv>0)
        init(nv);
    else
        int *mark = new int [numVertices];
} // Graph::Graph()

// Destructor - empty the lists and the vector.
Graph::~Graph()
{
    makeEmpty();
} // Graph::~Graph()

// Starts the graph over.
void Graph::makeEmpty()
{
    directed = false;
    if (numVertices)
    {
        for (int i=0; i<numVertices; i++)
            vertices[i].edgeList.clear();
        vertices.erase(vertices.begin(), vertices.end());
        numVertices = 0;
        numEdges = 0;
        delete [] mark;
    }
} // Graph::makeEmpty()

// Tells us if the graph is directed.
bool Graph::isDirected()
{
    return directed;
} // Graph::isDirected()

// Set the direction flag to directed or undirected.
void Graph::setDirected()
{
    directed = true;
} // Graph::setDirected()

void Graph::setUndirected()
{
    directed = false;
} // Graph::setUndirected()

// Initialize the data structure for the specified number of vertices.
void Graph::init(int nv)
{
    if (numVertices > 0) // is there something in the graph?
        makeEmpty();     // the clear the data
    int *mark = new int [nv]; // dynamic creation mark array
    for (int i = 0; i < nv; i++)// init to false
        mark[i] = 0;
    addVertex(nv-1);     // Last index we need
} // Graph::init()

// Adds a vertex with the specified ind_nex. It also adds all the
// intermediate vertices from the current position to the new one. It
// creates labels with uppercase letters in order.
void Graph::addVertex(int index)
{
    string s = "A";
    for(int i = numVertices; i<=index; i++)
    {
        mark[int('A' + i)] = 0;
        s[0] = char('A' + i);
        addVertex(s);
    }
} // Graph::addVertex()

// Adds a vertex with a given name.
void Graph::addVertex(const string theName)
{
    vertex newVertex;
    newVertex.name = theName;
    vertices.push_back(newVertex);
    numVertices++;
}

// Adds an edge based on vertex names. If the graph is undirected,
// it adds the edge in both directions.
void Graph::addEdge(const char *name1, const char *name2)
{
    int index1 = index(string(name1));
    int index2 = index(string(name2));
    if (index1 > numVertices)
        addVertex(index1);
    if (index2 >= numVertices)
        addVertex(index2);
    addEdge(index1, index2);
} // Graph::addEdge()

// Adds an edge based on vertex names. If the graph is undirected,
// it adds the edge in both directions.
void Graph::addEdge(const string name1, const string name2)
{
    int index1 = index(name1);
    int index2 = index(name2);
    if (index1 >= numVertices)
        addVertex(name1);
    if (index2 >= numVertices)
        addVertex(name2);
    addEdge(index1, index2);
} // Graph::addEdge()

// Adds an edge between vertices identified by their index
void Graph::addEdge(const int index1, const int index2)
{
    vertices[index1].edgeList.push_back(index2);
    if (!directed)
        vertices[index2].edgeList.push_back(index1);
    numEdges++;
} // Graph::addEdge()

// Reads the graph from a file
bool Graph::read(const char *filename)
{
    ifstream fin(filename);
    if (!fin.good())
        return false;
    if (numVertices > 0) // some data that was there before
        makeEmpty();
    
    char buffer[20], buffer1[20];
    int nrv;
    fin >> buffer; // read the letter U or D
    if (buffer[0] == 'd' || buffer[0] == 'D')
        directed = true;
    else
        directed = false;
    fin >> nrv; // read the number of vertices
    
    for (int i=0; i<nrv; i++) // read all the names of vertices
    {
        fin >> buffer; // read the vertex name
        addVertex(string(buffer)); // this will also update numVertices
    }
    
    // read all the edges until the end of the file
    while (!fin.eof() && fin.good()) // we have not reached the end of the file
    {
        fin >> buffer;
        if (strlen(buffer) && fin.good()) // was the first name read correctly?
        {
            fin >> buffer1;
            if (strlen(buffer) && strlen(buffer1))  // how about the second?
            {
                // we know both names are valid here
                buffer[strlen(buffer)-1]='\0'; // Deleting the ','
                addEdge(string(buffer), string(buffer1));
            }
        }
    }
    fin.close();
    return true;
} // Graph::read()

// Simple print of the graph.
void Graph::print()
{
    int i;
    if (directed)
        cout << "The graph is directed" << endl;
    else
        cout << "The graph is undirected" << endl;
    cout << "The graph contains " << numVertices
    << " vertices and " << numEdges << " edges" << endl;
    
    if (numVertices)
    {
        // print all the names
        cout << "The vertex names are: ";
        for (i=0; i<numVertices; i++)
            cout << vertices[i].name << ' ';
        
        // print all the adjacency lists
        cout << endl
        << "The adjacency list for each vertex is:" << endl;
        for (i=0; i<numVertices; i++)
        {
            cout << "vertex " << i << ": ";
            ListIntIter iter;
            iter = vertices[i].edgeList.begin();
            while (iter != vertices[i].edgeList.end())
            {
                // here we have an edge from i to *iter.
                cout << *iter << ' '; // output the content of the node
                iter++;               // move to the next node
            }
            cout << endl;
        }
    }
} // Graph::print()

// Functions to be implemented by the students.

// Writes the graph to a file that is compatible with the function read.
void Graph::write(const char *filename)
{
    ofstream fout(filename);
    if (fout.is_open())
    {
        if (directed)
            fout << "D\n";
        else
            fout << "U\n"; // write if directed or not
        
        fout << numVertices << endl;
        
        if (numVertices)
        {
            // print all the names
            for (int i=0; i<numVertices; i++)
                fout << vertices[i].name << endl;
            
            for (int i=0; i<numVertices; i++)
            {
                ListIntIter iter;
                iter = vertices[i].edgeList.begin();
                while (iter != vertices[i].edgeList.end())
                {
                    // here we have an edge from i to *iter.
                    // convert node value to vertex name
                    // so read function will work
                    fout << vertices[i].name << ", "
                    << vertices[*iter].name << endl;
                    iter++;               // move to the next node
                }
            }
        }
        fout.close();
    }
    
    else
    {
        cerr << "Error opening file to write.\n\n";
        return;
    }
    
} // Graph::write()

// Inputs a graph from the user. First it should ask for the number of
// vertices, then call the function init with the number of vertices
// to automatically generate the labels as uppercase letters, and then
// ask the use to input the edges. The user will be expected to enter
// uppercase letters to identify the edges, and anything else should
// end the input process.
void Graph::input()
{
    char charInput[20], charInput2[20];
    char convInput;
    bool upper = true;
    int intInput;
    
    cout << "What are the total number of vertices: ";
    cin >> intInput;
    numVertices = intInput;
    init(intInput);
    
    // init sets directed to false, see if it should be true
    cout << "Is the graph (D)irected or (U)ndirected (input D or U accordingly): ";
    cin >> charInput;
    if (charInput[0] == 'd' || charInput[0] == 'D')
        directed = true;
    
    cout << "We will now input any edges. The format for inputting an edge "
    << "will look like:\n\n" << "A B\n\n"
    << "This indicates the edge connects A to B.\n"
    << "To end input, enter a non-uppercase letter\n";
    
    // Just creating an infinite while, the ifs will break
    // the loop if non-upper input is entered
    while (upper)
    {
        cout << "Please input an uppercase letter for " <<
        "the first half of the edge: ";
        cin >> charInput;
        convInput = charInput[0];
        if (!isupper(convInput))
        {
            upper = false;
            break;
        }
        cout << "Please input an uppercase letter for " <<
        "the second half of the edge: ";
        cin >> charInput2;
        convInput = charInput2[0];
        if (!isupper(convInput))
        {
            upper = false;
            break;
        }
        addEdge(charInput, charInput2);
    }
} // Graph::input()

// Print the graph in depth-first and breadth-first order. Implement
// one of these two functions.
void Graph::depthFirstSearch(int v)
{
    // Mark all the vertices as not visited
    for (int i = 0; i < v; i++)
        mark[i] = 0;
    printDf(v);
} // Graph::depthFirstSearch()

void Graph::printDf(int v)
{
    if (numVertices < 1)
    {
        cout << "Graph has no vertices to print.\n";
        return;
    }
    // Mark the current vertex as visited and print it
    mark[v] = 1;
    cout << vertices[v].name << " ";
    
    // Recur for all adjacent vertices
    list<int>::iterator i;
    for (i = vertices[v].edgeList.begin(); i != vertices[v].edgeList.end(); ++i)
        if (!mark[*i])
            printDf(*i);
    
} // Graph::printDf()

