#include "../include/naive_tree.h"
#include <stdlib.h>

NaiveNode* naive_create(int key)
{
    NaiveNode* node = (NaiveNode*)calloc(1, sizeof(NaiveNode));
    if (node)
    {
        node->key   = key;
        node->left  = NULL;
        node->right = NULL;
    }
    return node;
}

NaiveNode* naive_insert(NaiveNode* root, int key)
{
    if (root == NULL)
    {
        return naive_create(key);
    }
    
    if (key < root->key)
    {
        root->left = naive_insert(root->left, key);
    }
    else if (key > root->key)
    {
        root->right = naive_insert(root->right, key);
    }
    
    return root;
}

static NaiveNode* naive_find_min(NaiveNode* root)
{
    if (root == NULL) return NULL;
    while (root->left) root = root->left;
    return root;
}

NaiveNode* naive_delete(NaiveNode* root, int key)
{
    if (root == NULL) return NULL;
    
    if (key < root->key)
    {
        root->left = naive_delete(root->left, key);
    }
    else if (key > root->key)
    {
        root->right = naive_delete(root->right, key);
    } 
    else
    {
        if (root->left == NULL)
        {
            NaiveNode* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            NaiveNode* temp = root->left;
            free(root);
            return temp;
        }
        
        NaiveNode* temp = naive_find_min(root->right);
        root->key   = temp->key;
        root->right = naive_delete(root->right, temp->key);
    }
    
    return root;
}

NaiveNode* naive_find(NaiveNode* root, int key)
{
    if (root == NULL || root->key == key) return root;
    if (key < root->key) return naive_find(root->left, key);
    return naive_find(root->right, key);
}

void naive_free(NaiveNode* root)
{
    if (root == NULL) return;
    naive_free(root->left);
    naive_free(root->right);
    free(root);
}

int naive_height(NaiveNode* root)
{
    if (root == NULL) return -1;
    int left_h  = naive_height(root->left);
    int right_h = naive_height(root->right);
    return (left_h > right_h ? left_h : right_h) + 1;
}
