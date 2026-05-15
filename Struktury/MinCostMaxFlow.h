#ifndef MINCOSTMAXFLOW_H
#define MINCOSTMAXFLOW_H

#include "Graph.h"
#include <limits>

typedef struct {
    int flow;
    long long cost;
    MinCostMaxFlowResult() : flow(0), cost(0) {}
    MinCostMaxFlowResult(int flow, long long cost) : flow(flow), cost(cost) {}
}MinCostMaxFlowResult;

MinCostMaxFlowResult min_cost_max_flow(Graph& graph, int source, int sink, int maxFlow = std::numeric_limits<int>::max());

#endif
