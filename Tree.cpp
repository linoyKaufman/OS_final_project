#include <limits>
#include "Tree.hpp"
#include <queue>
#include <tuple>
#include <algorithm>
#include "Graph.hpp"
#include "UnionFind.hpp"


Tree::Tree(int vertex) : Graph(vertex), parent(vertex + 1, -1), visited(vertex + 1, false) {}

Tree Tree::primMST() {
    int V = getVertexCount();
    std::cout << "Graph has " << V << " vertices.\n";

    Tree mst(V);
    std::vector<bool> inMST(V + 1, false);
    std::vector<int> key(V + 1, std::numeric_limits<int>::max());
    std::vector<int> parent(V + 1, -1);

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
    pq.push({0, 1}); // התחלה מהצומת הראשון
    key[1] = 0;

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (u < 1 || u > V) {
            std::cerr << "Error: invalid vertex " << u << " accessed.\n";
            continue;
        }

        if (inMST[u]) continue;
        inMST[u] = true;
        std::cout << "Processing vertex: " << u << std::endl;

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

    for (int v = 2; v <= V; ++v) {
        if (parent[v] != -1) {
            mst.addEdge(parent[v], v, key[v]);
            std::cout << "Edge added to MST: " << parent[v] << " -> " << v << " with weight " << key[v] << std::endl;
        }
    }

    return mst;
}

Tree Tree::kruskalMST() {
    int V = getVertexCount(); // שימוש בפונקציית גישה
    Tree mst(V);
    std::vector<Edge> edges;

    // בניית רשימת קשתות
    for (int u = 1; u <= V; ++u) {
        for (const Edge& edge : getAdjList()[u]) { // שימוש בפונקציית גישה
            edges.push_back(edge);
        }
    }

    // מיון הקשתות לפי משקל
    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.weight < b.weight;
    });

    // Union-Find
    UnionFind uf(V + 1);
    for (const Edge& edge : edges) {
        int u = edge.vertex;
        int v = edge.weight;

        if (uf.find(u) != uf.find(v)) {
            mst.addEdge(u, v, edge.weight); // מוסיפים קשת ל-MST
            uf.unionSets(u, v); // מחברים בין קבוצות
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


