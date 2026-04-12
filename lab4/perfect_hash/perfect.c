#include "perfect.h"
#include <stdlib.h>
#include <string.h>

static unsigned int hash_int(int key, int a, int b, int size)
{
    return ((unsigned int)(a * key + b) >> 16) % size;
}

static int find_perfect_hash(int* keys, int count, int* a, int* b, int* size)
{
    for (int attempt = 0; attempt < 10000; attempt++)
    {
        *a = rand() % 1000 + 1;
        *b = rand() % 1000;
        *size = count * count;
        
        int* temp = (int*)calloc(*size, sizeof(int));
        for (int i = 0; i < *size; i++) temp[i] = -1;
        
        int ok = 1;
        for (int i = 0; i < count; i++)
        {
            unsigned int h = hash_int(keys[i], *a, *b, *size);
            if (temp[h] != -1)
            {
                ok = 0;
                break;
            }
            temp[h] = keys[i];
        }
        free(temp);
        if (ok) return 1;
    }
    return 0;
}

PerfectHashTable* perfect_create(int* keys, int count)
{
    PerfectHashTable* ht = (PerfectHashTable*)calloc(1, sizeof(PerfectHashTable));
    ht->size = count;
    ht->hash_params = (int*)calloc(2 * count, sizeof(int));
    ht->table = (PerfectSecondary*)calloc(count, sizeof(PerfectSecondary));
    
    for (int i = 0; i < count; i++)
    {
        ht->table[i].size = 0;
        ht->table[i].keys = NULL;
    }
    
    int* first_level = (int*)calloc(count, sizeof(int));
    
    for (int i = 0; i < count; i++)
    {
        unsigned int h = hash_int(keys[i], 100, 50, count);
        first_level[h]++;
    }
    
    for (int i = 0; i < count; i++)
    {
        if (first_level[i] > 0)
        {
            int* bucket_keys = (int*)calloc(first_level[i], sizeof(int));
            int idx = 0;
            for (int j = 0; j < count; j++)
            {
                unsigned int h = hash_int(keys[j], 100, 50, count);
                if (h == i) bucket_keys[idx++] = keys[j];
            }
            
            int a = 0, b = 0, size = 0;
            if (find_perfect_hash(bucket_keys, first_level[i], &a, &b, &size))
            {
                ht->table[i].size = size;
                ht->table[i].keys = (int*)calloc(size, sizeof(int));
                for (int j = 0; j < size; j++) ht->table[i].keys[j] = -1;
                
                for (int j = 0; j < first_level[i]; j++)
                {
                    unsigned int h2 = hash_int(bucket_keys[j], a, b, size);
                    ht->table[i].keys[h2] = bucket_keys[j];
                }
                ht->hash_params[2*i]     = a;
                ht->hash_params[2*i + 1] = b;
            }
            free(bucket_keys);
        }
    }
    
    free(first_level);
    return ht;
}

int perfect_search(PerfectHashTable* ht, int key)
{
    unsigned int h1 = hash_int(key, 100, 50, ht->size);
    if (ht->table[h1].size == 0) return 0;
    
    int a = ht->hash_params[2*h1];
    int b = ht->hash_params[2*h1 + 1];
    unsigned int h2 = hash_int(key, a, b, ht->table[h1].size);
    
    return (ht->table[h1].keys[h2] == key);
}

void perfect_destroy(PerfectHashTable* ht)
{
    for (int i = 0; i < ht->size; i++)
    {
        if (ht->table[i].keys) free(ht->table[i].keys);
    }
    free(ht->table);
    free(ht->hash_params);
    free(ht);
}