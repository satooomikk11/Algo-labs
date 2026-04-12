#include "double_hash.h"
#include <stdlib.h>

static unsigned int hash1(int key, int size)
{
    return (unsigned int)key % size;
}

static unsigned int hash2(int key)
{
    return 1 + (key % 997);
}

DoubleHashTable* double_create(int initial_size, float threshold)
{
    DoubleHashTable* ht = (DoubleHashTable*)calloc(1, sizeof(DoubleHashTable));
    ht->size  = initial_size;
    ht->count = 0;
    ht->load_factor_threshold = threshold;
    ht->keys   = (int*)calloc(initial_size, sizeof(int));
    ht->status = (int*)calloc(initial_size, sizeof(int));
    return ht;
}

static void double_rehash(DoubleHashTable* ht)
{
    int  old_size   = ht->size;
    int* old_keys   = ht->keys;
    int* old_status = ht->status;
    
    ht->size  *= 2;
    ht->keys   = (int*)calloc(ht->size, sizeof(int));
    ht->status = (int*)calloc(ht->size, sizeof(int));
    ht->count  = 0;
    
    for (int i = 0; i < old_size; i++)
    {
        if (old_status[i] == 1) double_insert(ht, old_keys[i]);
    }
    free(old_keys);
    free(old_status);
}

void double_insert(DoubleHashTable* ht, int key)
{
    if ((float)ht->count / ht->size >= ht->load_factor_threshold)
    {
        double_rehash(ht);
    }
    
    unsigned int h1 = hash1(key, ht->size);
    unsigned int h2 = hash2(key);
    int i = 0;
    
    while (ht->status[(h1 + i * h2) % ht->size] == 1)
    {
        if (ht->keys[(h1 + i * h2) % ht->size] == key) return;
        i++;
    }
    
    int pos = (h1 + i * h2) % ht->size;
    ht->keys[pos]   = key;
    ht->status[pos] = 1;
    ht->count++;
}

int double_search(DoubleHashTable* ht, int key)
{
    unsigned int h1 = hash1(key, ht->size);
    unsigned int h2 = hash2(key);
    int i = 0;
    
    while (i < ht->size)
    {
        int pos = (h1 + i * h2) % ht->size;
        if (ht->status[pos] == 0) return 0;
        if (ht->status[pos] == 1 && ht->keys[pos] == key) return 1;
        i++;
    }
    return 0;
}

int double_delete(DoubleHashTable* ht, int key)
{
    unsigned int h1 = hash1(key, ht->size);
    unsigned int h2 = hash2(key);
    int i = 0;
    
    while (i < ht->size)
    {
        int pos = (h1 + i * h2) % ht->size;
        if (ht->status[pos] == 0) return 0;
        if (ht->status[pos] == 1 && ht->keys[pos] == key)
        {
            ht->status[pos] = -1;
            ht->count--;
            return 1;
        }
        i++;
    }
    return 0;
}

void double_destroy(DoubleHashTable* ht)
{
    free(ht->keys);
    free(ht->status);
    free(ht);
}