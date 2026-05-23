#ifndef RB_TREE_H
#define RB_TREE_H

typedef enum { RED, BLACK } Color;

typedef struct RBNode
{
    int   key;
    Color color;
    struct RBNode* left;
    struct RBNode* right;
    struct RBNode* parent;
} RBNode;

typedef struct
{
    RBNode* root;
    RBNode* nil;
} RBTree;

RBTree* rb_create(void);
void    rb_insert(RBTree* tree, int key);
void    rb_delete(RBTree* tree, int key);
RBNode* rb_find  (RBTree* tree, int key);
void    rb_free  (RBTree* tree);
int     rb_height(RBTree* tree);

#endif // RB_TREE_H
