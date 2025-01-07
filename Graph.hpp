#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <stack>
#include <iostream>

class Graph {
public:
    int V; // Number of vertices
    std::vector<std::vector<int>> adj; // Adjacency list
    std::vector<std::vector<int>> rev_adj; // Reverse adjacency list for SCCs

    Graph(int vertex);
    void addEdge(int u, int v);
    void fillOrder(int v, std::vector<bool>& visited, std::stack<int>& Stack);
    void dfs(int v, std::vector<bool>& visited, std::vector<int>& component);
    void printSCCs();
};

#endif // GRAPH_HPP   