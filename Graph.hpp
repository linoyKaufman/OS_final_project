#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <stack>

struct Edge {
    int vertex;
    int weight;
};

class Graph {
private:
    int V; 
    std::vector<std::vector<Edge>> adj; 
    std::vector<std::vector<Edge>> rev_adj; 
    friend class MST_algorithm;

    void fillOrder(int v, std::vector<bool>& visited, std::stack<int>& Stack);
    void dfs(int v, std::vector<bool>& visited, std::vector<int>& component);

public:
    int getVertexCount() const { return V; }
    const std::vector<std::vector<Edge>>& getAdjList() const {return adj;}
    Graph(int vertex);
    void addEdge(int u, int v, int weight);
    void printSCCs();
};

#endif // GRAPH_HPP
