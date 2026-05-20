#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/fenwick_tree.h"
#include "../include/segment_tree.h"
#include "../include/sparse_table.h"

#define ARRAY_SIZE   1000000
#define QUERY_COUNT  1000000
#define REPEAT_COUNT 5

long long* generate_array(int element_count)
{
    long long* array = (long long*)calloc(element_count, sizeof(long long));
    for (int index = 0; index < element_count; index++)
    {
        array[index] = rand() % 1000000;
    }
    return array;
}

typedef struct
{
    int left_bound;
    int right_bound;
} Query;

Query* generate_queries(int query_count, int array_size)
{
    Query* queries = (Query*)calloc(query_count, sizeof(Query));
    for (int query_index = 0; query_index < query_count; query_index++)
    {
        queries[query_index].left_bound  = rand() % array_size;
        queries[query_index].right_bound = rand() % array_size;
        if (queries[query_index].left_bound > queries[query_index].right_bound)
        {
            int temp = queries[query_index].left_bound;
            queries[query_index].left_bound  = queries[query_index].right_bound;
            queries[query_index].right_bound = temp;
        }
    }
    return queries;
}

double test_fenwick(long long* source_array, int array_size, Query* queries, int query_count)
{
    FenwickTree* fenwick_tree = fenwick_create(array_size);
    fenwick_build_prefix(fenwick_tree, source_array, array_size);
    
    clock_t start_time = clock();
    for (int query_index = 0; query_index < query_count; query_index++)
    {
        fenwick_range_sum(fenwick_tree, queries[query_index].left_bound, queries[query_index].right_bound);
    }
    clock_t end_time = clock();
    
    fenwick_free(fenwick_tree);
    return (double)(end_time - start_time) / CLOCKS_PER_SEC;
}

double test_segtree(long long* source_array, int array_size, Query* queries, int query_count)
{
    SegmentTree* segment_tree = segtree_create(array_size);
    segtree_build(segment_tree, source_array, array_size);
    
    clock_t start_time = clock();
    for (int query_index = 0; query_index < query_count; query_index++)
    {
        segtree_query(segment_tree, queries[query_index].left_bound, queries[query_index].right_bound);
    }
    clock_t end_time = clock();
    
    segtree_free(segment_tree);
    return (double)(end_time - start_time) / CLOCKS_PER_SEC;
}

double test_st_v1(long long* source_array, int array_size, Query* queries, int query_count)
{
    SparseTable* sparse_table = st_create_v1(source_array, array_size);
    
    clock_t start_time = clock();
    for (int query_index = 0; query_index < query_count; query_index++)
    {
        st_query(sparse_table, queries[query_index].left_bound, queries[query_index].right_bound);
    }
    clock_t end_time = clock();
    
    st_free(sparse_table);
    return (double)(end_time - start_time) / CLOCKS_PER_SEC;
}

double test_st_v2(long long* source_array, int array_size, Query* queries, int query_count)
{
    SparseTable* sparse_table = st_create_v2(source_array, array_size);
    
    clock_t start_time = clock();
    for (int query_index = 0; query_index < query_count; query_index++)
    {
        st_query(sparse_table, queries[query_index].left_bound, queries[query_index].right_bound);
    }
    clock_t end_time = clock();
    
    st_free(sparse_table);
    return (double)(end_time - start_time) / CLOCKS_PER_SEC;
}

int main()
{
    srand(time(NULL));
    
    printf("\nЛабораторная работа 7. Запросы на отрезках\n\n");
    
    printf("RSQ: Фенвик и дерево отрезков\n");
    double fenwick_times[REPEAT_COUNT], segtree_times[REPEAT_COUNT];
    
    for (int repeat_index = 0; repeat_index < REPEAT_COUNT; repeat_index++)
    {
        long long* random_array = generate_array(ARRAY_SIZE);
        Query* random_queries   = generate_queries(QUERY_COUNT, ARRAY_SIZE);
        
        fenwick_times[repeat_index] = test_fenwick(random_array, ARRAY_SIZE, random_queries, QUERY_COUNT);
        segtree_times[repeat_index] = test_segtree(random_array, ARRAY_SIZE, random_queries, QUERY_COUNT);
        
        free(random_array);
        free(random_queries);
        
        printf("  %d: Фенвик = %.4fс, ДО = %.4fс\n", repeat_index + 1, fenwick_times[repeat_index], segtree_times[repeat_index]);
    }
    
    double fenwick_total  = 0, segtree_total = 0;
    for (int repeat_index = 0; repeat_index < REPEAT_COUNT; repeat_index++)
    {
        fenwick_total += fenwick_times[repeat_index];
        segtree_total += segtree_times[repeat_index];
    }
    double fenwick_average = fenwick_total / REPEAT_COUNT;
    double segtree_average = segtree_total / REPEAT_COUNT;
    
    printf("\nСреднее: Фенвик = %.4fс, ДО = %.4fс, отношение = %.2fx\n\n", 
           fenwick_average, segtree_average, segtree_average / fenwick_average);
    
    printf("RMQ: Sparse Table\n");
    double st_v1_times[REPEAT_COUNT], st_v2_times[REPEAT_COUNT];
    
    for (int repeat_index = 0; repeat_index < REPEAT_COUNT; repeat_index++)
    {
        long long* random_array = generate_array(ARRAY_SIZE);
        Query* random_queries   = generate_queries(QUERY_COUNT, ARRAY_SIZE);
        
        st_v1_times[repeat_index] = test_st_v1(random_array, ARRAY_SIZE, random_queries, QUERY_COUNT);
        st_v2_times[repeat_index] = test_st_v2(random_array, ARRAY_SIZE, random_queries, QUERY_COUNT);
        
        free(random_array);
        free(random_queries);
        
        printf("  %d: V1 = %.4fс, V2 = %.4fс\n", repeat_index + 1, st_v1_times[repeat_index], st_v2_times[repeat_index]);
    }
    
    double v1_total = 0, v2_total = 0;
    for (int repeat_index = 0; repeat_index < REPEAT_COUNT; repeat_index++)
    {
        v1_total += st_v1_times[repeat_index];
        v2_total += st_v2_times[repeat_index];
    }
    double v1_average = v1_total / REPEAT_COUNT;
    double v2_average = v2_total / REPEAT_COUNT;

     printf("\nСреднее: V1 = %.4fс, V2 = %.4fс, отношение = %.2fx\n\n", 
           v1_average, v2_average, v1_average / v2_average);
    
    return 0;
}
