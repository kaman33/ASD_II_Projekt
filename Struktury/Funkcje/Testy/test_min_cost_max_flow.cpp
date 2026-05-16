#include "../../Graph.h"
#include "../../MinCostMaxFlow.h"

#include <iostream>

bool test1() {
    Graph graph(4);

    graph.add_edge(0, 1, 1, 2);
    graph.add_edge(0, 2, 1, 1);
    graph.add_edge(1, 3, 1, 2);
    graph.add_edge(2, 3, 1, 5);

    MinCostMaxFlowResult result = min_cost_max_flow(graph, 0, 3, 1);

    return result.flow == 1 && result.cost == 4;
}

bool test2() {
    Graph graph(4);

    graph.add_edge(0, 1, 1, 2);
    graph.add_edge(0, 2, 1, 1);
    graph.add_edge(1, 3, 1, 2);
    graph.add_edge(2, 3, 1, 5);

    MinCostMaxFlowResult result = min_cost_max_flow(graph, 0, 3, 2);

    return result.flow == 2 && result.cost == 10;
}

bool test3() {
    Graph graph(4);

    graph.add_edge(0, 1, 1, 2);
    graph.add_edge(1, 2, 1, 3);

    MinCostMaxFlowResult result = min_cost_max_flow(graph, 0, 3, 1);

    return result.flow == 0 && result.cost == 0;
}

int main() {
    std::cout << "Testy dla MinCostMaxFlow:" << std::endl;
    std::cout << "Test 1: " << (test1() ? "OK" : "ERROR") << " (najtansza sciezka dla maxFlow = 1)" << std::endl;
    std::cout << "Test 2: " << (test2() ? "OK" : "ERROR") << " (dwie sciezki dla maxFlow = 2)" << std::endl;
    std::cout << "Test 3: " << (test3() ? "OK" : "ERROR") << " (brak sciezki do sink)" << std::endl;

    return 0;
}
