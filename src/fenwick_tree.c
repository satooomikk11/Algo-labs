#include "../include/fenwick_tree.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

FenwickTree* fenwick_create(int n)
{
    assert(n > 0);
    
    FenwickTree* ft = (FenwickTree*)calloc(1, sizeof(FenwickTree));
    if (ft == NULL) return NULL;
    
    ft->n = n;
    ft->T = (long long*)calloc(n, sizeof(long long));
    if (!ft->T)
    {
        free(ft);
        return NULL;
    }
    
    return ft;
}

void fenwick_build_prefix(FenwickTree* ft, long long* a, int n)
{
    assert(ft && a && n > 0 && ft->n == n);
    
    long long* prefix = (long long*)calloc((n + 1), sizeof(long long));
    if (!prefix) return;
    
    prefix[0] = 0;
    for (int i = 0; i < n; i++)
    {
        prefix[i + 1] = prefix[i] + a[i];
    }
    
    for (int i = 0; i < n; i++)
    {
        int F = i & (i + 1);
        ft->T[i] = prefix[i + 1] - prefix[F];
    }
    
    free(prefix);
}

void fenwick_inc(FenwickTree* ft, int i, long long delta)
{
    assert(ft && (i >= 0 && i < ft->n));

    while (i < ft->n)
    {
        ft->T[i] += delta;
        i = i | (i + 1);
    }
}

long long fenwick_sum(FenwickTree* ft, int r)
{
    assert(ft && (r >= -1 && r < ft->n));
    
    long long result = 0;
    while (r >= 0)
    {
        result += ft->T[r];
        r = (r & (r + 1)) - 1;
    }
    return result;
}

long long fenwick_range_sum(FenwickTree* ft, int l, int r)
{
    assert(ft && (l >= 0 && l < ft->n) && (r >= 0 && r < ft->n) && l <= r);
    
    if (l == 0) return fenwick_sum(ft, r);
    return fenwick_sum(ft, r) - fenwick_sum(ft, l - 1);
}

void fenwick_free(FenwickTree* ft)
{
    if (ft)
    {
        if (ft->T) free(ft->T);
        free(ft);
    }
}