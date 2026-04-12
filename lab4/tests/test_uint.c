#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../hash_functions/uint_hash.h"

#define M 1000
#define N 1000000

static void save_histogram(const char* filename, int* buckets, int size)
{
    FILE* f = fopen(filename, "w");
    if (!f) return;
    fprintf(f, "bucket,count\n");
    for (int i = 0; i < size; i++)
    {
        fprintf(f, "%d,%d\n", i, buckets[i]);
    }
    fclose(f);
}

void test_uint_hashes()
{
    printf("\nUINT HASH FUNCTIONS\n");
    
    FILE* f = fopen("data/uint_keys.bin", "rb");
    if (!f)
    {
        printf("Error: data/uint_keys.bin not found\n");
        return;
    }
    
    unsigned int* keys = calloc(N, sizeof(unsigned int));
    size_t read = fread(keys, sizeof(unsigned int), N, f);
    (void)read; // подавление warning
    fclose(f);
    
    struct
    {
        const char* name;
        uint32_t (*func)(uint32_t);
        const char* filename;
    }
    tests[] =
    {
        {"Modulo",         hash_uint_mod,            "results/functions/uint_mod.csv"    },
        {"Bitwise",        hash_uint_bitwise,        "results/functions/uint_bitwise.csv"},
        {"Knuth multiply", hash_uint_multiply_knuth, "results/functions/uint_knuth.csv"  }
    };
    
    for (int t = 0; t < 3; t++)
    {
        int buckets[M] = {0};
        clock_t start  = clock();
        
        for (int i = 0; i < N; i++)
        {
            uint32_t h = tests[t].func(keys[i]);
            buckets[h % M]++;
        }
        
        clock_t end    = clock();
        double time_ms = (double)(end - start) / CLOCKS_PER_SEC * 1000;
        
        double mean = (double)N / M;
        double variance = 0;
        for (int i = 0; i < M; i++)
        {
            variance += (buckets[i] - mean) * (buckets[i] - mean);
        }
        variance /= M;
        
        printf("%-15s: time = %8.2f, variance = %10.2f\n",
               tests[t].name, time_ms, variance);
        
        save_histogram(tests[t].filename, buckets, M);
    }
    
    free(keys);
}