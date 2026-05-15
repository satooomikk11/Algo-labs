#include "../include/sparse_table.h"
#include <stdlib.h>
#include <math.h>
#include <assert.h>

static long long min_ll(long long a, long long b)
{
    return a < b ? a : b;
}

static int* precompute_fl_log(int n)
{
    assert(!(n < 0));
    
    int* fl_log = (int*)calloc((n + 1), sizeof(int));
    if (!fl_log) return NULL;
    
    if (n >= 1)
    {
        fl_log[1] = 0;
    }

    for (int i = 2; i <= n; i++)
    {
        fl_log[i] = fl_log[i / 2] + 1;
    }

    return fl_log;
}

SparseTable* st_create_v1(long long* a, int n)
{
    assert(a && n > 0);
    
    SparseTable* st = (SparseTable*)calloc(1, sizeof(SparseTable));
    if (!st) return NULL;
    
    st->variant = 1;
    st->n = n;
    st->logn   = (int)(log2(n)) + 1;
    st->fl_log = precompute_fl_log(n);
    if (!st->fl_log)
    {
        free(st);
        return NULL;
    }
    
    st->st = (long long**)calloc(n, sizeof(long long*));
    if (!st->st)
    {
        free(st->fl_log);
        free(st);
        return NULL;
    }
    
    for (int i = 0; i < n; i++)
    {
        st->st[i] = (long long*)calloc(st->logn, sizeof(long long));
        if (!st->st[i])
        {
            for (int j = 0; j < i; j++) free(st->st[j]);
            free(st->st);
            free(st->fl_log);
            free(st);
            return NULL;
        }
        st->st[i][0] = a[i];
    }
    
    for (int k = 1; k < st->logn; k++)
    {
        int step = 1 << (k - 1);
        for (int i = 0; i + (1 << k) <= n; i++)
        {
            st->st[i][k] = min_ll(st->st[i][k - 1], st->st[i + step][k - 1]);
        }
    }
    
    return st;
}

SparseTable* st_create_v2(long long* a, int n)
{
    assert(a && n > 0);
    
    SparseTable* st = (SparseTable*)calloc(1, sizeof(SparseTable));
    if (!st) return NULL;
    
    st->variant = 2;
    st->n = n;
    st->logn = (int)(log2(n)) + 1;
    st->fl_log = precompute_fl_log(n);
    if (!st->fl_log)
    {
        free(st);
        return NULL;
    }
    
    st->st = (long long**)calloc(st->logn, sizeof(long long*));
    if (!st->st)
    {
        free(st->fl_log);
        free(st);
        return NULL;
    }
    
    for (int k = 0; k < st->logn; k++)
    {
        st->st[k] = (long long*)calloc(n, sizeof(long long));
        if (!st->st[k])
        {
            for (int j = 0; j < k; j++) free(st->st[j]);
            free(st->st);
            free(st->fl_log);
            free(st);
            return NULL;
        }
    }
    
    for (int i = 0; i < n; i++)
    {
        st->st[0][i] = a[i];
    }

    for (int k = 1; k < st->logn; k++)
    {
        int step = 1 << (k - 1);
        for (int i = 0; i + (1 << k) <= n; i++)
        {
            st->st[k][i] = min_ll(st->st[k - 1][i], st->st[k - 1][i + step]);
        }
    }
    
    return st;
}

long long st_query(SparseTable* st, int l, int r)
{
    assert(st && l >= 0 && r >= 0 && l <= r && r < st->n);
    
    int len = r - l + 1;
    int j = st->fl_log[len];
    
    if (st->variant == 1)
        return min_ll(st->st[l][j], st->st[r - (1 << j) + 1][j]);
    else
        return min_ll(st->st[j][l], st->st[j][r - (1 << j) + 1]);
}

void st_free(SparseTable* st)
{
    if (!st) return;
    
    if (st->variant == 1)
    {
        for (int i = 0; i < st->n; i++)
        {
            if (st->st && st->st[i]) free(st->st[i]);
        }
    }
    else
    {
        for (int k = 0; k < st->logn; k++)
        {
            if (st->st && st->st[k]) free(st->st[k]);
        }
    }
    
    if (st->st)     free(st->st);
    if (st->fl_log) free(st->fl_log);
    free(st);
}
