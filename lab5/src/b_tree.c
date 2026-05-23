#include "../include/b_tree.h"
#include <stdlib.h>
#include <stdbool.h>

static int  b_find_key      (BNode* node,   int key);
static void b_split_child   (BNode* parent, int i, BNode* child);
static void b_insert_nonfull(BNode* node,   int key);

BNode* b_create(void)
{
    BNode* node = (BNode*)calloc(1, sizeof(BNode));
    if (node == NULL) return NULL;
    
    node->n = 0;
    node->leaf = true;
    node->keys = (int*)calloc(2 * T - 1, sizeof(int));
    node->children = (BNode**)calloc(2 * T, sizeof(BNode*));
    
    if (node->keys == NULL || node->children == NULL)
    {
        free(node->keys);
        free(node->children);
        free(node);
        return NULL;
    }
    
    return node;
}

static void b_split_child(BNode* parent, int i, BNode* child)
{
    BNode* new_child = (BNode*)calloc(1, sizeof(BNode));
    new_child->leaf = child->leaf;
    new_child->n = T - 1;
    new_child->keys = (int*)calloc(2 * T - 1, sizeof(int));
    new_child->children = (BNode**)calloc(2 * T, sizeof(BNode*));
    
    for (int j = 0; j < T - 1; j++)
    {
        new_child->keys[j] = child->keys[j + T];
    }
    
    if (!child->leaf)
    {
        for (int j = 0; j < T; j++)
        {
            new_child->children[j] = child->children[j + T];
        }
    }
    
    child->n = T - 1;
    
    for (int j = parent->n; j >= i + 1; j--)
    {
        parent->children[j + 1] = parent->children[j];
    }
    parent->children[i + 1] = new_child;
    
    for (int j = parent->n - 1; j >= i; j--)
    {
        parent->keys[j + 1] = parent->keys[j];
    }
    parent->keys[i] = child->keys[T - 1];
    parent->n++;
}

static void b_insert_nonfull(BNode* node, int key)
{
    int i = node->n - 1;
    
    int pos = b_find_key(node, key);
    if (pos < node->n && node->keys[pos] == key)
    {
        return;
    }

    if (node->leaf)
    {
        while (i >= 0 && key < node->keys[i])
        {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->n++;
    }
    else
    {
        while (i >= 0 && key < node->keys[i]) i--;
        i++;
        
        if (node->children[i]->n == 2 * T - 1)
        {
            b_split_child(node, i, node->children[i]);
            if (key > node->keys[i]) i++;
        }
        b_insert_nonfull(node->children[i], key);
    }
}

BNode* b_insert(BNode* root, int key)
{
    if (root == NULL)
    {
        return b_create();
    }
    
    if (root->n == 2 * T - 1)
    {
        BNode* new_root = b_create();
        new_root->leaf = false;
        new_root->children[0] = root;
        b_split_child(new_root, 0, root);
        b_insert_nonfull(new_root, key);
        return new_root;
    }
    else
    {
        b_insert_nonfull(root, key);
        return root;
    }
}

static int b_find_key(BNode* node, int key)
{
    int i = 0;
    while (i < node->n && key > node->keys[i]) i++;
    return i;
}

static void b_merge(BNode* parent, int i)
{
    BNode* left = parent->children[i];
    BNode* right = parent->children[i + 1];
    
    left->keys[T - 1] = parent->keys[i];
    
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
    
    for (int j = i + 1; j < parent->n; j++)
    {
        parent->keys[j - 1] = parent->keys[j];
        parent->children[j] = parent->children[j + 1];
    }
    
    left->n = 2 * T - 1;
    parent->n--;
    
    free(right->keys);
    free(right->children);
    free(right);
}

static void b_borrow_from_prev(BNode* parent, int i)
{
    BNode* child = parent->children[i];
    BNode* sibling = parent->children[i - 1];
    
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
    
    child->keys[0] = parent->keys[i - 1];
    parent->keys[i - 1] = sibling->keys[sibling->n - 1];
    child->n++;
    sibling->n--;
}

static void b_borrow_from_next(BNode* parent, int i)
{
    BNode* child = parent->children[i];
    BNode* sibling = parent->children[i + 1];
    
    child->keys[child->n] = parent->keys[i];
    parent->keys[i] = sibling->keys[0];
    
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

static BNode* b_delete_from_node(BNode* node, int key)
{
    if (node == NULL) return NULL;
    
    int idx = b_find_key(node, key);
    
    if (idx < node->n && node->keys[idx] == key)
    {
        if (node->leaf)
        {
            for (int i = idx + 1; i < node->n; i++)
            {
                node->keys[i - 1] = node->keys[i];
            }
            node->n--;
        }
        else
        {
            if (node->children[idx]->n >= T)
            {
                BNode* pred = node->children[idx];
                while (!pred->leaf) pred = pred->children[pred->n];
                int pred_key = pred->keys[pred->n - 1];
                node->keys[idx] = pred_key;
                node->children[idx] = b_delete_from_node(node->children[idx], pred_key);
            }
            else if (node->children[idx + 1]->n >= T)
            {
                BNode* succ = node->children[idx + 1];
                while (!succ->leaf) succ = succ->children[0];
                int succ_key = succ->keys[0];
                node->keys[idx] = succ_key;
                node->children[idx + 1] = b_delete_from_node(node->children[idx + 1], succ_key);
            }
            else
            {
                b_merge(node, idx);
                node->children[idx] = b_delete_from_node(node->children[idx], key);
            }
        }
    }
    else
    {
        if (node->leaf) return node;
        
        int child_idx = idx;
        
        if (node->children[child_idx]->n == T - 1)
        {
            if (child_idx > 0 && node->children[child_idx - 1]->n >= T)
            {
                b_borrow_from_prev(node, child_idx);
            }
            else if (child_idx < node->n && node->children[child_idx + 1]->n >= T)
            {
                b_borrow_from_next(node, child_idx);
            }
            else
            {
                if (child_idx < node->n)
                {
                    b_merge(node, child_idx);
                }
                else
                {
                    b_merge(node, child_idx - 1);
                    child_idx--;
                }
            }
        }
        
        node->children[child_idx] = b_delete_from_node(node->children[child_idx], key);
    }
    
    return node;
}

BNode* b_delete(BNode* root, int key)
{
    if (root == NULL) return NULL;
    
    root = b_delete_from_node(root, key);
    
    if (root->n == 0 && !root->leaf)
    {
        BNode* old_root = root;
        root = root->children[0];
        free(old_root->keys);
        free(old_root->children);
        free(old_root);
    }
    
    return root;
}

BNode* b_find(BNode* root, int key)
{
    BNode* current = root;
    while (current != NULL)
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

void b_free(BNode* root)
{
    b_free_nodes(root);
}

static int b_height_rec(BNode* node)
{
    if (node == NULL || node->leaf) return 0;
    return 1 + b_height_rec(node->children[0]);
}

int b_height(BNode* root)
{
    if (root == NULL) return 0;
    return b_height_rec(root);
}