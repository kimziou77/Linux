#ifndef __GRAPH_H__
#define __GRAPH_H__
class Graph {
public:
    Graph(int n);
    ~Graph();
    void add_edge(int source, int target);
    void remove_edge(int source, int target);
    void print_graph();
    void depth_first_search(int start);
    void depth_first_search(int start, bool visited[]);
private:
    bool **adjMatrix;
    int size;
};
#endif
