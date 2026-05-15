#include "segment_tree.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

SegmentTree* segtree_create(int n)
{
    assert(n > 0);
    
    SegmentTree* st = (SegmentTree*)calloc(1, sizeof(SegmentTree));
    if (!st) return NULL;
    
    st->n = n;
    st->size = 1;
    while (st->size < n) st->size <<= 1;
    
    st->tree = (long long*)calloc(2 * st->size, sizeof(long long));
    if (!st->tree)
    {
        free(st);
        return NULL;
    }
    
    return st;
}

void segtree_build(SegmentTree* st, long long* a, int n)
{
    assert(st && a && n > 0 && st->n == n);
    
    for (int i = 0; i < n; i++)
    {
        st->tree[st->size + i] = a[i];
    }

    for (int i = st->size - 1; i > 0; i--)
    {
        st->tree[i] = st->tree[2 * i] + st->tree[2 * i + 1];
    }
}

void segtree_update(SegmentTree* st, int idx, long long value)
{
    assert(st && idx >= 0 && idx < st->n);
    
    int pos = st->size + idx;
    st->tree[pos] = value;
    pos >>= 1;
    
    while (pos >= 1)
    {
        st->tree[pos] = st->tree[2 * pos] + st->tree[2 * pos + 1];
        pos >>= 1;
    }
}

static long long segtree_query_rec(SegmentTree* st, int node, int node_l, int node_r, int q_l, int q_r)
{
    if (node_r < q_l || node_l > q_r) return 0;
    
    if (q_l <= node_l && node_r <= q_r) return st->tree[node];
    
    int mid = (node_l + node_r) / 2;
    long long left_sum  = segtree_query_rec(st, 2 * node, node_l, mid, q_l, q_r);
    long long right_sum = segtree_query_rec(st, 2 * node + 1, mid + 1, node_r, q_l, q_r);
    return left_sum + right_sum;
}

long long segtree_query(SegmentTree* st, int l, int r)
{
    assert(st && l >= 0 && r >= 0 && l <= r && r < st->n);
    
    return segtree_query_rec(st, 1, 0, st->size - 1, l, r);
}

void segtree_free(SegmentTree* st)
{
    if (st)
    {
        if (st->tree) free(st->tree);
        free(st);
    }
}
