#include <stdio.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <cmath>
#include <chrono>
#include <random>


using namespace std;
using namespace std::chrono;

class edge {
public:
    
    int vertFrom;
    int vertTo;
    float length;
    
    edge(int x, int y, float val) { //constructor
        vertFrom = x;
        vertTo = y;
        length = val;
    }
    //how to compare edges
    bool operator>(const edge otherEdge) const {
        return length > otherEdge.length;
    }
    
};

void GenerateGraph(vector<vector <float> > &graph, int V, int dimension);

float CalcEdgeLength(vector<float> vertOne, vector<float> vertTwo, int dimension);

float MakeTree(vector<edge> &MST, vector<vector<float> > &graph,
              int V, int dimension);

void TestCalcEdgeLength();

void TestMakeTreeOne();

void TestMakeTreeTwo();


int main(int argc, const char * argv[]) {
    
    assert(argc == 5);
    assert(atoi(argv[4]) <= 4);
    assert(atoi(argv[4]) != 1);
    
    
    int trials = 0;
    int V = 0;
    int dimension = 0;
    double totalWeight = 0;
    double avgEdgeLengthTotal = 0;
    
    V = atoi(argv[2]);
    trials = atoi(argv[3]);
    dimension = atoi(argv[4]);

    for (int i = 1; i <= trials; ++i) {
        
        cout << "Trial " << i << endl << endl;
        //auto start = high_resolution_clock::now();
        
        vector<vector <float> > graph;
        GenerateGraph(graph, V, dimension);
        
        vector<edge> MST;
        float treeWeight = MakeTree(MST, graph, V, dimension);
        cout << "Tree Weight: " << treeWeight << endl;
        cout << "Average Edge Length: " << treeWeight / V << endl;
        totalWeight += treeWeight;
        avgEdgeLengthTotal += treeWeight / V;
        
        
        //auto stop = high_resolution_clock::now();
        //auto duration = duration_cast<seconds>(stop - start);
        
        //cout << "Time taken by function: "
        //<< duration.count() << " seconds" << endl;
        cout << endl << endl;
    }
    
    cout << "Average weight of tree: " << totalWeight / trials << endl;
    cout << "Average weight of average edge: " << avgEdgeLengthTotal / trials << endl << endl;
    
    cout << "Trials concluded." << endl << endl;
    
}


//
//
//
//
//takes in the specifications for a graph and generates V vertices,
//with dimension coordinates
void GenerateGraph(vector<vector <float> > &graph, int V, int dimension) {
    
    //Generate random seed
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_real_distribution<double> distribution(0.0,1.0);
    

    //for number of vertices
    for (int i = 0; i < V; ++i) {
        
        vector<float> coordinates;
        
        if (dimension == 0) {
            coordinates.push_back(0);
        } else {
            for (int j = 0; j < dimension; ++j) { //for dimension of coordinates
                
                float x = distribution(generator); //generate random coordiantes [0,1]
                coordinates.push_back(x); //add coordinates to point
            }
        }
        graph.push_back(coordinates); //add vertices and coordinates to graph
    }
    return;
}
//
//
//
//
//
//takes in two vertices and calculates the edge length.
float CalcEdgeLength(vector<float> vertOne, vector<float> vertTwo, int dimension) {
    
    assert(dimension >= 2);
    assert(dimension <= 4);
    
    //Pythagorean Theorem for given dimension
    float preSquareRoot = 0;
    for (int i = 0; i < dimension; ++i) {
        
        float x = vertTwo[i] - vertOne[i]; //individual coordinate difference
        x = x*x; //square the difference
        preSquareRoot += x; //sum the differences
    }
    
    float length = sqrt(preSquareRoot); //final square root
    
    return length;
    
}
//
//
//
//
//
//creates MST given a complete graph represented by a vector containing all edges
float MakeTree(vector<edge> &MST, vector<vector<float> > &graph,
              int V, int dimension) {
    
    float weight = 0;
    
    //for first vertex
    edge startVert(-1, 0, 0);
    MST.push_back(startVert);
    //initialize edge lengths to something greater than max of 1
    edge initEdge(-1, -1, 10);
    for (int i = 1; i < V; ++i) {
        MST.push_back(initEdge);
    }
    
    //only needed if dimension = 0
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_real_distribution<double> distribution(0.0,1.0);

    
    int currVert = 0; //stores vertex to be explored
    for (int i = 0; i < V - 1; ++i) {
        
        edge currMinEdge(-1, -1, 2);
        for (int j = 0; j < V; ++j){
            
            if (graph[j][0] >= 0 && j != currVert) {//unexplored
                
                float length;
                
                if (dimension == 0) {
                    length = distribution(generator);
                } else {
                    length = CalcEdgeLength(graph[currVert], graph[j], dimension);
                }
    
                //update shortest edge to each unexplored vertex
                if (length < MST[j].length) {
                    edge thisEdge(currVert, j, length);
                    MST[j] = thisEdge;
                }
                //find the smallest edge length from this iteration
                if (MST[j].length < currMinEdge.length) {
                    currMinEdge = MST[j];
                }
            }
        }
        graph[currVert][0] = -1;
        weight += currMinEdge.length;
        currVert = currMinEdge.vertTo;
    }
    MST.erase(MST.begin());
    return weight;
}
