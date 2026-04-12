#include "linear.h"
#include <stdlib.h>

static unsigned int hash_int(int key, int size)
{
    return (unsigned int)key % size;
}

LinearHashTable* linear_create(int initial_size, float threshold)
{
    LinearHashTable* ht = (LinearHashTable*)calloc(1, sizeof(LinearHashTable));
    ht->size  = initial_size;
    ht->count = 0;
    ht->load_factor_threshold = threshold;
    ht->keys   = (int*)calloc(initial_size, sizeof(int));
    ht->status = (int*)calloc(initial_size, sizeof(int));
    return ht;
}

static void linear_rehash(LinearHashTable* ht)
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
        if (old_status[i] == 1) linear_insert(ht, old_keys[i]);
    }
    free(old_keys);
    free(old_status);
}

void linear_insert(LinearHashTable* ht, int key)
{
    if ((float)ht->count / ht->size >= ht->load_factor_threshold)
    {
        linear_rehash(ht);
    }
    
    unsigned int index = hash_int(key, ht->size);
    int i = 0;
    
    while (ht->status[(index + i) % ht->size] == 1)
    {
        if (ht->keys[(index + i) % ht->size] == key) return;
        i++;
    }
    
    int pos = (index + i) % ht->size;
    ht->keys[pos]   = key;
    ht->status[pos] = 1;
    ht->count++;
}

int linear_search(LinearHashTable* ht, int key)
{
    unsigned int index = hash_int(key, ht->size);
    int i = 0;
    
    while (i < ht->size)
    {
        int pos = (index + i) % ht->size;
        if (ht->status[pos] == 0) return 0;
        if (ht->status[pos] == 1 && ht->keys[pos] == key) return 1;
        i++;
    }
    return 0;
}

int linear_delete(LinearHashTable* ht, int key)
{
    unsigned int index = hash_int(key, ht->size);
    int i = 0;
    
    while (i < ht->size)
    {
        int pos = (index + i) % ht->size;
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

void linear_destroy(LinearHashTable* ht)
{
    free(ht->keys);
    free(ht->status);
    free(ht);
}