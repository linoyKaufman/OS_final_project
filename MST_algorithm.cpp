#include "MST_algorithm.hpp"

// Prim's Algorithm (using a priority queue)
int MST_algorithm::primMST(Graph& graph) {
    std::vector<int> key(graph.V, INT_MAX);  // Store the minimum weight edge for each vertex
    std::vector<bool> inMST(graph.V, false);  // Track vertices already included in MST
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;  // Min-heap

    int mstWeight = 0;
    key[0] = 0;
    pq.push({0, 0});  // {weight, vertex}

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (inMST[u]) continue;
        inMST[u] = true;
        mstWeight += key[u];

        for (auto& neighbor : graph.adj[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;

            if (!inMST[v] && weight < key[v]) {
                key[v] = weight;
                pq.push({key[v], v});
            }
        }
    }

    return mstWeight;
}

// Kruskal's Algorithm
int MST_algorithm::kruskalMST(Graph& graph) {
    std::vector<std::tuple<int, int, int>> edges;  // {weight, u, v}

    for (int u = 0; u < graph.V; ++u) {
        for (auto& neighbor : graph.adj[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;
            if (u < v) {  // To avoid duplicate edges in an undirected graph
                edges.push_back({weight, u, v});
            }
        }
    }

    // Sort edges by weight
    std::sort(edges.begin(), edges.end());

    UnionFind uf(graph.V);
    int mstWeight = 0;

    for (auto& edge : edges) {
        int weight, u, v;
        std::tie(weight, u, v) = edge;

        if (uf.find(u) != uf.find(v)) {
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
