#ifndef MSTFACTORY_HPP
#define MSTFACTORY_HPP

#include "Graph.hpp"
#include "UnionFind.hpp"

class MSTFactory {
public:
    enum class Algorithm {
        PRIM,
        KRUSKAL
    };

    static int getMSTWeight(Graph& graph, Algorithm algo);
};

#endif // MSTFACTORY_HPP
