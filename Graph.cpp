#include "Graph.hpp"
#include <iostream>
#include <algorithm>

// מבנה קשת עם משקל
Graph::Graph(int vertex) : V(vertex), adj(vertex + 1), rev_adj(vertex + 1) {}

void Graph::addEdge(int u, int v, int weight) {
    if (u < 1 || u > V || v < 1 || v > V) {
        std::cerr << "Error: invalid vertex index: " << u << " or " << v << std::endl;
        return;
    }

    adj[u].push_back({v, weight});       // הוספת קשת קדימה עם משקל
    rev_adj[v].push_back({u, weight});   // הוספת קשת הפוכה עם משקל
}

void Graph::fillOrder(int v, std::vector<bool>& visited, std::stack<int>& Stack) {
    visited[v] = true;

    // לולאה דרך הקשתות היוצאות מהקודקוד הנוכחי
    for (const auto& edge : adj[v]) {
        int neighbor = edge.vertex; // הקודקוד השכן
        if (!visited[neighbor]) {
            fillOrder(neighbor, visited, Stack);
        }
    }

    // הוספת הקודקוד למחסנית לאחר סיום כל הצאצאים
    Stack.push(v);
}

void Graph::dfs(int v, std::vector<bool>& visited, std::vector<int>& component) {
    visited[v] = true;
    component.push_back(v); // הוספת הקודקוד לרשימת הקומפוננטה

    // לולאה דרך הקשתות ההפוכות של הקודקוד הנוכחי
    for (const auto& edge : rev_adj[v]) {
        int neighbor = edge.vertex; // הקודקוד השכן בקשת ההפוכה
        if (!visited[neighbor]) {
            dfs(neighbor, visited, component);
      }
   }
}

void Graph::printSCCs() {
    std::stack<int> Stack;
    std::vector<bool> visited(V + 1, false);

    // Fill vertices in stack according to their finishing times
    for (int i = 1; i <= V; i++) {
        if (!visited[i])
            fillOrder(i, visited, Stack);
    }

    // Process all vertices in order defined by Stack
    std::fill(visited.begin(), visited.end(), false);

    while (!Stack.empty()) {
        int v = Stack.top();
        Stack.pop();

        if (!visited[v]) {
            std::vector<int> component;
            dfs(v, visited, component);
            
            // Output the component as an SCC
            for (int i : component) {
                std::cout << i << " ";
            }
            std::cout << std::endl;
        }
    }
}