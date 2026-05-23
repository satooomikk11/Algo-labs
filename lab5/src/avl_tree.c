#include "../include/avl_tree.h"
#include <stdlib.h>

static int max(int a, int b)
{
    return a > b ? a : b;
}

static int height(AVLNode* node)
{
    return node ? node->height : -1;
}

static int balance_factor(AVLNode* node)
{
    return height(node->right) - height(node->left);
}

static void update_height(AVLNode* node)
{
    if (node)
    {
        node->height = max(height(node->left), height(node->right)) + 1;
    }
}

static AVLNode* rotate_right(AVLNode* y)
{
    AVLNode* x  = y->left;
    AVLNode* T2 = x->right;
    
    x->right = y;
    y->left  = T2;
    
    update_height(y);
    update_height(x);
    
    return x;
}

static AVLNode* rotate_left(AVLNode* x)
{
    AVLNode* y  = x->right;
    AVLNode* T2 = y->left;
    
    y->left  = x;
    x->right = T2;
    
    update_height(x);
    update_height(y);
    
    return y;
}

static AVLNode* balance(AVLNode* node)
{
    if (node == NULL) return NULL;
    
    update_height(node);
    int bf = balance_factor(node);
    
    if (bf == 2)
    {
        if (balance_factor(node->right) == -1)
        { 
            node->right = rotate_right(node->right);
        }
        return rotate_left(node);
    }
    
    if (bf == -2)
    {
        if (balance_factor(node->left) == 1)
        {
            node->left = rotate_left(node->left);
        }
        return rotate_right(node);
    }
    
    return node;
}

AVLNode* avl_create(int key)
{
    AVLNode* node = (AVLNode*)calloc(1, sizeof(AVLNode));
    if (node)
    {
        node->key    = key;
        node->height = 0;
        node->left   = NULL;
        node->right  = NULL;
    }
    return node;
}

AVLNode* avl_insert(AVLNode* root, int key)
{
    if (root == NULL) return avl_create(key);
    
    if (key < root->key)
    {
        root->left = avl_insert(root->left, key);
    }
    else if (key > root->key)
    {
        root->right = avl_insert(root->right, key);
    }
    else
    {
        return root;
    }
    
    return balance(root);
}

static AVLNode* find_min(AVLNode* root)
{
    if (root == NULL) return NULL;
    while (root->left) root = root->left;
    return root;
}

AVLNode* avl_delete(AVLNode* root, int key)
{
    if (root == NULL) return NULL;
    
    if (key < root->key)
    {
        root->left = avl_delete(root->left, key);
    }
    else if (key > root->key)
    {
        root->right = avl_delete(root->right, key);
    }
    else
    {
        if (root->left == NULL || root->right == NULL)
        {
            AVLNode* temp = root->left ? root->left : root->right;
            free(root);
            return temp;
        } 
        else
        {
            AVLNode* temp = find_min(root->right);
            root->key     = temp->key;
            root->right   = avl_delete(root->right, temp->key);
        }
    }
    
    return balance(root);
}

AVLNode* avl_find(AVLNode* root, int key)
{
    if (root == NULL || root->key == key) return root;
    if (key < root->key) return avl_find(root->left, key);
    return avl_find(root->right, key);
}

void avl_free(AVLNode* root)
{
    if (root == NULL) return;
    avl_free(root->left);
    avl_free(root->right);
    free(root);
}

int avl_height(AVLNode* root)
{
    return height(root);
}
