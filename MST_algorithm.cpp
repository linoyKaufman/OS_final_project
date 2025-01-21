#include "MST_algorithm.hpp"
#include "UnionFind.hpp"
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include <limits>
#include <stdexcept>

// Prim's Algorithm (using a priority queue)
int MST_algorithm::primMST(Graph& graph) {
    if (graph.V == 0) return 0; // Handle empty graph

    int V = graph.V;
    std::vector<int> key(V + 1, std::numeric_limits<int>::max());
    std::vector<bool> inMST(V + 1, false);
    key[1] = 0;

    auto cmp = [&key](int left, int right) { return key[left] > key[right]; };
    std::priority_queue<int, std::vector<int>, decltype(cmp)> pq(cmp);

    pq.push(1);
    int mstWeight = 0;

    while (!pq.empty()) {
        int u = pq.top();
        pq.pop();

        if (!inMST[u]) {
            inMST[u] = true;
            mstWeight += key[u];

            for (const Edge& edge : graph.adj[u]) {
                int v = edge.vertex;
                int weight = edge.weight;
                if (!inMST[v] && weight < key[v]) {
                    key[v] = weight;
                    pq.push(v);
                }
            }
        }
    }

    return mstWeight;
}

// Kruskal's Algorithm
int MST_algorithm::kruskalMST(Graph& graph) {
    if (graph.V == 0) return 0; // Handle empty graph

    std::vector<Edge> edges;
    for (int u = 1; u <= graph.V; ++u) {
        for (const Edge& edge : graph.adj[u]) {
            edges.push_back(edge);
        }
    }

    // Sort edges by weight
    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.weight < b.weight;
    });

    UnionFind uf(graph.V + 1);
    int mstWeight = 0;

    for (const Edge& edge : edges) {
        int u = edge.vertex;  // מקור
        int v = edge.weight;  // יעד

        if (uf.find(u) != uf.find(v)) {
            uf.unionSets(u, v);
            mstWeight += edge.weight;
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