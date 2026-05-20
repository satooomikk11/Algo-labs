#ifndef FENWICK_TREE_H
#define FENWICK_TREE_H

typedef struct
{
    long long *tree_array;      // массив дерева 
    int size;                   // размер исходного массива
} FenwickTree;

FenwickTree* fenwick_create(int array_size);
void fenwick_build_prefix  (FenwickTree* fenwick_tree, long long* source_array, int array_size);
void fenwick_inc           (FenwickTree* fenwick_tree, int index, long long delta);
long long fenwick_sum      (FenwickTree* fenwick_tree, int right_bound);
long long fenwick_range_sum(FenwickTree* fenwick_tree, int left_bound, int right_bound);
void fenwick_free          (FenwickTree* fenwick_tree);

#endif // FENWICK_TREE_H
