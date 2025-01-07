#ifndef TREE_HPP
#define TREE_HPP

#include "Graph.hpp"
#include <vector>
#include <iostream>

class Tree : public Graph {
public:
    // Constructor
    explicit Tree(int vertex);

    // Check if the graph is a valid tree
    bool isTree();

    // Perform DFS to set parent-child relationships
    void dfsTree(int v, int p = -1);

    // Get the parent of a given node
    int getParent(int node) const;

    // Get the children of a given node
    std::vector<int> getChildren(int node) const;

private:
    // Data members
    std::vector<int> parent; // Parent array for each vertex
    std::vector<bool> visited; // Visited array for DFS

    // Helper method to detect cycles in the graph
    bool hasCycle(int v, std::vector<bool>& visited, int parent);
};

#endif 