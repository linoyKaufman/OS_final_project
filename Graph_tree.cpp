#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <tuple>
#include <stack>
#include <queue>
#include <limits>
#include <iostream>

class Graph {
    int vertex; // מספר הצמתים
    std::vector<std::vector<std::tuple<int, int>>> adj; // רשימת שכנים: (יעד, משקל)

public:
    Graph(int vertex) : vertex(vertex), adj(vertex + 1) {}

    // הוספת קשת משוקללת
    void addEdge(int u, int v, int weight) {
        if (u < 1 || u > vertex || v < 1 || v > vertex) {
            std::cerr << "Error: Invalid vertex index: " << u << " or " << v << std::endl;
            return;
        }
        adj[u].emplace_back(v, weight);
    }

    // חישוב MST באמצעות אלגוריתם פרים
    std::vector<std::tuple<int, int, int>> calculateMST() {
        std::vector<bool> inMST(vertex + 1, false);
        std::vector<std::tuple<int, int, int>> mstEdges; // (מקור, יעד, משקל)
        auto compare = [](const std::tuple<int, int, int>& a, const std::tuple<int, int, int>& b) {
            return std::get<2>(a) > std::get<2>(b);
        };
        std::priority_queue<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>, decltype(compare)> pq(compare);

        inMST[1] = true; // נתחיל מהצומת הראשון
        for (const auto& edge : adj[1]) {
            pq.push({1, std::get<0>(edge), std::get<1>(edge)});
        }

        while (!pq.empty() && mstEdges.size() < vertex - 1) {
            auto [u, v, weight] = pq.top();
            pq.pop();

            if (inMST[v]) continue;
            inMST[v] = true;
            mstEdges.emplace_back(u, v, weight);

            for (const auto& edge : adj[v]) {
                if (!inMST[std::get<0>(edge)]) {
                    pq.push({v, std::get<0>(edge), std::get<1>(edge)});
                }
            }
        }

        return mstEdges;
    }

    // משקל כולל של ה-MST
    int getMSTTotalWeight(const std::vector<std::tuple<int, int, int>>& mstEdges) {
        int totalWeight = 0;
        for (const auto& edge : mstEdges) {
            totalWeight += std::get<2>(edge);
        }
        return totalWeight;
    }

    // חישוב המרחק הממוצע
    double calculateAverageDistance() {
        double totalDistance = 0;
        int pairCount = 0;

        for (int i = 1; i <= vertex; ++i) {
            std::vector<int> distances = shortestDistancesFrom(i);
            for (int j = 1; j <= vertex; ++j) {
                if (j >= i && distances[j] != std::numeric_limits<int>::max()) {
                    totalDistance += distances[j];
                    pairCount++;
                }
            }
        }

        return pairCount > 0 ? totalDistance / pairCount : 0;
    }

    // מציאת המרחק הקצר ביותר בין שני צמתים
    int getShortestDistanceInMST(const std::vector<std::tuple<int, int, int>>& mstEdges) {
        int minWeight = std::numeric_limits<int>::max();
        for (const auto& edge : mstEdges) {
            minWeight = std::min(minWeight, std::get<2>(edge));
        }
        return minWeight;
    }

   
    std::vector<int> shortestDistancesFrom(int start) {
        std::vector<int> distances(vertex + 1, std::numeric_limits<int>::max());
        distances[start] = 0;

        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
        pq.push({0, start});

        while (!pq.empty()) {
            auto [dist, u] = pq.top();
            pq.pop();

            if (dist > distances[u]) continue;

            for (const auto& edge : adj[u]) {
                int v = std::get<0>(edge);
                int weight = std::get<1>(edge);

                if (distances[u] + weight < distances[v]) {
                    distances[v] = distances[u] + weight;
                    pq.push({distances[v], v});
                }
            }
        }

        return distances;
    }
};

#endif
