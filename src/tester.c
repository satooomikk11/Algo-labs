#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/fenwick_tree.h"
#include "../include/segment_tree.h"
#include "../include/sparse_table.h"

#define N 1000000
#define Q 1000000
#define REPEATS 5

long long* generate_array(int n)
{
    long long* a = (long long*)calloc(n, sizeof(long long));
    for (int i = 0; i < n; i++)
    {
        a[i] = rand() % 1000000;
    }
    return a;
}

typedef struct
{
    int l, r;
} Query;

Query* generate_queries(int q, int n)
{
    Query* queries = (Query*)calloc(q, sizeof(Query));
    for (int i = 0; i < q; i++)
    {
        queries[i].l = rand() % n;
        queries[i].r = rand() % n;
        if (queries[i].l > queries[i].r)
        {
            int tmp = queries[i].l;
                      queries[i].l = queries[i].r;
                                     queries[i].r = tmp;
        }
    }
    return queries;
}

double test_fenwick(long long* a, int n, Query* qs, int q)
{
    FenwickTree* ft = fenwick_create(n);
    fenwick_build_prefix(ft, a, n);
    
    clock_t start = clock();
    for (int i = 0; i < q; i++)
    {
        fenwick_range_sum(ft, qs[i].l, qs[i].r);
    }
    clock_t end = clock();
    
    fenwick_free(ft);
    return (double)(end - start) / CLOCKS_PER_SEC;
}

double test_segtree(long long* a, int n, Query* qs, int q)
{
    SegmentTree* st = segtree_create(n);
    segtree_build(st, a, n);
    
    clock_t start = clock();
    for (int i = 0; i < q; i++)
    {
        segtree_query(st, qs[i].l, qs[i].r);
    }
    clock_t end = clock();
    
    segtree_free(st);
    return (double)(end - start) / CLOCKS_PER_SEC;
}

double test_st_v1(long long* a, int n, Query* qs, int q)
{
    SparseTable* st = st_create_v1(a, n);
    
    clock_t start = clock();
    for (int i = 0; i < q; i++)
    {
        st_query(st, qs[i].l, qs[i].r);
    }
    clock_t end = clock();
    
    st_free(st);
    return (double)(end - start) / CLOCKS_PER_SEC;
}

double test_st_v2(long long* a, int n, Query* qs, int q)
{
    SparseTable* st = st_create_v2(a, n);
    
    clock_t start = clock();
    for (int i = 0; i < q; i++)
    {
        st_query(st, qs[i].l, qs[i].r);
    }
    clock_t end = clock();
    
    st_free(st);
    return (double)(end - start) / CLOCKS_PER_SEC;
}

int main()
{
    srand(time(NULL));
    
    printf("\nЛабораторная работа 7. Запросы на отрезках\n\n");
    
    printf("RSQ: Фенвик и дерево отрезков\n");
    double fenwick_times[REPEATS], segtree_times[REPEATS];
    
    for (int r = 0; r < REPEATS; r++)
    {
        long long* a = generate_array(N);
        Query* qs = generate_queries(Q, N);
        
        fenwick_times[r] = test_fenwick(a, N, qs, Q);
        segtree_times[r] = test_segtree(a, N, qs, Q);
        
        free(a);
        free(qs);
        
        printf("  %d: Фенвик = %.4fс, ДО = %.4fс\n", r + 1, fenwick_times[r], segtree_times[r]);
    }
    
    double avg_fenwick = 0, avg_segtree = 0;
    for (int i = 0; i < REPEATS; i++)
    {
        avg_fenwick += fenwick_times[i];
        avg_segtree += segtree_times[i];
    }
    avg_fenwick /= REPEATS;
    avg_segtree /= REPEATS;
    
    printf("\nСреднее: Фенвик = %.4fс, ДО = %.4fс, отношение = %.2fx\n\n", 
           avg_fenwick, avg_segtree, avg_segtree / avg_fenwick);
    

    printf("RMQ: Sparse Table\n");
    double st_v1_times[REPEATS], st_v2_times[REPEATS];
    
    for (int r = 0; r < REPEATS; r++)
    {
        long long* a  = generate_array(N);
        Query*     qs = generate_queries(Q, N);
        
        st_v1_times[r] = test_st_v1(a, N, qs, Q);
        st_v2_times[r] = test_st_v2(a, N, qs, Q);
        
        free(a);
        free(qs);
        
        printf("  %d: V1 = %.4fс, V2 = %.4fс\n", r + 1, st_v1_times[r], st_v2_times[r]);
    }
    
    double avg_st_v1 = 0, avg_st_v2 = 0;
    for (int i = 0; i < REPEATS; i++)
    {
        avg_st_v1 += st_v1_times[i];
        avg_st_v2 += st_v2_times[i];
    }
    avg_st_v1 /= REPEATS;
    avg_st_v2 /= REPEATS;
    
    printf("\nСреднее: V1 = %.4fс, V2 = %.4fс, отношение = %.2fx\n\n", 
           avg_st_v1, avg_st_v2, avg_st_v1 / avg_st_v2);
    
    return 0;
}
