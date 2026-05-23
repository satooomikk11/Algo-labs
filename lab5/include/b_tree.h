#ifndef B_TREE_H
#define B_TREE_H

#include <stdbool.h>

#define T 32

typedef struct BNode
{
    int n;  // текущее количество ключей
    int* keys;
    struct BNode** children;
    bool leaf;
} BNode;

BNode* b_create(void);
BNode* b_insert(BNode* root, int key);
BNode* b_delete(BNode* root, int key);
BNode* b_find  (BNode* root, int key);
void   b_free  (BNode* root);
int    b_height(BNode* root);

#endif // B_TREE_H