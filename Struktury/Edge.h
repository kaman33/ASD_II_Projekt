#ifndef EDGE_H
#define EDGE_H

struct Edge {
    int to;       // dokad idzie
    int flow;     // przeplyw
    int capacity; // pojemnosc
    int rev;      // ile wezlow ma dany node
    int cost;     // ile posilkow musi zjesc krasnolud, zeby przebyc droge

    Edge(int to, int capacity, int rev, int cost) :
        to(to), flow(0), capacity(capacity), rev(rev), cost(cost) {}
};

#endif