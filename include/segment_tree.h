#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

typedef struct
{
    long long *tree_array;   // массив дерева 
    int tree_size;           // размер, округлённый до степени двойки
    int array_size;          // исходный размер массива
} SegmentTree;

SegmentTree* segtree_create(int array_size);
void segtree_build         (SegmentTree* segment_tree, long long* source_array, int array_size);
void segtree_update        (SegmentTree* segment_tree, int index, long long value);
long long segtree_query    (SegmentTree* segment_tree, int left_bound, int right_bound);
void segtree_free          (SegmentTree* segment_tree);

#endif // SEGMENT_TREE_H
