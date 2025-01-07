#include "Tree.hpp"
#include <queue>
#include <tuple>
#include <algorithm>

Tree::Tree(int vertex) : Graph(vertex), parent(vertex + 1, -1), visited(vertex + 1, false) {}

Tree Tree::primMST() {
    Tree mst(V);
    std::vector<bool> inMST(V + 1, false);
    std::priority_queue<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>, std::greater<>> pq;

    // Start from vertex 1 (arbitrary choice)
    inMST[1] = true;
    for (int v : adj[1]) {
        pq.push({1, v, 0}); // Weight is 0 for unweighted graph
    }

    while (!pq.empty()) {
        auto [u, v, weight] = pq.top();
        pq.pop();

        if (inMST[v]) continue;

        inMST[v] = true;
        mst.addEdge(u, v);

        for (int neighbor : adj[v]) {
            if (!inMST[neighbor]) {
                pq.push({v, neighbor, 0}); // Weight is 0 for unweighted graph
            }
        }
    }
    return mst;
}

Tree Tree::kruskalMST() {
    Tree mst(V); // MST tree with the same number of vertices
    std::vector<std::tuple<int, int, int>> edges; // {weight, u, v}

    // Collect all edges
    for (int u = 1; u <= V; ++u) {
        for (int v : adj[u]) {
            edges.push_back({0, u, v}); // Weight is 0 for unweighted graph
        }
    }

    // Sort edges by weight
    std::sort(edges.begin(), edges.end());

    // Disjoint set union-find initialization
    std::vector<int> parent(V + 1), rank(V + 1, 0);
    for (int i = 1; i <= V; ++i) {
        parent[i] = i;
    }

    // Kruskal's algorithm: adding edges to the MST
    for (auto [weight, u, v] : edges) {
        if (find(u, parent) != find(v, parent)) {
            mst.addEdge(u, v);
            unite(u, v, parent, rank);
        }
    }

    return mst;
}

// Helper method to find the representative of a set with path compression
int Tree::find(int u, std::vector<int>& parent) {
    if (parent[u] != u) {
        parent[u] = find(parent[u], parent); // Path compression
    }
    return parent[u];
}

// Helper method to unite two sets
void Tree::unite(int u, int v, std::vector<int>& parent, std::vector<int>& rank) {
    u = find(u, parent);
    v = find(v, parent);
    if (u != v) {
        if (rank[u] < rank[v]) std::swap(u, v);
        parent[v] = u;
        if (rank[u] == rank[v]) ++rank[u];
    }
}
