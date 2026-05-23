#ifndef TREAP_H
#define TREAP_H

typedef struct TreapNode
{
    int key;
    int priority;
    struct TreapNode* left;
    struct TreapNode* right;
} TreapNode;

TreapNode* treap_create(int key);
TreapNode* treap_insert(TreapNode* root, int key);
TreapNode* treap_delete(TreapNode* root, int key);
TreapNode* treap_find  (TreapNode* root, int key);
void       treap_free  (TreapNode* root);
int        treap_height(TreapNode* root);

#endif // TREAP_H
