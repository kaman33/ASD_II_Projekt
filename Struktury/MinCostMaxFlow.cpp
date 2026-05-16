#include "MinCostMaxFlow.h"
#include <algorithm>
#include <limits>

using namespace std;

static bool bellman_ford(const Graph& graph, int source, int sink,
    vector<long long>& dist, vector<int>& parentVertex, vector<int>& parentEdge) {

    const long long INF = numeric_limits<long long>::max() / 4;

    dist.assign(graph.n, INF);
    //Wierzechołek
    parentVertex.assign(graph.n, -1);
    parentEdge.assign(graph.n, -1);
    dist[source] = 0;

    for ( int i = 0; i < graph.n - 1; i++ ) {

        bool changed = false;

        for ( int j = 0; j < graph.n; j++ ) {
            if ( dist[j] == INF) {
                continue;
            }


        for (size_t edgeIndex = 0; edgeIndex < graph.graph[j].size(); edgeIndex++) {
            const Edge& edge = graph.graph[j][edgeIndex];

            int residual = edge.capacity - edge.flow;
            if (residual <= 0) {
                continue;
            }

            long long candidate = dist[j] + edge.cost;
            if (candidate < dist[edge.to]) {
                dist[edge.to] = candidate;
                parentVertex[edge.to] = j;
                parentEdge[edge.to] = static_cast<int>(edgeIndex);
                changed = true;
            }
        }
        }
        if (!changed) {
            break;
        }


    }

    return dist[sink] != INF;

}

MinCostMaxFlowResult min_cost_max_flow(Graph& graph, int source, int sink, int maxFlow) {
    MinCostMaxFlowResult result;

    if ( source < 0 || source >= graph.n || sink < 0 || sink >= graph.n ) {
        return result;
    }

    if ( source == sink || maxFlow <= 0) {
        return result;
    }

    vector<long long> dist;
    vector<int> parentVertex;
    vector<int> parentEdge;

    while (result.flow < maxFlow && bellman_ford(graph, source, sink, dist, parentVertex, parentEdge)) {
        int pushedFlow = maxFlow - result.flow;

        for ( int vertex = sink; vertex != source; vertex = parentVertex[vertex]) {
            int previousVertex = parentVertex[vertex];
            int edgeIndex = parentEdge[vertex];
            const Edge& edge = graph.graph[previousVertex][edgeIndex];

            pushedFlow = min(pushedFlow, edge.capacity - edge.flow);
        }

        if (pushedFlow <= 0) {
            break;
        }
        for ( int vertex = sink; vertex != source; vertex = parentVertex[vertex] ) {
            int previousVertex = parentVertex[vertex];
            int edgeIndex = parentEdge[vertex];
            Edge& edge = graph.graph[previousVertex][edgeIndex];
            Edge& reverseEdge = graph.graph[vertex][edge.rev];
            edge.flow += pushedFlow;
            reverseEdge.flow -= pushedFlow;
        }
        result.flow += pushedFlow;
        result.cost += static_cast<long long>(pushedFlow) * dist[sink];
    }
        return result;

}

