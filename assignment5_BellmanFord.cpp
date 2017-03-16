#include <iostream>
#include <iomanip>

using namespace std;

struct Edge {
    int source, destination, weight;
};

struct Graph {
    int V, E;	// V -> Number of vertices, E -> Number of edges
    Edge* edge; // Graph is represented by array of edges
};

Graph* createGraph(int, int);	// Create graph with V vertices and E edges
void print(int [], int);		// Print solution
void BellmanFord(Graph*, int);	// Bellman-Ford Algorithm

int main() {
    
    int V = 5, E = 8;  // V -> Number of vertices, E -> Number of edges
    Graph* graph = createGraph(V, E);
 
    graph->edge[0].source = 0;
    graph->edge[0].destination = 1;
    graph->edge[0].weight = -1;

    graph->edge[1].source = 0;
    graph->edge[1].destination = 2;
    graph->edge[1].weight = 4;

    graph->edge[2].source = 1;
    graph->edge[2].destination = 2;
    graph->edge[2].weight = 3;

    graph->edge[3].source = 1;
    graph->edge[3].destination = 3;
    graph->edge[3].weight = 2;

    graph->edge[4].source = 1;
    graph->edge[4].destination = 4;
    graph->edge[4].weight = 2;

    graph->edge[5].source = 3;
    graph->edge[5].destination = 2;
    graph->edge[5].weight = 5;

    graph->edge[6].source = 3;
    graph->edge[6].destination = 1;
    graph->edge[6].weight = 1;

    graph->edge[7].source = 4;
    graph->edge[7].destination = 3;
    graph->edge[7].weight = -3;
 
    BellmanFord(graph, 0);
 
    return 0;
}

// Create graph with V vertices and E edges
Graph* createGraph(int V, int E) {
    Graph* graph = new Graph;
    graph->V = V;
    graph->E = E;
    graph->edge = new Edge;

    return graph;
}

// Print solution
void print(int distance[], int n) {
    cout << "Vertex\tDistance from source" << endl;
    for(int i = 0; i < n; i++)
        cout << i << setw(9) << distance[i] << endl;
}

// Bellman-Ford Algorithm
void BellmanFord(Graph* graph, int source) {
    int V = graph->V, E = graph->E, i, j;
    int distance[V];

    for(i = 0; i < V; i++)
        distance[i] = INT_MAX;
    distance[source] = 0;

    for(i = 1; i < V; i++) {
        for(j = 0; j < E; j++) {
            int u = graph->edge[j].source, v = graph->edge[j].destination, wt = graph->edge[j].weight;
            if(distance[u] != INT_MAX && distance[u] + wt < distance[v])
                distance[v] = distance[u] + wt;
        }
    }

    for(i = 0; i < E; i++) {
        int u = graph->edge[i].source, v = graph->edge[i].destination, wt = graph->edge[i].weight;
        if(distance[u] != INT_MAX && distance[u] + wt < distance[v])
            cout << "Graph contains a negative weight cycle." << endl;
    }

    print(distance, V);
}
