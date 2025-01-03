#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <stack>

class Graph {
public:
    int V;  // Number of vertices
    
    std::vector<std::vector<int>> adj;  // Adjacency list for the graph
    std::vector<std::vector<int>> rev_adj;  // Adjacency list for the reversed graph

    Graph(int V);
    void addEdge(int u, int v);
    void printSCCs();

private:
    void fillOrder(int v, std::vector<bool>& visited, std::stack<int>& Stack);
    void dfs(int v, std::vector<bool>& visited, std::vector<int>& component);
};

#endif  // GRAPH_HPP
