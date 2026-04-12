#ifndef LINEAR_H
#define LINEAR_H

typedef struct
{
    int*  keys;
    int*  status;  // 0 empty, 1 occupied, -1 deleted
    int   size;
    int   count;
    float load_factor_threshold;
} LinearHashTable;

LinearHashTable* linear_create(int initial_size, float threshold);
void linear_insert (LinearHashTable* ht, int key);
int  linear_search (LinearHashTable* ht, int key);
int  linear_delete (LinearHashTable* ht, int key);
void linear_destroy(LinearHashTable* ht);

#endif // LINEAR_H