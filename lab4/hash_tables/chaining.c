#include "chaining.h"
#include <stdlib.h>
#include <string.h>

static unsigned int hash_int(int key, int size)
{
    return (unsigned int)key % size;
}

ChainingHashTable* chaining_create(int initial_size, float threshold)
{
    ChainingHashTable* ht = (ChainingHashTable*)calloc(1, sizeof(ChainingHashTable));
    ht->size  = initial_size;
    ht->count = 0;
    ht->load_factor_threshold = threshold;
    ht->buckets = (ChainingNode**)calloc(initial_size, sizeof(ChainingNode*));
    return ht;
}

static void chaining_rehash(ChainingHashTable* ht)
{
    int old_size = ht->size;
    ChainingNode** old_buckets = ht->buckets;
    
    ht->size   *= 2;
    ht->buckets = (ChainingNode**)calloc(ht->size, sizeof(ChainingNode*));
    ht->count   = 0;
    
    for (int i = 0; i < old_size; i++)
    {
        ChainingNode* node = old_buckets[i];
        while (node)
        {
            chaining_insert(ht, node->key);
            ChainingNode* tmp = node;
                                node = node->next;
            free(tmp);
        }
    }
    free(old_buckets);
}

void chaining_insert(ChainingHashTable* ht, int key)
{
    if ((float)ht->count / ht->size >= ht->load_factor_threshold)
    {
        chaining_rehash(ht);
    }
    
    unsigned int index = hash_int(key, ht->size);
    ChainingNode* node = ht->buckets[index];
    
    while (node)
    {
        if (node->key == key) return;
        node = node->next;
    }
    
    ChainingNode* new_node = (ChainingNode*)calloc(1, sizeof(ChainingNode));
    new_node->key  = key;
    new_node->next = ht->buckets[index];
    ht->buckets[index] = new_node;
    ht->count++;
}

int chaining_search(ChainingHashTable* ht, int key)
{
    unsigned int index = hash_int(key, ht->size);
    ChainingNode* node = ht->buckets[index];
    while (node)
    {
        if (node->key == key) return 1;
        node = node->next;
    }
    return 0;
}

int chaining_delete(ChainingHashTable* ht, int key)
{
    unsigned int index = hash_int(key, ht->size);
    ChainingNode* node = ht->buckets[index];
    ChainingNode* prev = NULL;
    
    while (node)
    {
        if (node->key == key)
        {
            if (prev) prev->next = node->next;
            else ht->buckets[index] = node->next;
            free(node);
            ht->count--;
            return 1;
        }
        prev = node;
        node = node->next;
    }
    return 0;
}

void chaining_destroy(ChainingHashTable* ht)
{
    for (int i = 0; i < ht->size; i++)
    {
        ChainingNode* node = ht->buckets[i];
        while (node)
        {
            ChainingNode* tmp = node;
                                node = node->next;
            free(tmp);
        }
    }
    free(ht->buckets);
    free(ht);
}

float chaining_get_load_factor(ChainingHashTable* ht)
{
    return (float)ht->count / ht->size;
}