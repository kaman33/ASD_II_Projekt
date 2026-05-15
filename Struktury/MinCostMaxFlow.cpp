#include "MinCostMaxFlow.h"
#include <algorithm>
#include <limits>

using namespace std;

static bool bellman_ford(const Graph& graph, int source, int sink,
    vector<long long>& dist, vector<int>& parentVertex, vector<int>& parentEdge) {

    const long long INF = numeric_limits<long long>::max();

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


