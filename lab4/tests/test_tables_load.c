#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../hash_tables/chaining.h"
#include "../hash_tables/linear.h"
#include "../hash_tables/quadratic.h"
#include "../hash_tables/double_hash.h"
#include "../hash_tables/cuckoo.h"

#define N 1000000

void test_hash_tables_load_factor()
{
    printf("\nHASH TABLES LOAD FACTOR TEST\n");
    
    FILE* f = fopen("data/int_keys.bin", "rb");
    if (!f)
    {
        printf("Error: data/int_keys.bin not found\n");
        return;
    }
    
    int* keys = calloc(N, sizeof(int));
    if (!keys)
    {
        fclose(f);
        return;
    }

    size_t read = fread(keys, sizeof(int), N, f);
    (void)read;
    fclose(f);
    
    float load_factors[] = {0.4, 0.6, 0.7, 0.8, 0.9};
    
    FILE* out = fopen("results/tables/load_factor_results.csv", "w");
    fprintf(out, "load_factor,chaining,linear,quadratic,double,cuckoo\n");
    
    printf("Load factor comparison:\n");
    printf("LF\tChaining\tLinear\tQuadratic\tDouble\tCuckoo\n");
    
    for (int lf_idx = 0; lf_idx < 5; lf_idx++)
    {
        float lf = load_factors[lf_idx];
        printf("%.1f\t", lf);
        fprintf(out, "%.1f", lf);
        
        // цепочки
        ChainingHashTable* ch = chaining_create(1000, lf);
        clock_t start = clock();
        for (int i = 0; i < N; i++) chaining_insert(ch, keys[i]);
        clock_t end = clock();
        double t = (double)(end - start) / CLOCKS_PER_SEC * 1000;
        printf("%.0f\t\t", t);
        fprintf(out, ",%.0f", t);
        chaining_destroy(ch);
        
        // линейное
        LinearHashTable* lin = linear_create(1000, lf);
        start = clock();
        for (int i = 0; i < N; i++) linear_insert(lin, keys[i]);
        end = clock();
        t = (double)(end - start) / CLOCKS_PER_SEC * 1000;
        printf("%.0f\t\t", t);
        fprintf(out, ",%.0f", t);
        linear_destroy(lin);
        
        // квалратичное
        QuadraticHashTable* quad = quadratic_create(1000, lf);
        start = clock();
        for (int i = 0; i < N; i++) quadratic_insert(quad, keys[i]);
        end = clock();
        t = (double)(end - start) / CLOCKS_PER_SEC * 1000;
        printf("%.0f\t\t", t);
        fprintf(out, ",%.0f", t);
        quadratic_destroy(quad);
        
        // двойное
        DoubleHashTable* dbl = double_create(1000, lf);
        start = clock();
        for (int i = 0; i < N; i++) double_insert(dbl, keys[i]);
        end = clock();
        t = (double)(end - start) / CLOCKS_PER_SEC * 1000;
        printf("%.0f\t\t", t);
        fprintf(out, ",%.0f", t);
        double_destroy(dbl);
        
        // кукушка (lf <= 0.7)
        if (lf <= 0.7)
        {
            CuckooHashTable* cuck = cuckoo_create(2000, lf);
            start = clock();
            for (int i = 0; i < N; i++) cuckoo_insert(cuck, keys[i]);
            end = clock();
            t = (double)(end - start) / CLOCKS_PER_SEC * 1000;
            printf("%.0f\n", t);
            fprintf(out, ",%.0f\n", t);
            cuckoo_destroy(cuck);
        }
        else
        {
            printf("N/A\n");
            fprintf(out, ",N/A\n");
        }
    }
    
    fclose(out);
    free(keys);
}