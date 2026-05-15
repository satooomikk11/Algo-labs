#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

typedef struct
{
    long long *tree;   // массив дерева 
    int size;          // размер, округлённый до степени двойки
    int n;             // исходный размер массива
} SegmentTree;

SegmentTree* segtree_create(int n);
void segtree_build         (SegmentTree* st, long long* a, int n);
void segtree_update        (SegmentTree* st, int idx, long long value);
long long segtree_query    (SegmentTree* st, int l, int r);
void segtree_free          (SegmentTree* st);

#endif // SEGMENT_TREE_H
