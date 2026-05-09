#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "../hash_tables/chaining.h"
#include "../hash_tables/linear.h"
#include "../hash_tables/quadratic.h"
#include "../hash_tables/double_hash.h"
#include "../hash_tables/cuckoo.h"

#define MIN_SIZE 10000
#define MAX_SIZE 1000000
#define STEP 10000

static double test_table(void *table, int size, int* ops, int* keys_op, int type)
{
    if (!ops || !keys_op) return 0;
    
    double total_time = 0;
    int repeats = 1;
    
    if (size < 100000)
    {
        repeats = 10;
    }
    
    for (int r = 0; r < repeats; r++)
    {
        clock_t start, end;
        
        if (type == 0)
        {
            ChainingHashTable* ht = chaining_create(1000, 0.7);
            if (!ht) continue;
            
            start = clock();
            for (int i = 0; i < size; i++)
            {
                if      (ops[i] == 0) chaining_insert(ht, keys_op[i]);
                else if (ops[i] == 1) chaining_search(ht, keys_op[i]);
                else                  chaining_delete(ht, keys_op[i]);
            }
            end = clock();
            total_time += (double)(end - start) / CLOCKS_PER_SEC * 1000;
            chaining_destroy(ht);
        }
        else if (type == 1)
        {
            LinearHashTable* ht = linear_create(1000, 0.7);
            if (!ht) continue;
            
            start = clock();
            for (int i = 0; i < size; i++)
            {
                if      (ops[i] == 0) linear_insert(ht, keys_op[i]);
                else if (ops[i] == 1) linear_search(ht, keys_op[i]);
                else                  linear_delete(ht, keys_op[i]);
            }
            end = clock();
            total_time += (double)(end - start) / CLOCKS_PER_SEC * 1000;
            linear_destroy(ht);
        }
        else if (type == 2)
        {
            QuadraticHashTable* ht = quadratic_create(1000, 0.7);
            if (!ht) continue;
            
            start = clock();
            for (int i = 0; i < size; i++)
            {
                if      (ops[i] == 0) quadratic_insert(ht, keys_op[i]);
                else if (ops[i] == 1) quadratic_search(ht, keys_op[i]);
                else                  quadratic_delete(ht, keys_op[i]);
            }
            end = clock();
            total_time += (double)(end - start) / CLOCKS_PER_SEC * 1000;
            quadratic_destroy(ht);
        }
        else if (type == 3)
        {
            DoubleHashTable* ht = double_create(1000, 0.7);
            if (!ht) continue;
            
            start = clock();
            for (int i = 0; i < size; i++)
            {
                if      (ops[i] == 0) double_insert(ht, keys_op[i]);
                else if (ops[i] == 1) double_search(ht, keys_op[i]);
                else                  double_delete(ht, keys_op[i]);
            }
            end = clock();
            total_time += (double)(end - start) / CLOCKS_PER_SEC * 1000;
            double_destroy(ht);
        }
        else if (type == 4)
        {
            CuckooHashTable* ht = cuckoo_create(2000, 0.5);
            if (!ht) continue;
            
            start = clock();
            for (int i = 0; i < size; i++)
            {
                if      (ops[i] == 0) cuckoo_insert(ht, keys_op[i]);
                else if (ops[i] == 1) cuckoo_search(ht, keys_op[i]);
                else                  cuckoo_delete(ht, keys_op[i]);
            }
            end = clock();
            total_time += (double)(end - start) / CLOCKS_PER_SEC * 1000;
            cuckoo_destroy(ht);
        }
    }
    
    return total_time / repeats;
}

void test_hash_tables_operations() 
{
    printf("\nMIXED OPERATIONS (insert=0.5, search=0.25, delete=0.25)\n");
    srand(42);
    
    int num_sizes = (MAX_SIZE - MIN_SIZE) / STEP + 1;
    int* sizes = (int*)calloc(num_sizes, sizeof(int));
    if (!sizes) return;
    
    for (int i = 0; i < num_sizes; i++)
    {
        sizes[i] = MIN_SIZE + i * STEP;
    }
    
    float probs[] = {0.5, 0.25, 0.25};
    
    FILE* out = fopen("results/tables/operations_results.csv", "w");
    if (!out)
    {
        free(sizes);
        return;
    }
    fprintf(out, "size,chaining,linear,quadratic,double,cuckoo\n");
    
    for (int s_idx = 0; s_idx < num_sizes; s_idx++)
    {
        int size = sizes[s_idx];
        
        int* ops     = (int*)calloc(size, sizeof(int));
        int* keys_op = (int*)calloc(size, sizeof(int));
        
        if (!ops || !keys_op)
        {
            free(ops);
            free(keys_op);
            continue;
        }
        
        for (int i = 0; i < size; i++)
        {
            double r = rand() / (double)RAND_MAX;
            if      (r < probs[0])            ops[i] = 0;
            else if (r < probs[0] + probs[1]) ops[i] = 1;
            else                              ops[i] = 2;
            keys_op[i] = rand();
        }
        
        fprintf(out, "%d", size);
        
        double t = test_table(NULL, size, ops, keys_op, 0);
        fprintf(out, ",%.2f", t);
        
        t = test_table(NULL, size, ops, keys_op, 1);
        fprintf(out, ",%.2f", t);
        
        t = test_table(NULL, size, ops, keys_op, 2);
        fprintf(out, ",%.2f", t);
        
        t = test_table(NULL, size, ops, keys_op, 3);
        fprintf(out, ",%.2f", t);
        
        if (size <= 500000)
        {
            t = test_table(NULL, size, ops, keys_op, 4);
            fprintf(out, ",%.2f\n", t);
        }
        else
        {
            fprintf(out, ",N/A\n");
        }
        
        free(ops);
        free(keys_op);
    }
    
    fclose(out);
    free(sizes);
    printf("Saved: results/tables/operations_results.csv\n");
}

void test_hash_tables_operations_uniform()
{
    printf("\nUNIFORM OPERATIONS (insert=33%%, search=33%%, delete=33%%)\n");
    srand(42);
    
    int num_sizes = (MAX_SIZE - MIN_SIZE) / STEP + 1;
    int* sizes = (int*)calloc(num_sizes, sizeof(int));
    if (!sizes) return;
    
    for (int i = 0; i < num_sizes; i++)
    {
        sizes[i] = MIN_SIZE + i * STEP;
    }
    
    float probs[] = {0.3333, 0.3333, 0.3334};
    
    FILE* out = fopen("results/tables/operations_uniform_results.csv", "w");
    if (!out)
    {
        free(sizes);
        return;
    }
    fprintf(out, "size,chaining,linear,quadratic,double,cuckoo\n");
    
    for (int s_idx = 0; s_idx < num_sizes; s_idx++)
    {
        int size = sizes[s_idx];
        
        int* ops     = (int*)calloc(size, sizeof(int));
        int* keys_op = (int*)calloc(size, sizeof(int));
        
        if (!ops || !keys_op)
        {
            free(ops);
            free(keys_op);
            continue;
        }
        
        for (int i = 0; i < size; i++)
        {
            double r = rand() / (double)RAND_MAX;
            if      (r < probs[0])            ops[i] = 0;
            else if (r < probs[0] + probs[1]) ops[i] = 1;
            else                              ops[i] = 2;
            keys_op[i] = rand();
        }
        
        fprintf(out, "%d", size);
        
        double t = test_table(NULL, size, ops, keys_op, 0);
        fprintf(out, ",%.2f", t);
        
        t = test_table(NULL, size, ops, keys_op, 1);
        fprintf(out, ",%.2f", t);
        
        t = test_table(NULL, size, ops, keys_op, 2);
        fprintf(out, ",%.2f", t);
        
        t = test_table(NULL, size, ops, keys_op, 3);
        fprintf(out, ",%.2f", t);
        
        if (size <= 500000)
        {
            t = test_table(NULL, size, ops, keys_op, 4);
            fprintf(out, ",%.2f\n", t);
        }
        else
        {
            fprintf(out, ",N/A\n");
        }
        
        free(ops);
        free(keys_op);
    }
    
    fclose(out);
    free(sizes);
    printf("Saved: results/tables/operations_uniform_results.csv\n");
}
