#ifndef TREE_HPP
#define TREE_HPP

#include "Graph.hpp"
#include <vector>
#include <iostream>

class Tree : public Graph {
    protected:
    std::vector<std::vector<Edge>> adj;
public:
    Tree(int vertex);
    

    // Add this declaration if missing
    std::vector<std::vector<Edge>>& getAdjList(){ return adj;}


    const std::vector<std::vector<Edge>>& getAdjList() const{return adj;}

    Tree primMST();
    Tree kruskalMST();

    void removeEdge(int u, int v); // Ensure this is declared in the public section
     // Helper method for find (used in union-find)
    int find(int u, std::vector<int>& parent);

    // Helper method for union (used in union-find)
    void unite(int u, int v, std::vector<int>& parent, std::vector<int>& rank);

private:
    // Other private members
    std::vector<int> parent;
    std::vector<bool>visited;
};

#endif  // TREE_HPP

