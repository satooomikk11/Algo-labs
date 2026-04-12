#ifndef CUCKOO_H
#define CUCKOO_H

typedef struct
{
    int*  table1;
    int*  table2;
    int   size;
    int   count;
    float load_factor_threshold;
} CuckooHashTable;

CuckooHashTable* cuckoo_create(int initial_size, float threshold);
void cuckoo_insert (CuckooHashTable* ht, int key);
int  cuckoo_search (CuckooHashTable* ht, int key);
int  cuckoo_delete (CuckooHashTable* ht, int key);
void cuckoo_destroy(CuckooHashTable* ht);

#endif // CUCKOO_H