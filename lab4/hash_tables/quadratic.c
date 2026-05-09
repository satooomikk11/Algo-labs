#include "quadratic.h"
#include <stdlib.h>

static unsigned int hash_int(int key, int size)
{
    return (unsigned int)key % size;
}

QuadraticHashTable* quadratic_create(int initial_size, float threshold)
{
    QuadraticHashTable* ht = (QuadraticHashTable*)calloc(1, sizeof(QuadraticHashTable));
    if (!ht) return NULL;
    
    ht->size  = initial_size;
    ht->count = 0;
    ht->load_factor_threshold = threshold;
    ht->keys   = (int*)calloc(initial_size, sizeof(int));
    ht->status = (CellStatus*)calloc(initial_size, sizeof(CellStatus));
    
    if (!ht->keys || !ht->status)
    {
        free(ht->keys);
        free(ht->status);
        free(ht);
        return NULL;
    }
    
    return ht;
}

static void quadratic_rehash(QuadraticHashTable* ht)
{
    if (!ht) return;
    
    int  old_size = ht->size;
    int* old_keys = ht->keys;
    CellStatus* old_status = ht->status;
    
    ht->size  *= 2;
    ht->keys   = (int*)calloc(ht->size, sizeof(int));
    ht->status = (CellStatus*)calloc(ht->size, sizeof(CellStatus));
    
    if (!ht->keys || !ht->status)
    {
        ht->keys   = old_keys;
        ht->status = old_status;
        ht->size   = old_size;
        return;
    }
    
    ht->count = 0;
    
    for (int i = 0; i < old_size; i++)
    {
        if (old_status[i] == CELL_OCCUPIED)
        {
            quadratic_insert(ht, old_keys[i]);
        }
    }
    free(old_keys);
    free(old_status);
}

void quadratic_insert(QuadraticHashTable* ht, int key)
{
    if (!ht) return;
    
    if ((float)ht->count / ht->size >= ht->load_factor_threshold)
    {
        quadratic_rehash(ht);
    }
    
    unsigned int index = hash_int(key, ht->size);
    int i = 0;
    
    while (ht->status[(index + i*i) % ht->size] == CELL_OCCUPIED)
    {
        if (ht->keys[(index + i*i) % ht->size] == key) return;
        i++;
    }
    
    int pos = (index + i*i) % ht->size;
    ht->keys[pos]   = key;
    ht->status[pos] = CELL_OCCUPIED;
    ht->count++;
}

int quadratic_search(QuadraticHashTable* ht, int key)
{
    if (!ht) return 0;
    
    unsigned int index = hash_int(key, ht->size);
    int i = 0;
    
    while (i < ht->size)
    {
        int pos = (index + i*i) % ht->size;
        if (ht->status[pos] == CELL_EMPTY) return 0;
        if (ht->status[pos] == CELL_OCCUPIED && ht->keys[pos] == key) return 1;
        i++;
    }
    return 0;
}

int quadratic_delete(QuadraticHashTable* ht, int key)
{
    if (!ht) return 0;
    
    unsigned int index = hash_int(key, ht->size);
    int i = 0;
    
    while (i < ht->size)
    {
        int pos = (index + i*i) % ht->size;
        if (ht->status[pos] == CELL_EMPTY) return 0;
        if (ht->status[pos] == CELL_OCCUPIED && ht->keys[pos] == key)
        {
            ht->status[pos] = CELL_DELETED;
            ht->count--;
            return 1;
        }
        i++;
    }
    return 0;
}

void quadratic_destroy(QuadraticHashTable* ht)
{
    if (!ht) return;
    
    free(ht->keys);
    free(ht->status);
    free(ht);
}
