#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.hpp"
#include "Graph.hpp"
#include "Tree.hpp"
#include "MST_algorithm.hpp"

TEST_CASE("Graph Creation and Basic Operations") {
    // Test graph creation
    Graph graph(5);
    CHECK(graph.getVertexCount() == 5);

    // Test adding edges
    graph.addEdge(1, 2, 10);
    graph.addEdge(2, 3, 20);
    graph.addEdge(4, 5, 30);

    // Verify adjacency list
    auto adjList = graph.getAdjList();
    CHECK(adjList[1].size() == 1);
    CHECK(adjList[1][0].vertex == 2);
    CHECK(adjList[1][0].weight == 10);

    CHECK(adjList[2].size() == 1);
    CHECK(adjList[2][0].vertex == 3);
    CHECK(adjList[2][0].weight == 20);

    CHECK(adjList[4].size() == 1);
    CHECK(adjList[4][0].vertex == 5);
    CHECK(adjList[4][0].weight == 30);

   
}
