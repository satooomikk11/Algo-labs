#ifndef PERFECT_HASH_H
#define PERFECT_HASH_H

typedef struct
{
    int* keys;
    int size;
} PerfectSecondary;

typedef struct
{
    PerfectSecondary* table;
    int* hash_params;
    int size;
} PerfectHashTable;

PerfectHashTable* perfect_create(int* keys, int count);
int  perfect_search (PerfectHashTable* ht, int key);
void perfect_destroy(PerfectHashTable* ht);

#endif // PERFECT_HASH_H