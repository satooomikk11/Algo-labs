#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../hash_tables/chaining.h"
#include "../hash_tables/quadratic.h"
#include "../hash_tables/cuckoo.h"
#include "../perfect_hash/perfect.h"

void test_perfect_hashing()
{
    printf("\nPERFECT HASHING COMPARISON\n");
    srand(42);
    
    const int N_KEYS   = 100000;
    const int N_SEARCH = 10000000;
    
    int* keys = calloc (N_KEYS, sizeof(int));
    for (int i = 0; i < N_KEYS; i++) keys[i] = rand();
    
    FILE* out = fopen("results/tables/perfect_hash_results.csv", "w");
    fprintf(out, "table_type,build_ms,search_ms\n");
    
    // цепочки
    ChainingHashTable* ch = chaining_create(10000, 0.7);
    clock_t start = clock();
    for (int i = 0; i < N_KEYS; i++) chaining_insert(ch, keys[i]);
    clock_t end = clock();
    double build_ch = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    
    start = clock();
    for (int i = 0; i < N_SEARCH; i++) chaining_search(ch, keys[i % N_KEYS]);
    end = clock();
    double search_ch = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    
    // квадратичное хеширование
    QuadraticHashTable* quad = quadratic_create(10000, 0.7);
    start = clock();
    for (int i = 0; i < N_KEYS; i++) quadratic_insert(quad, keys[i]);
    end = clock();
    double build_quad = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    
    start = clock();
    for (int i = 0; i < N_SEARCH; i++) quadratic_search(quad, keys[i % N_KEYS]);
    end = clock();
    double search_quad = (double)(end - start) / CLOCKS_PER_SEC * 1000;

    // кукушка
    CuckooHashTable* cuck = cuckoo_create(20000, 0.5);
    start = clock();
    for (int i = 0; i < N_KEYS; i++) cuckoo_insert(cuck, keys[i]);
    end = clock();
    double build_cuck = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    
    start = clock();
    for (int i = 0; i < N_SEARCH; i++) cuckoo_search(cuck, keys[i % N_KEYS]);
    end = clock();
    double search_cuck = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    
    // идеальное хеширование
    start = clock();
    PerfectHashTable* perf = perfect_create(keys, N_KEYS);
    end = clock();
    double build_perf = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    
    start = clock();
    for (int i = 0; i < N_SEARCH; i++) perfect_search(perf, keys[i % N_KEYS]);
    end = clock();
    double search_perf = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    
    printf("\nTable type\t\tBuild(ms)\tSearch(ms)\n");
    printf("Chaining\t\t%.0f\t\t%.0f\n",        build_ch,   search_ch  );
    printf("Quadratic\t\t%.0f\t\t%.0f\n",       build_quad, search_quad);
    printf("Cuckoo\t\t\t%.0f\t\t%.0f\n",        build_cuck, search_cuck);
    printf("Perfect hashing\t\t%.0f\t\t%.0f\n", build_perf, search_perf);
    
    fprintf(out, "chaining,%.0f,%.0f\n",        build_ch,   search_ch  );
    fprintf(out, "quadratic,%.0f,%.0f\n",       build_quad, search_quad);
    fprintf(out, "cuckoo,%.0f,%.0f\n",          build_cuck, search_cuck);
    fprintf(out, "perfect_hashing,%.0f,%.0f\n", build_perf, search_perf);
    fclose(out);
    
    chaining_destroy(ch);
    double_destroy  (quad);
    cuckoo_destroy  (cuck);
    perfect_destroy (perf);
    free(keys);
}