#include "Tree.hpp"

Tree::Tree(int vertex) : Graph(vertex), parent(vertex + 1, -1), visited(vertex + 1, false) {}

bool Tree::isTree() {
    std::fill(visited.begin(), visited.end(), false);
    if (hasCycle(1, visited, -1)) {
        return false; // Graph contains a cycle
    }

    // Check if all vertices are visited (graph is connected)
    for (int i = 1; i <= V; i++) {
        if (!visited[i]) return false;
    }
    return true;
}

void Tree::dfsTree(int v, int p) {
    visited[v] = true;
    parent[v] = p;

    for (int u : adj[v]) {
        if (!visited[u]) {
            dfsTree(u, v);
        }
    }
}

int Tree::getParent(int node) const {
    if (node < 1 || node > V) {
        std::cerr << "Error: Node out of range" << std::endl;
        return -1;
    }
    return parent[node];
}

std::vector<int> Tree::getChildren(int node) const {
    std::vector<int> children;
    for (int u : adj[node]) {
        if (parent[u] == node) {
            children.push_back(u);
        }
    }
    return children;
}

bool Tree::hasCycle(int v, std::vector<bool>& visited, int parent) {
    visited[v] = true;

    for (int u : adj[v]) {
        if (!visited[u]) {
            if (hasCycle(u, visited, v)) return true;
        } else if (u != parent) {
            return true; // Back edge detected
        }
    }
    return false;
}
