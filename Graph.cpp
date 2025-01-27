#include "Graph.hpp"
#include <iostream>
#include <algorithm>

// Constructor to initialize the graph with a given number of vertices.
Graph::Graph(int vertex) : V(vertex), adj(vertex + 1), rev_adj(vertex + 1) {}

// Function to add a directed edge from vertex 'u' to vertex 'v' with a given weight.
void Graph::addEdge(int u, int v, int weight) {
    // Ensure vertices are within valid range.
    if (u < 1 || u > V || v < 1 || v > V) {
        std::cerr << "Error: invalid vertex index: " << u << " or " << v << std::endl;
        return;
    }

    // Add edge to the adjacency list of the graph.
    adj[u].push_back({v, weight});       
    // Add reverse edge to the reverse adjacency list.
    rev_adj[v].push_back({u, weight});  
}

// Helper function to perform DFS and fill the stack based on finishing times of vertices.
void Graph::fillOrder(int v, std::vector<bool>& visited, std::stack<int>& Stack) {
    visited[v] = true; // Mark the current vertex as visited.

    // Traverse all neighbors of the current vertex.
    for (const auto& edge : adj[v]) {
        int neighbor = edge.vertex; // Get the neighboring vertex.
        if (!visited[neighbor]) {
            fillOrder(neighbor, visited, Stack);
        }
    }

    // Push the vertex to the stack after finishing all its neighbors.
    Stack.push(v);
}

// Helper function to perform DFS on the reverse graph and collect vertices in the current SCC.
void Graph::dfs(int v, std::vector<bool>& visited, std::vector<int>& component) {
    visited[v] = true; // Mark the current vertex as visited.
    component.push_back(v); // Add the vertex to the current component.

    // Traverse all neighbors in the reverse graph.
    for (const auto& edge : rev_adj[v]) {
        int neighbor = edge.vertex; // Get the neighboring vertex in the reverse graph.
        if (!visited[neighbor]) {
            dfs(neighbor, visited, component);
        }
    }
}

// Function to find and print all Strongly Connected Components (SCCs) in the graph.
void Graph::printSCCs() {
    std::stack<int> Stack;
    std::vector<bool> visited(V + 1, false);

    // Step 1: Perform DFS and fill the stack with vertices based on their finishing times.
    for (int i = 1; i <= V; i++) {
        if (!visited[i]) {
            fillOrder(i, visited, Stack);
        }
    }

    // Step 2: Reset the visited array for processing the reverse graph.
    std::fill(visited.begin(), visited.end(), false);

    // Step 3: Process all vertices in the order defined by the stack.
    while (!Stack.empty()) {
        int v = Stack.top();
        Stack.pop();

        // If the vertex has not been visited, it belongs to a new SCC.
        if (!visited[v]) {
            std::vector<int> component;
            dfs(v, visited, component); // Perform DFS on the reverse graph.

            // Print the current SCC.
            for (int i : component) {
                std::cout << i << " ";
            }
            std::cout << std::endl;
        }
    }
}
