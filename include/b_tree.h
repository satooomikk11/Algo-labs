#ifndef B_TREE_H
#define B_TREE_H

#include <stdbool.h>

#define T 32

typedef struct BNode
{
    int  n;  // текущее количество ключей
    int* keys;
    struct BNode** children;
    bool leaf;
} BNode;

typedef struct
{
    BNode* root;
} BTree;

BTree* b_create(void);
void   b_insert(BTree* tree, int key);
void   b_delete(BTree* tree, int key);
BNode* b_find  (BTree* tree, int key);
void   b_free  (BTree* tree);
int    b_height(BTree* tree);

#endif // B_TREE_H
