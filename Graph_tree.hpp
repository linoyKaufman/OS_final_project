#ifndef TREE_HPP
#define TREE_HPP

#include "Graph.hpp"
#include <vector>
#include <iostream>

class Tree : public Graph {
public:
    Tree(int vertex);

    bool isTree();
    void dfsTree(int v, int p = -1);
    int getParent(int node) const;
    std::vector<int> getChildren(int node) const;

private:
    std::vector<int> parent; // Stores the parent of each node
    std::vector<bool> visited; // Tracks visited nodes for DFS

    bool hasCycle(int v, std::vector<bool>& visited, int parent);
};

#endif // TREE_HPP
