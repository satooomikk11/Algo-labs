#include "../include/fenwick_tree.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

FenwickTree* fenwick_create(int array_size)
{
    assert(array_size > 0);
    
    FenwickTree* fenwick_tree = (FenwickTree*)calloc(1, sizeof(FenwickTree));
    if (fenwick_tree == NULL) return NULL;
    
    fenwick_tree->size = array_size;
    fenwick_tree->tree_array = (long long*)calloc(array_size, sizeof(long long));
    if (!fenwick_tree->tree_array)
    {
        free(fenwick_tree);
        return NULL;
    }
    
    return fenwick_tree;
}

void fenwick_build_prefix(FenwickTree* fenwick_tree, long long* source_array, int array_size)
{
    assert(fenwick_tree && source_array && array_size > 0 && fenwick_tree->size == array_size);
    
    long long* prefix_array = (long long*)calloc((array_size + 1), sizeof(long long));
    if (!prefix_array) return;
    
    prefix_array[0] = 0;
    for (int index = 0; index < array_size; index++)
    {
        prefix_array[index + 1] = prefix_array[index] + source_array[index];
    }
    
    for (int index = 0; index < array_size; index++)
    {
        int func_f_value = index & (index + 1);
        fenwick_tree->tree_array[index] = prefix_array[index + 1] - prefix_array[func_f_value];
    }
    
    free(prefix_array);
}

void fenwick_inc(FenwickTree* fenwick_tree, int index, long long delta)
{
    assert(fenwick_tree && (index >= 0 && index < fenwick_tree->size));

    while (index < fenwick_tree->size)
    {
        fenwick_tree->tree_array[index] += delta;
        index = index | (index + 1);
    }
}

long long fenwick_sum(FenwickTree* fenwick_tree, int right_bound)
{
    assert(fenwick_tree && (right_bound >= -1 && right_bound < fenwick_tree->size));
    
    long long result_sum = 0;
    while (right_bound >= 0)
    {
        result_sum += fenwick_tree->tree_array[right_bound];
        right_bound = (right_bound & (right_bound + 1)) - 1;
    }
    return result_sum;
}

long long fenwick_range_sum(FenwickTree* fenwick_tree, int left_bound, int right_bound)
{
    assert(fenwick_tree && (left_bound >= 0 && left_bound < fenwick_tree->size) && 
           (right_bound >= 0 && right_bound < fenwick_tree->size) && left_bound <= right_bound);
    
    if (left_bound == 0) return fenwick_sum(fenwick_tree, right_bound);
    return fenwick_sum(fenwick_tree, right_bound) - fenwick_sum(fenwick_tree, left_bound - 1);
}

void fenwick_free(FenwickTree* fenwick_tree)
{
    if (fenwick_tree)
    {
        if (fenwick_tree->tree_array) free(fenwick_tree->tree_array);
        free(fenwick_tree);
    }
}
