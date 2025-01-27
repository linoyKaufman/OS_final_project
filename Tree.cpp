#include <limits>
#include "Tree.hpp"
#include <queue>
#include <tuple>
#include <algorithm>
#include "Graph.hpp"
#include "UnionFind.hpp"

// Constructor for the Tree class, initializes the Graph and additional structures
Tree::Tree(int vertex) : Graph(vertex), parent(vertex + 1, -1), visited(vertex + 1, false) {}

// Prim's algorithm for finding the Minimum Spanning Tree (MST)
Tree Tree::primMST() {
    int V = getVertexCount();
    std::cout << "Graph has " << V << " vertices.\n";

    Tree mst(V); // Create an empty tree for MST
    std::vector<bool> inMST(V + 1, false); // Track vertices included in MST
    std::vector<int> key(V + 1, std::numeric_limits<int>::max()); // Store minimum edge weight
    std::vector<int> parent(V + 1, -1); // Store parent of each vertex

    // Priority queue for selecting the minimum weight edge
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
    pq.push({0, 1}); // Start from vertex 1
    key[1] = 0;

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        // Skip invalid or already included vertices
        if (u < 1 || u > V) {
            std::cerr << "Error: invalid vertex " << u << " accessed.\n";
            continue;
        }
        if (inMST[u]) continue;

        inMST[u] = true; // Include the vertex in MST
        std::cout << "Processing vertex: " << u << std::endl;

        // Update adjacent vertices
        for (const Edge& edge : getAdjList()[u]) {
            int v = edge.vertex;
            int weight = edge.weight;

            std::cout << "Checking edge from " << u << " to " << v << " with weight " << weight << std::endl;

            if (!inMST[v] && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
                pq.push({key[v], v});
            }
        }
    }

    // Add edges to the MST
    for (int v = 2; v <= V; ++v) {
        if (parent[v] != -1) {
            mst.addEdge(parent[v], v, key[v]);
            std::cout << "Edge added to MST: " << parent[v] << " -> " << v << " with weight " << key[v] << std::endl;
        }
    }

    return mst;
}

// Kruskal's algorithm for finding the Minimum Spanning Tree (MST)
Tree Tree::kruskalMST() {
    int V = getVertexCount();
    Tree mst(V);
    std::vector<Edge> edges;

    // Build a list of edges from the graph
    for (int u = 1; u <= V; ++u) {
        for (const Edge& edge : getAdjList()[u]) { 
            edges.push_back(edge);
        }
    }

    // Sort edges by weight
    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.weight < b.weight;
    });

    // Initialize Union-Find structure
    UnionFind uf(V + 1);
    for (const Edge& edge : edges) {
        int u = edge.vertex;
        int v = edge.weight;

        // Add edge to MST if it doesn't form a cycle
        if (uf.find(u) != uf.find(v)) {
            mst.addEdge(u, v, edge.weight);
            uf.unionSets(u, v);
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

// Method to remove an edge between two vertices
void Tree::removeEdge(int u, int v) {
    if (u < 1 || u > getVertexCount() || v < 1 || v > getVertexCount()) {
        std::cerr << "Error: Invalid vertices " << u << " or " << v << std::endl;
        return;
    }

    // Access adjacency lists as non-const references
    auto& adjListU = getAdjList()[u];
    auto& adjListV = getAdjList()[v];

    // Create new adjacency lists for u and v excluding the specified edges
    std::vector<Edge> newAdjListU;
    for (const auto& edge : adjListU) {
        if (edge.vertex != v) {
            newAdjListU.push_back(edge);
        }
    }

    std::vector<Edge> newAdjListV;
    for (const auto& edge : adjListV) {
        if (edge.vertex != u) {
            newAdjListV.push_back(edge);
        }
    }

    // Replace old adjacency lists with the new ones
    adjListU = std::move(newAdjListU);
    adjListV = std::move(newAdjListV);
}
