#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include <vector>
#include <climits>
#include <limits>
#include <algorithm>

class SegmentTree{
private:
    std::vector<int> tree;
    std::vector<int> A;
    int n;

    void build(int v, int l, int r);
    int query(int v, int l, int r, int ql, int qr);
    void update(int v, int l, int r, int idx, int x);

public:
    SegmentTree(const std::vector<int> &a);

    void update(int i, int val);
    int query(int l, int r);

};

#endif