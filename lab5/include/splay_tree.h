#ifndef SPLAY_TREE_H
#define SPLAY_TREE_H

typedef struct SplayNode
{
    int key;
    struct SplayNode* left;
    struct SplayNode* right;
    struct SplayNode* parent;
} SplayNode;

SplayNode* splay_create(int key);
SplayNode* splay_insert(SplayNode* root, int key);
SplayNode* splay_delete(SplayNode* root, int key);
SplayNode* splay_find  (SplayNode* root, int key);
void       splay_free  (SplayNode* root);
int        splay_height(SplayNode* root);

#endif // SPLAY_TREE_H
