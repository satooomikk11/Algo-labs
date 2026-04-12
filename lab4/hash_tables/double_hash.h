#ifndef DOUBLE_HASH_H
#define DOUBLE_HASH_H

typedef struct
{
    int*  keys;
    int*  status;  // 0 empty, 1 occupied, -1 deleted
    int   size;
    int   count;
    float load_factor_threshold;
} DoubleHashTable;

DoubleHashTable* double_create(int initial_size, float threshold);
void double_insert (DoubleHashTable* ht, int key);
int  double_search (DoubleHashTable* ht, int key);
int  double_delete (DoubleHashTable* ht, int key);
void double_destroy(DoubleHashTable* ht);

#endif // DOUBLE_HASH_H