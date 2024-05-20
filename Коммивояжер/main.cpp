#include <iostream>
#include <fstream>
#include "Graph.h"

using namespace std;

const int MAX_SIZE = 255;

void dijkstra(Graph& g) {
    ifstream f;
    f.open("./Graph_Dijkstra.txt");

    g.read(f);
    f.close();

    vector<int> d = g.dijkstra(5);

    for (auto e : d)
        cout << e << " ";
    cout << endl;
}

int main() {
    Graph g(MAX_SIZE);

    ifstream f;
    f.open("Graph_Salesman.txt");
    g.read(f);
    f.close();

    vector<int> path = g.salesman();

    for (auto e : path)
        cout << (e + 1) << " ";
    cout << endl;

    cout << path_lenght(g, path) << endl;
}
