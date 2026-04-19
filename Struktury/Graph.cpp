#include "Graph.h"

void Graph::add_edge(int from, int to, int capacity, int cost) {
    graph[from].push_back(Edge(to, capacity, (int)graph[to].size(), cost));
    graph[to].push_back(Edge(from, 0, (int)graph[from].size() - 1, -cost));
}

void Graph::clear() {
    for (auto& edge : graph) edge.clear();
}