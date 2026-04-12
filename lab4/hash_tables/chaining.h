#ifndef CHAINING_H
#define CHAINING_H

typedef struct ChainingNode
{
    int key;
    struct ChainingNode* next;
} ChainingNode;

typedef struct
{
    ChainingNode** buckets;
    int size;
    int count;
    float load_factor_threshold;
} ChainingHashTable;

ChainingHashTable* chaining_create(int initial_size, float threshold);
void  chaining_insert         (ChainingHashTable* ht, int key);
int   chaining_search         (ChainingHashTable* ht, int key);
int   chaining_delete         (ChainingHashTable* ht, int key);
void  chaining_destroy        (ChainingHashTable* ht);
float chaining_get_load_factor(ChainingHashTable* ht);

#endif // CHAINING_H