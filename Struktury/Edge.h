#ifndef EDGE_H
#define EDGE_H

struct Edge {
    int to;       // dokąd idzie
    int flow;     // przepływ
    int capacity; // pojemność
    int rev;      // ile węzłów ma dany node
    int cost;     // ile posiłków musi zjeść krasnolud, żeby przebyć drogę

    Edge(int to, int capacity, int rev, int cost) :
        to(to), flow(0), capacity(capacity), rev(rev), cost(cost) {}
};

#endif