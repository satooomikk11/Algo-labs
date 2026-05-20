#include "../include/treap.h"
#include <stdlib.h>
#include <time.h>

static int rand_priority(void)
{
    static int initialized = 0;
    if (!initialized)
    {
        srand(time(NULL));
        initialized = 1;
    }
    return rand();
}

TreapNode* treap_create(int key)
{
    TreapNode* node = (TreapNode*)calloc(1, sizeof(TreapNode));
    if (node)
    {
        node->key   = key;
        node->priority = rand_priority();
        node->left  = NULL;
        node->right = NULL;
    }
    return node;
}

static TreapNode* rotate_right(TreapNode* y)
{
    TreapNode* x  = y->left;
    TreapNode* T2 = x->right;
    
    x->right = y;
    y->left  = T2;
    
    return x;
}

static TreapNode* rotate_left(TreapNode* x)
{
    TreapNode* y  = x->right;
    TreapNode* T2 = y->left;
    
    y->left  = x;
    x->right = T2;
    
    return y;
}

TreapNode* treap_insert(TreapNode* root, int key)
{
    if (root == NULL) return treap_create(key);
    
    if (key < root->key)
    {
        root->left = treap_insert(root->left, key);
        if (root->left->priority > root->priority)
        {
            root = rotate_right(root);
        }
    }
    else if (key > root->key)
    {
        root->right = treap_insert(root->right, key);
        if (root->right->priority > root->priority)
        {
            root = rotate_left(root);
        }
    }
    
    return root;
}

TreapNode* treap_delete(TreapNode* root, int key) 
{
    if (root == NULL) return NULL;
    
    if (key < root->key)
    {
        root->left = treap_delete(root->left, key);
    }
    else if (key > root->key)
    {
        root->right = treap_delete(root->right, key);
    }
    else
    {
        if (root->left == NULL)
        {
            TreapNode* temp = root->right;
            free(root);
            return temp;
        }
        if (root->right == NULL)
        {
            TreapNode* temp = root->left;
            free(root);
            return temp;
        }
        
        if (root->left->priority > root->right->priority)
        {
            root = rotate_right(root);
            root->right = treap_delete(root->right, key);
        }
        else
        {
            root = rotate_left(root);
            root->left = treap_delete(root->left, key);
        }
    }
    
    return root;
}

TreapNode* treap_find(TreapNode* root, int key)
{
    if (root == NULL || root->key == key) return root;
    if (key < root->key) return treap_find(root->left, key);
    return treap_find(root->right, key);
}

void treap_free(TreapNode* root) 
{
    if (root == NULL) return;
    treap_free(root->left);
    treap_free(root->right);
    free(root);
}

int treap_height(TreapNode* root)
{
    if (root == NULL) return -1;
    int left_h  = treap_height(root->left);
    int right_h = treap_height(root->right);
    return (left_h > right_h ? left_h : right_h) + 1;
}
