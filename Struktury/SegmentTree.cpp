#include "SegmentTree.h"
#include <iostream>
using namespace std;

// !!! Indeksowanie od 0

void SegmentTree::build(int v, int l, int r)
{
    if (l == r)
    {
        tree[v] = A[l];
        return;
    }

    int mid = (l + r) / 2;
    build(2 * v + 1, l, mid);
    build(2 * v + 2, mid + 1, r);

    tree[v] = max(tree[2 * v + 1], tree[2 * v + 2]);
}

int SegmentTree::query(int v, int l, int r, int ql, int qr)
{
    if (r < ql || qr < l)
        return numeric_limits<int>::lowest();
    ; // rozlaczne
    if (ql <= l && r <= qr)
        return tree[v]; // zawarty

    int mid = (l + r) / 2;
    int x = query(2 * v + 1, l, mid, ql, qr);
    int y = query(2 * v + 2, mid + 1, r, ql, qr);

    return max(x, y);
}

void SegmentTree::update(int v, int l, int r, int idx, int x)
{
    if (l == r)
    {
        tree[v] = x;
        return;
    }

    int mid = (l + r) / 2;
    if (idx <= mid)
        update(2 * v + 1, l, mid, idx, x);
    else
        update(2 * v + 2, mid + 1, r, idx, x);

    tree[v] = max(tree[2 * v + 1], tree[2 * v + 2]);
}

SegmentTree::SegmentTree(const vector<int> &a) : A(a), n(a.size())
{
    tree.resize(4 * n);
    build(0, 0, n - 1);
}

void SegmentTree::update(int i, int val)
{
    update(0, 0, n - 1, i, val);
}

int SegmentTree::query(int l, int r)
{
    return query(0, 0, n - 1, l, r);
}
