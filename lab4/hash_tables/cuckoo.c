#include "cuckoo.h"
#include <stdlib.h>
#include <stdio.h>

static unsigned int hash1(int key, int size)
{
    return (unsigned int)key % size;
}

static unsigned int hash2(int key, int size)
{
    return ((unsigned int)key >> 16) % size;
}

CuckooHashTable* cuckoo_create(int initial_size, float threshold)
{
    CuckooHashTable* ht = (CuckooHashTable*)calloc(1, sizeof(CuckooHashTable));
    if (!ht) return NULL;
    
    ht->size  = initial_size;
    ht->count = 0;
    ht->load_factor_threshold = threshold;
    ht->table1 = (int*)calloc(initial_size, sizeof(int));
    ht->table2 = (int*)calloc(initial_size, sizeof(int));

    if (!ht->table1 || !ht->table2)
    {
        free(ht->table1);
        free(ht->table2);
        free(ht);
        return NULL;
    }
    
    for (int i = 0; i < initial_size; i++)
    {
        ht->table1[i] = -1;
        ht->table2[i] = -1;
    }
    return ht;
}

static void cuckoo_rehash(CuckooHashTable* ht)
{
    if (!ht) return;
    
    int  old_size   = ht->size;
    int* old_table1 = ht->table1;
    int* old_table2 = ht->table2;
    
    ht->size  *= 2;
    ht->count  = 0;
    ht->table1 = (int*)calloc(ht->size, sizeof(int));
    ht->table2 = (int*)calloc(ht->size, sizeof(int));
    
    if (!ht->table1 || !ht->table2)
    {
        ht->table1 = old_table1;
        ht->table2 = old_table2;
        ht->size   = old_size;
        return;
    }
    
    for (int i = 0; i < ht->size; i++)
    {
        ht->table1[i] = -1;
        ht->table2[i] = -1;
    }
    
    for (int i = 0; i < old_size; i++)
    {
        if (old_table1[i] != -1) cuckoo_insert(ht, old_table1[i]);
        if (old_table2[i] != -1) cuckoo_insert(ht, old_table2[i]);
    }
    
    free(old_table1);
    free(old_table2);
}

void cuckoo_insert(CuckooHashTable* ht, int key)
{
    if (!ht) return;
    
    if ((float)ht->count / (2 * ht->size) >= ht->load_factor_threshold)
    {
        cuckoo_rehash(ht);
    }
    
    if (cuckoo_search(ht, key)) return;
    
    int tmp_key    = key;
    int loop_count = 0;
    const int MAX_LOOPS = 100;
    
    while (loop_count < MAX_LOOPS)
    {
        unsigned int pos1 = hash1(tmp_key, ht->size);
        if (ht->table1[pos1] == -1)
        {
            ht->table1[pos1] = tmp_key;
            ht->count++;
            return;
        }
        
        int evicted = ht->table1[pos1];
        ht->table1[pos1] = tmp_key;
                           tmp_key = evicted;
        
        unsigned int pos2 = hash2(tmp_key, ht->size);
        if (ht->table2[pos2] == -1)
        {
            ht->table2[pos2] = tmp_key;
            ht->count++;
            return;
        }
        
        evicted = ht->table2[pos2];
                  ht->table2[pos2] = tmp_key;
                                     tmp_key = evicted;
        
        loop_count++;
    }
    
    cuckoo_rehash(ht);
    cuckoo_insert(ht, tmp_key);
}

int cuckoo_search(CuckooHashTable* ht, int key)
{
    if (!ht) return 0;
    
    unsigned int pos1 = hash1(key, ht->size);
    if (ht->table1[pos1] == key) return 1;
    
    unsigned int pos2 = hash2(key, ht->size);
    if (ht->table2[pos2] == key) return 1;
    
    return 0;
}

int cuckoo_delete(CuckooHashTable* ht, int key)
{
    if (!ht) return 0;
    
    unsigned int pos1 = hash1(key, ht->size);
    if (ht->table1[pos1] == key)
    {
        ht->table1[pos1] = -1;
        ht->count--;
        return 1;
    }
    
    unsigned int pos2 = hash2(key, ht->size);
    if (ht->table2[pos2] == key)
    {
        ht->table2[pos2] = -1;
        ht->count--;
        return 1;
    }
    return 0;
}

void cuckoo_destroy(CuckooHashTable* ht)
{
    if (!ht) return;
    
    free(ht->table1);
    free(ht->table2);
    free(ht);
}
