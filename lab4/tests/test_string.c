#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../hash_functions/string_hash.h"

#define M 1000
#define N 1000000

static void save_histogram(const char* filename, int* buckets, int size)
{
    FILE* f = fopen(filename, "w");
    if (!f)
    {
        printf("ERROR: Cannot open %s for writing\n", filename);
        return;
    }
    fprintf(f, "bucket,count\n");
    for (int i = 0; i < size; i++)
    {
        fprintf(f, "%d,%d\n", i, buckets[i]);
    }
    fclose(f);
    printf("Saved: %s\n", filename);
}

void test_string_hashes()
{
    printf("\nSTRING HASH FUNCTIONS\n");
    
    FILE* f = fopen("data/string_keys.txt", "r");
    if (!f)
    {
        printf("Error: data/string_keys.txt not found\n");
        return;
    }
    
    char** strings = calloc(N, sizeof(char*));
    if (!strings)
    {
        fclose(f);
        return;
    }
    
    char buffer[100];
    for (int i = 0; i < N; i++)
    {
        int scanned = fscanf(f, "%s", buffer);
        (void)scanned;
        strings[i] = strdup(buffer);
    }
    fclose(f);
    
    struct
    {
        const char* name;
        uint32_t (*func)(const char*);
        const char* filename;
    }
    tests[] =
    {
        {"Length",     hash_string_len,        "results/functions/string_len.csv"  },
        {"Sum",        hash_string_sum,        "results/functions/string_sum.csv"  },
        {"Polynomial", hash_string_polynomial, "results/functions/string_poly.csv" },
        {"crc32",      hash_string_crc32,      "results/functions/string_crc32.csv"}
    };
    
    for (int t = 0; t < 4; t++)
    {
        int buckets[M] = {0};
        clock_t start  = clock();
        
        for (int i = 0; i < N; i++)
        {
            uint32_t h = tests[t].func(strings[i]);
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
    
    for (int i = 0; i < N; i++) free(strings[i]);
    free(strings);
}