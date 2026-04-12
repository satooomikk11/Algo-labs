#ifndef QUADRATIC_H
#define QUADRATIC_H

typedef struct
{
    int*  keys;
    int*  status;  // 0 empty, 1 occupied, -1 deleted
    int   size;
    int   count;
    float load_factor_threshold;
} QuadraticHashTable;

QuadraticHashTable* quadratic_create(int initial_size, float threshold);
void quadratic_insert (QuadraticHashTable* ht, int key);
int  quadratic_search (QuadraticHashTable* ht, int key);
int  quadratic_delete (QuadraticHashTable* ht, int key);
void quadratic_destroy(QuadraticHashTable* ht);

#endif // QUADRATIC_H