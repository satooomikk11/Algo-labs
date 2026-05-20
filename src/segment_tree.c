#include "segment_tree.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

SegmentTree* segtree_create(int array_size)
{
    assert(array_size > 0);
    
    SegmentTree* segment_tree = (SegmentTree*)calloc(1, sizeof(SegmentTree));
    if (!segment_tree) return NULL;
    
    segment_tree->array_size = array_size;
    segment_tree->tree_size = 1;
    while (segment_tree->tree_size < array_size) segment_tree->tree_size <<= 1;
    
    segment_tree->tree_array = (long long*)calloc(2 * segment_tree->tree_size, sizeof(long long));
    if (!segment_tree->tree_array)
    {
        free(segment_tree);
        return NULL;
    }
    
    return segment_tree;
}

void segtree_build(SegmentTree* segment_tree, long long* source_array, int array_size)
{
    assert(segment_tree && source_array && array_size > 0 && segment_tree->array_size == array_size);
    
    for (int index = 0; index < array_size; index++)
    {
        segment_tree->tree_array[segment_tree->tree_size + index] = source_array[index];
    }

    for (int index = segment_tree->tree_size - 1; index > 0; index--)
    {
        segment_tree->tree_array[index] = segment_tree->tree_array[2 * index] + segment_tree->tree_array[2 * index + 1];
    }
}

void segtree_update(SegmentTree* segment_tree, int index, long long value)
{
    assert(segment_tree && index >= 0 && index < segment_tree->array_size);
    
    int position = segment_tree->tree_size + index;
    segment_tree->tree_array[position] = value;
    position >>= 1;
    
    while (position >= 1)
    {
        segment_tree->tree_array[position] = segment_tree->tree_array[2 * position] + segment_tree->tree_array[2 * position + 1];
        position >>= 1;
    }
}

static long long segtree_query_rec(SegmentTree* segment_tree, int node_index, int node_left, int node_right, int query_left, int query_right)
{
    if (node_right < query_left || node_left > query_right) return 0;
    
    if (query_left <= node_left && node_right <= query_right) return segment_tree->tree_array[node_index];
    
    int mid = (node_left + node_right) / 2;
    long long left_sum  = segtree_query_rec(segment_tree, 2 * node_index, node_left, mid, query_left, query_right);
    long long right_sum = segtree_query_rec(segment_tree, 2 * node_index + 1, mid + 1, node_right, query_left, query_right);
    return left_sum + right_sum;
}

long long segtree_query(SegmentTree* segment_tree, int left_bound, int right_bound)
{
    assert(segment_tree && left_bound >= 0 && right_bound >= 0 && left_bound <= right_bound && right_bound < segment_tree->array_size);
    
    return segtree_query_rec(segment_tree, 1, 0, segment_tree->tree_size - 1, left_bound, right_bound);
}

void segtree_free(SegmentTree* segment_tree)
{
    if (segment_tree)
    {
        if (segment_tree->tree_array) free(segment_tree->tree_array);
        free(segment_tree);
    }
}
