#include "../include/splay_tree.h"
#include <stdlib.h>

SplayNode* splay_create(int key)
{
    SplayNode* node = (SplayNode*)calloc(1, sizeof(SplayNode));
    if (node)
    {
        node->key    = key;
        node->left   = NULL;
        node->right  = NULL;
        node->parent = NULL;
    }
    return node;
}

static void rotate_right(SplayNode** root, SplayNode* x)
{
    SplayNode* y = x->left;
    if (!y) return;
    
    x->left   = y->right;
    if (y->right) y->right->parent = x;
    y->parent = x->parent;
    
    if (!x->parent) *root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    
    y->right  = x;
    x->parent = y;
}

static void rotate_left(SplayNode** root, SplayNode* x)
{
    SplayNode* y = x->right;
    if (!y) return;
    
    x->right  = y->left;
    if (y->left) y->left->parent = x;
    y->parent = x->parent;
    
    if (!x->parent) *root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    
    y->left   = x;
    x->parent = y;
}

static void splay(SplayNode** root, SplayNode* x)
{
    if (!x) return;
    
    while (x->parent)
    {
        SplayNode* p = x->parent;
        SplayNode* g = p->parent;
        
        if (!g)
        {
            if (x == p->left) rotate_right(root, p);
            else rotate_left(root, p);
        }
        else if (x == p->left && p == g->left)
        {
            rotate_right(root, g);
            rotate_right(root, p);
        }
        else if (x == p->right && p == g->right)
        {
            rotate_left (root, g);
            rotate_left (root, p);
        }
        else if (x == p->right && p == g->left)
        {
            rotate_left (root, p);
            rotate_right(root, g);
        }
        else
        {
            rotate_right(root, p);
            rotate_left (root, g);
        }
    }
    *root = x;
}

static SplayNode* find_node(SplayNode** root, int key)
{
    SplayNode* x = *root;
    SplayNode* last = NULL;
    
    while (x)
    {
        last = x;
        if (key < x->key) x = x->left;
        else if (key > x->key) x = x->right;
        else
        {
            splay(root, x);
            return x;
        }
    }
    
    if (last) splay(root, last);
    return NULL;
}

SplayNode* splay_insert(SplayNode* root, int key)
{
    if (!root)
    { 
        return splay_create(key);
    }
    
    SplayNode* x = root;
    SplayNode* parent = NULL;
    
    while (x)
    {
        parent = x;
        if (key < x->key) x = x->left;
        else if (key > x->key) x = x->right;
        else
        {
            splay(&root, x);
            return root;
        }
    }
    
    SplayNode* new_node = splay_create(key);
    new_node->parent = parent;
    
    if (key < parent->key) parent->left = new_node;
    else parent->right = new_node;
    
    splay(&root, new_node);
    return root;
}

SplayNode* splay_delete(SplayNode* root, int key)
{
    SplayNode* node = find_node(&root, key);
    if (!node || node->key != key)
    {
        return root;
    }
    
    splay(&root, node);
    
    if (!root->left)
    {
        SplayNode* new_root = root->right;
        if (new_root) new_root->parent = NULL;
        free(root);
        return new_root;
    }
    
    if (!root->right)
    {
        SplayNode* new_root = root->left;
        if (new_root) new_root->parent = NULL;
        free(root);
        return new_root;
    }
    
    SplayNode* left_max = root->left;
    while (left_max->right) left_max = left_max->right;
    
    splay(&root->left, left_max);
    
    root->left->right   = root->right;
    root->right->parent = root->left;
    
    SplayNode* new_root = root->left;
    new_root->parent    = NULL;
    
    free(root);
    
    return new_root;
}

SplayNode* splay_find(SplayNode* root, int key)
{
    SplayNode* node = find_node(&root, key);
    if (node && node->key == key) return node;
    return NULL;
}

void splay_free(SplayNode* root)
{
    if (!root) return;
    
    SplayNode* current = root;
    while (current)
    {
        if (current->left)
        {
            current = current->left;
        }
        else if (current->right)
        {
            current = current->right;
        }
        else
        {
            SplayNode* parent = current->parent;
            if (parent)
            {
                if (parent->left == current) parent->left = NULL;
                else parent->right = NULL;
            }
            free(current);
            current = parent;
        }
    }
}

int splay_height(SplayNode* root)
{
    if (!root) return -1;
    int left_h  = splay_height(root->left);
    int right_h = splay_height(root->right);
    return (left_h > right_h ? left_h : right_h) + 1;
}
