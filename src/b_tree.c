#include "../include/b_tree.h"
#include <stdlib.h>

BTree* b_create(void)
{
    BTree* tree = (BTree*)calloc(1, sizeof(BTree));
    if (tree == NULL) return NULL;
    
    tree->root = (BNode*)calloc(1, sizeof(BNode));
    if (tree->root == NULL)
    {
        free(tree);
        return NULL;
    }
    
    tree->root->n = 0;
    tree->root->keys     = (int*)calloc(sizeof(int), (2 * T - 1));
    tree->root->children = (BNode**)calloc(sizeof(BNode*), (2 * T));
    tree->root->leaf = 1;
    
    if (tree->root->keys == NULL || tree->root->children == NULL)
    {
        free(tree->root->keys);
        free(tree->root->children);
        free(tree->root);
        free(tree);
        return NULL;
    }
    
    return tree;
}

static void b_split_child(BNode* x, int i)
{
    BNode* z = (BNode*)calloc(1, sizeof(BNode));
    BNode* y = x->children[i];
    
    z->leaf = y->leaf;
    z->n = T - 1;
    z->keys     = (int*)calloc(sizeof(int), (2 * T - 1));
    z->children = (BNode**)calloc(sizeof(BNode*), (2 * T));
    
    for (int j = 0; j < T - 1; j++)
    {
        z->keys[j] = y->keys[j + T];
    }
    
    if (!y->leaf)
    {
        for (int j = 0; j < T; j++)
        {
            z->children[j] = y->children[j + T];
        }
    }
    
    y->n = T - 1;
    
    for (int j = x->n; j >= i + 1; j--)
    {
        x->children[j + 1] = x->children[j];
    }
    x->children[i + 1] = z;
    
    for (int j = x->n - 1; j >= i; j--)
    {
        x->keys[j + 1] = x->keys[j];
    }
    x->keys[i] = y->keys[T - 1];
    x->n++;
}

static void b_insert_nonfull(BNode* x, int k)
{
    int i = x->n - 1;
    
    if (x->leaf)
    {
        while (i >= 0 && k < x->keys[i])
        {
            x->keys[i + 1] = x->keys[i];
            i--;
        }
        x->keys[i + 1] = k;
        x->n++;
    }
    else
    {
        while (i >= 0 && k < x->keys[i]) i--;
        i++;
        if (x->children[i]->n == 2 * T - 1)
        {
            b_split_child(x, i);
            if (k > x->keys[i]) i++;
        }
        b_insert_nonfull(x->children[i], k);
    }
}

void b_insert(BTree* tree, int key)
{
    BNode* r = tree->root;
    if (r->n == 2 * T - 1)
    {
        BNode* s = (BNode*)calloc(1, sizeof(BNode));
        s->leaf = 0;
        s->n    = 0;
        s->keys     = (int*)calloc(sizeof(int), (2 * T - 1));
        s->children = (BNode**)calloc(sizeof(BNode*), (2 * T));
        s->children[0] = r;
        tree->root  = s;
        b_split_child(s, 0);
        b_insert_nonfull(s, key);
    }
    else
    {
        b_insert_nonfull(r, key);
    }
}

static int b_find_key(BNode* x, int k)
{
    int i = 0;
    while (i < x->n && k > x->keys[i]) i++;
    return i;
}

static void b_merge(BNode* x, int i)
{
    BNode* left  = x->children[i];
    BNode* right = x->children[i + 1];
    
    left->keys[T - 1] = x->keys[i];
    
    for (int j = 0; j < T - 1; j++)
    {
        left->keys[T + j] = right->keys[j];
    }
    
    if (!left->leaf)
    {
        for (int j = 0; j < T; j++)
        {
            left->children[T + j] = right->children[j];
        }
    }
    
    for (int j = i + 1; j < x->n; j++)
    {
        x->keys[j - 1] = x->keys[j];
        x->children[j] = x->children[j + 1];
    }
    
    left->n = 2 * T - 1;
    x->n--;
    
    free(right->keys);
    free(right->children);
    free(right);
}

static void b_borrow_from_prev(BNode* x, int i)
{
    BNode* child   = x->children[i];
    BNode* sibling = x->children[i - 1];
    
    for (int j = child->n - 1; j >= 0; j--)
    {
        child->keys[j + 1] = child->keys[j];
    }
    
    if (!child->leaf)
    {
        for (int j = child->n; j >= 0; j--)
        {
            child->children[j + 1] = child->children[j];
        }
        child->children[0] = sibling->children[sibling->n];
    }
    
    child->keys[0] = x->keys[i - 1];
    x->keys[i - 1] = sibling->keys[sibling->n - 1];
    child->n++;
    sibling->n--;
}

static void b_borrow_from_next(BNode* x, int i)
{
    BNode* child  = x->children[i];
    BNode* sibling = x->children[i + 1];
    
    child->keys[child->n] = x->keys[i];
    x->keys[i] = sibling->keys[0];
    
    if (!child->leaf)
    {
        child->children[child->n + 1] = sibling->children[0];
        for (int j = 1; j <= sibling->n; j++)
        {
            sibling->children[j - 1] = sibling->children[j];
        }
    }
    
    for (int j = 1; j < sibling->n; j++)
    {
        sibling->keys[j - 1] = sibling->keys[j];
    }
    
    child->n++;
    sibling->n--;
}

static void b_delete_from_node(BNode* x, int k)
{
    int idx = b_find_key(x, k);
    
    if (idx < x->n && x->keys[idx] == k)
    {
        if (x->leaf) 
        {
            for (int i = idx + 1; i < x->n; i++)
            {
                x->keys[i - 1] = x->keys[i];
            }
            x->n--;
        }
        else
        {
            if (x->children[idx]->n >= T)
            {
                BNode* pred  = x->children[idx];
                while (!pred->leaf) pred = pred->children[pred->n];
                int pred_key = pred->keys[pred->n - 1];
                x->keys[idx] = pred_key;
                b_delete_from_node(x->children[idx], pred_key);
            }
            else if (x->children[idx + 1]->n >= T)
            {
                BNode* succ  = x->children[idx + 1];
                while (!succ->leaf) succ = succ->children[0];
                int succ_key = succ->keys[0];
                x->keys[idx] = succ_key;
                b_delete_from_node(x->children[idx + 1], succ_key);
            }
            else
            {
                b_merge(x, idx);
                b_delete_from_node(x->children[idx], k);
            }
        }
    }
    else
    {
        if (x->leaf) return;
        
        int child_idx = idx;
        
        if (x->children[child_idx]->n == T - 1)
        {
            if (child_idx > 0 && x->children[child_idx - 1]->n >= T)
            {
                b_borrow_from_prev(x, child_idx);
            }
            else if (child_idx < x->n && x->children[child_idx + 1]->n >= T)
            {
                b_borrow_from_next(x, child_idx);
            }
            else
            {
                if (child_idx < x->n)
                {
                    b_merge(x, child_idx);
                }
                else
                {
                    b_merge(x, child_idx - 1);
                    child_idx--;
                }
            }
        }
        
        b_delete_from_node(x->children[child_idx], k);
    }
}

void b_delete(BTree* tree, int key)
{
    if (tree->root->n == 0) return;
    b_delete_from_node(tree->root, key);
    
    if (tree->root->n == 0 && !tree->root->leaf)
    {
        BNode* old_root = tree->root;
        tree->root = tree->root->children[0];
        free(old_root->keys);
        free(old_root->children);
        free(old_root);
    }
}

BNode* b_find(BTree* tree, int key)
{
    BNode* current = tree->root;
    while (current)
    {
        int i = b_find_key(current, key);
        if (i < current->n && current->keys[i] == key) return current;
        if (current->leaf) return NULL;
        current = current->children[i];
    }
    return NULL;
}

static void b_free_nodes(BNode* node)
{
    if (node == NULL) return;
    if (!node->leaf)
    {
        for (int i = 0; i <= node->n; i++)
        {
            b_free_nodes(node->children[i]);
        }
    }
    free(node->keys);
    free(node->children);
    free(node);
}

void b_free(BTree* tree)
{
    if (tree == NULL) return;
    b_free_nodes(tree->root);
    free(tree);
}

static int b_height_rec(BNode* node)
{
    if (node->leaf) return 0;
    return 1 + b_height_rec(node->children[0]);
}

int b_height(BTree* tree) 
{
    if (tree->root->n == 0) return 0;
    return b_height_rec(tree->root);
}
