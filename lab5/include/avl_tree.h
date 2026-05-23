#ifndef AVL_TREE_H
#define AVL_TREE_H

typedef struct AVLNode
{
    int key;
    int height;
    struct AVLNode* left;
    struct AVLNode* right;
} AVLNode;

AVLNode* avl_create(int key);
AVLNode* avl_insert(AVLNode* root, int key);
AVLNode* avl_delete(AVLNode* root, int key);
AVLNode* avl_find  (AVLNode* root, int key);
void     avl_free  (AVLNode* root);
int      avl_height(AVLNode* root);

#endif // AVL_TREE_H
