#include "MST_algorithm.hpp"
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include <limits>
#include <stdexcept>

// Prim's Algorithm (using a priority queue)
int MST_algorithm::primMST(Graph& graph) {
    if (graph.V == 0) return 0;  // Handle empty graph

    // Minimum weight edge for each vertex
    std::vector<int> key(graph.V + 1, std::numeric_limits<int>::max());
    // Track vertices in MST
    std::vector<bool> inMST(graph.V + 1, false);
    // Min-heap: priority queue with pairs {key, vertex}
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;

    int mstWeight = 0;
    key[1] = 0; // Start from vertex 1 (adjusted for 1-based indexing)
    pq.push(std::make_pair(0, 1)); // Insert {key, vertex}

    while (!pq.empty()) {
        // Get the vertex with the smallest key value
        std::pair<int, int> current = pq.top();
        pq.pop();
        int u = current.second; // Vertex number

        if (inMST[u]) continue; // Skip if already included in MST
        inMST[u] = true;
        mstWeight += current.first;

        // Iterate over all adjacent vertices
        if (u >= 1 && u <= graph.V) { // Safe bounds check for 1-based indexing
            for (int v : graph.adj[u]) { // Adjacent vertices from adj list
                int weight = 1; // Assuming weight of 1 for unweighted graph
                if (!inMST[v] && weight < key[v]) {
                    key[v] = weight;
                    pq.push(std::make_pair(key[v], v)); // Insert {key, vertex} into heap
                }
            }
        }
    }

    return mstWeight;
}

// Kruskal's Algorithm
int MST_algorithm::kruskalMST(Graph& graph) {
    if (graph.V == 0) return 0;  // Handle empty graph

    std::vector<std::tuple<int, int, int>> edges; // {weight, u, v}

    // Collect all edges
    for (int u = 1; u <= graph.V; ++u) { // Adjust for 1-based indexing
        for (int v : graph.adj[u]) {
            edges.emplace_back(1, u, v); // Assuming weight of 1 for unweighted graph
        }
    }

    // Sort edges by weight
    std::sort(edges.begin(), edges.end());

    UnionFind uf(graph.V + 1); // Adjust for 1-based indexing
    int mstWeight = 0;

    // Process edges in ascending order of weight
    for (const auto& edge : edges) {
        int weight, u, v;
        std::tie(weight, u, v) = edge;

        if (uf.find(u) != uf.find(v)) { // If not in the same set
            uf.unionSets(u, v);
            mstWeight += weight;
        }
    }

    return mstWeight;
}

// Factory method to select MST algorithm
int MST_algorithm::getMSTWeight(Graph& graph, Algorithm algo) {
    switch (algo) {
        case Algorithm::PRIM:
            return primMST(graph);
        case Algorithm::KRUSKAL:
            return kruskalMST(graph);
        default:
            throw std::invalid_argument("Invalid algorithm choice");
    }
}