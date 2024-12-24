#ifndef MST_ALGORITHM_HPP
#define MST_ALGORITHM_HPP

#include "Graph.hpp"
#include "UnionFind.hpp"
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include <climits>

class MST_algorithm {
public:
    enum class Algorithm {
        PRIM,
        KRUSKAL
    };

    static int getMSTWeight(Graph& graph, Algorithm algo);
    
private:
    static int primMST(Graph& graph);
    static int kruskalMST(Graph& graph);
};

#endif // MST_ALGORITHM_HPP
