#ifndef FENWICK_TREE_H
#define FENWICK_TREE_H

typedef struct
{
    long long *T;      // массив дерева 
    int n;             // размер исходного массива
} FenwickTree;

FenwickTree* fenwick_create(int n);
void fenwick_build_prefix  (FenwickTree* ft, long long* a, int n);
void fenwick_inc           (FenwickTree* ft, int i, long long delta);
long long fenwick_sum      (FenwickTree* ft, int r);
long long fenwick_range_sum(FenwickTree* ft, int l, int r);
void fenwick_free          (FenwickTree* ft);

#endif // FENWICK_TREE_H
