#include "Graph.hpp"
#include <cassert>
#include <iostream>
#include <stack>
void Graph::depth_first_search(int start) {
    bool *visited = new bool[this->size];
    for (int i = 0; i < this->size; i++) {
        visited[i] = false;
    }
    depth_first_search(start, visited);
    std::cout << std::endl;
    delete visited;
}
void Graph::depth_first_search(int start, bool visited[]) {
    visited[start] = true;
    std::cout << start << " ";
    for (int i = 0; i < this->size; i++) {
        if (this->adjMatrix[start][i] && !visited[i]) {
            depth_first_search(i, visited);
        }
    }
}

Graph::Graph(int n) {
    assert(n > 0);
    this->size = n;
    this->adjMatrix = new bool *[n];
    for (int i = 0; i < n; i++) {
        adjMatrix[i] = new bool[n];
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            adjMatrix[i][j] = false;
        }
    }
}

Graph::~Graph() { delete adjMatrix; }

void Graph::add_edge(int source, int target) {
    assert(source >= 0 && source < size);
    assert(target >= 0 && target < size);
    this->adjMatrix[source][target] = true;
}

void Graph::remove_edge(int source, int target) {
    assert(source >= 0 && source < size);
    assert(target >= 0 && target < size);
    this->adjMatrix[source][target] = false;
}
void Graph::print_graph() {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            std::cout << adjMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
