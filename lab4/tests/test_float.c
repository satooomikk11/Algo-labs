#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../hash_functions/float_hash.h"

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

void test_float_hashes()
{
    printf("\nFLOAT HASH FUNCTIONS\n");
    
    FILE* f = fopen("data/float_keys.bin", "rb");
    if (!f)
    {
        printf("Error: data/float_keys.bin not found\n");
        return;
    }
    
    float* keys = calloc(N, sizeof(float));
    if (!keys)
    {
        fclose(f);
        return;
    }

    size_t read = fread(keys, sizeof(unsigned int), N, f);
    (void)read; // подавление warning
    fclose(f);
    
    struct
    {
        const char* name;
        uint32_t (*func)(float);
        const char* filename;
    }
    tests[] =
    {
        {"To int bits", hash_float_to_int_bits,          "results/functions/float_tobits.csv"  },
        {"Bitwise",     hash_float_bitwise,              "results/functions/float_bitwise.csv" },
        {"Mantissa",    hash_float_mantissa,             "results/functions/float_mantissa.csv"},
        {"Exponent",    hash_float_exponent,             "results/functions/float_exponent.csv"},
        {"Mant*Exp",    hash_float_mantissa_exp_product, "results/functions/float_mantexp.csv" }
    };
    
    for (int t = 0; t < 5; t++)
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