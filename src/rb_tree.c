#include "../include/rb_tree.h"
#include <stdlib.h>

static RBNode* rb_create_node(RBTree* tree, int key, Color color)
{
    RBNode* node = (RBNode*)calloc(1, sizeof(RBNode));
    if (node)
    {
        node->key    = key;
        node->color  = color;
        node->left   = tree->nil;
        node->right  = tree->nil;
        node->parent = tree->nil;
    }
    return node;
}

RBTree* rb_create(void)
{
    RBTree* tree = (RBTree*)calloc(1, sizeof(RBTree));
    if (tree == NULL) return NULL;
    
    tree->nil = (RBNode*)calloc(1, sizeof(RBNode));
    if (tree->nil == NULL)
    {
        free(tree);
        return NULL;
    }
    
    tree->nil->color = BLACK;
    tree->nil->left  = tree->nil->right = tree->nil->parent = NULL;
    tree->root       = tree->nil;
    
    return tree;
}

static void rotate_left(RBTree* tree, RBNode* x)
{
    RBNode* y = x->right;
    x->right = y->left;
    if (y->left != tree->nil) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == tree->nil)
    {
        tree->root = y;
    }
    else if (x == x->parent->left)
    {
        x->parent->left  = y;
    }
    else
    {
        x->parent->right = y;
    }
    y->left   = x;
    x->parent = y;
}

static void rotate_right(RBTree* tree, RBNode* x)
{
    RBNode* y = x->left;
    x->left = y->right;
    if (y->right != tree->nil) y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == tree->nil)
    {
        tree->root = y;
    }
    else if (x == x->parent->right)
    {
        x->parent->right = y;
    }
    else
    {
        x->parent->left  = y;
    }
    y->right  = x;
    x->parent = y;
}

static void rb_insert_fixup(RBTree* tree, RBNode* z)
{
    while (z->parent->color == RED)
    {
        if (z->parent == z->parent->parent->left)
        {
            RBNode* y = z->parent->parent->right;
            if (y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->right)
                {
                    z = z->parent;
                    rotate_left(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotate_right(tree, z->parent->parent);
            }
        }
        else
        {
            RBNode* y = z->parent->parent->left;
            if (y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->left)
                {
                    z = z->parent;
                    rotate_right(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotate_left(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

void rb_insert(RBTree* tree, int key)
{
    RBNode* z = rb_create_node(tree, key, RED);
    if (z == NULL) return;
    
    RBNode* y = tree->nil;
    RBNode* x = tree->root;
    
    while (x != tree->nil)
    {
        y = x;
        if (z->key < x->key)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }
    
    z->parent = y;
    if (y == tree->nil)
    {
        tree->root = z;
    }
    else if (z->key < y->key)
    {
        y->left = z;
    }
    else
    {
        y->right = z;
    }
    
    rb_insert_fixup(tree, z);
}

static void rb_transplant(RBTree* tree, RBNode* u, RBNode* v)
{
    if (u->parent == tree->nil)
    {
        tree->root = v;
    }
    else if (u == u->parent->left)
    {
        u->parent->left  = v;
    }
    else
    {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

static RBNode* rb_minimum(RBTree* tree, RBNode* x)
{
    while (x->left != tree->nil) x = x->left;
    return x;
}

static void rb_delete_fixup(RBTree* tree, RBNode* x)
{
    while (x != tree->root && x->color == BLACK)
    {
        if (x == x->parent->left)
        {
            RBNode* w = x->parent->right;
            if (w->color == RED)
            {
                w->color = BLACK;
                x->parent->color = RED;
                rotate_left(tree, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK)
            {
                w->color = RED;
                x = x->parent;
            }
            else
            {
                if (w->right->color == BLACK)
                {
                    w->left->color = BLACK;
                    w->color = RED;
                    rotate_right(tree, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color  = BLACK;
                rotate_left(tree, x->parent);
                x = tree->root;
            }
        }
        else
        {
            RBNode* w = x->parent->left;
            if (w->color == RED)
            {
                w->color = BLACK;
                x->parent->color = RED;
                rotate_right(tree, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK)
            {
                w->color = RED;
                x = x->parent;
            }
            else
            {
                if (w->left->color == BLACK
                ){
                    w->right->color = BLACK;
                    w->color = RED;
                    rotate_left(tree, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color   = BLACK;
                rotate_right(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = BLACK;
}

void rb_delete(RBTree* tree, int key)
{
    RBNode* z = rb_find(tree, key);
    if (z == tree->nil) return;
    
    RBNode* y = z;
    RBNode* x;
    Color y_original_color = y->color;
    
    if (z->left == tree->nil) 
    {
        x = z->right;
        rb_transplant(tree, z, z->right);
    }
    else if (z->right == tree->nil)
    {
        x = z->left;
        rb_transplant(tree, z, z->left);
    }
    else
    {
        y = rb_minimum(tree, z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z)
        {
            x->parent = y;
        }
        else
        {
            rb_transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        rb_transplant(tree, z, y);
        y->left  = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    
    free(z);
    
    if (y_original_color == BLACK)
    {
        rb_delete_fixup(tree, x);
    }
}

RBNode* rb_find(RBTree* tree, int key)
{ 
    RBNode* current = tree->root;
    while (current != tree->nil)
    {
        if (key == current->key) return current;
        if (key < current->key)  current = current->left;
        else current = current->right;
    }
    return tree->nil;
}

static void rb_free_nodes(RBTree* tree, RBNode* node)
{
    if (node == tree->nil) return;
    rb_free_nodes(tree, node->left);
    rb_free_nodes(tree, node->right);
    free(node);
}

void rb_free(RBTree* tree)
{
    if (tree == NULL) return;
    rb_free_nodes(tree, tree->root);
    free(tree->nil);
    free(tree);
}

static int rb_height_rec(RBNode* node, RBNode* nil)
{
    if (node == nil) return -1;
    int left_h  = rb_height_rec(node->left, nil);
    int right_h = rb_height_rec(node->right, nil);
    return (left_h > right_h ? left_h : right_h) + 1;
}

int rb_height(RBTree* tree)
{
    return rb_height_rec(tree->root, tree->nil);
}
