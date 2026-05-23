#ifndef NAIVE_TREE_H
#define NAIVE_TREE_H

typedef struct NaiveNode
{
    int key;
    struct NaiveNode* left;
    struct NaiveNode* right;
} NaiveNode;

NaiveNode* naive_create(int key);
NaiveNode* naive_insert(NaiveNode* root, int key);
NaiveNode* naive_delete(NaiveNode* root, int key);
NaiveNode* naive_find  (NaiveNode* root, int key);
void       naive_free  (NaiveNode* root);
int        naive_height(NaiveNode* root);

#endif // NAIVE_TREE_H
