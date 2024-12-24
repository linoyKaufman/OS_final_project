#ifndef UNIONFIND_HPP
#define UNIONFIND_HPP

#include <vector>

class UnionFind {
public:
    std::vector<int> parent, rank;

    UnionFind(int n);

    int find(int x);
    void unionSets(int x, int y);
};

#endif // UNIONFIND_HPP
