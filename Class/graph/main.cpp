#include "Graph.hpp"
#include <iostream>
using namespace std;
int main() {
    Graph g(7);
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.add_edge(1, 0);
    g.add_edge(1, 3);
    g.add_edge(1, 4);
    g.add_edge(2, 0);
    g.add_edge(2, 4);
    g.add_edge(3, 1);
    g.add_edge(3, 6);
    g.add_edge(4, 1);
    g.add_edge(4, 2);
    g.add_edge(4, 6);
    g.add_edge(5, 6);
    g.add_edge(6, 3);
    g.add_edge(6, 4);
    g.add_edge(6, 5);

    cout << "--- Adjacency Matrix ---" << endl;
    g.print_graph();

    cout << endl << "--- Depth First Search ---" << endl;
    g.depth_first_search(0);
    return 0;
}
