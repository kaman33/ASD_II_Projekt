#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include "Edge.h"

class Graph {
public:
    int n; // ilość domów i kopalni
    std::vector<std::vector<Edge>> graph;

    Graph(int n) : n(n), graph(n) {}

    void add_edge(int from, int to, int capacity, int cost);
    void clear();
};

#endif