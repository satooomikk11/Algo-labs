#include "binomial_heap.h"
#include "assert_all.h"
#include <stdlib.h>
#include <string.h>

static Node* binomial_heap_create_node(const void* key, size_t data_size)
{
    Node* node = (Node*)calloc(1, sizeof(Node) + data_size);
    if (!node) return NULL;
    
    node->degree  = 0;
    node->parent  = NULL;
    node->child   = NULL;
    node->sibling = NULL;    
    memcpy(node->data, key, data_size);
    
    return node;
}

static int binomial_heap_compare(Node* a, Node* b, CompareFunc compare)
{
    return compare(a->data, b->data);
}

static Node* binomial_heap_merge_trees(Node* tree1, Node* tree2, CompareFunc compare)
{
    ASSERT_ALL(tree1 != NULL, tree2 != NULL, tree1->degree == tree2->degree);
    
    if (binomial_heap_compare(tree1, tree2, compare) > 0)
    {
        Node* temp = tree1;
        tree1 = tree2;
        tree2 = temp;
    }
    
    tree2->parent  = tree1;
    tree2->sibling = tree1->child;
    tree1->child   = tree2;
    tree1->degree++;
    
    return tree1;
}

static Node* binomial_heap_insert_root(Node* head, Node* node)
{
    node->sibling = head;
    return node;
}

static Node* binomial_heap_fix_heap(Node* head, CompareFunc compare)
{
    if (!head) return NULL;
    
    Node* prev = NULL;
    Node* curr = head;
    Node* next = curr->sibling;
    
    while (next && curr->degree == next->degree)
    {
        if (next->sibling && next->sibling->degree == curr->degree)
        {
            prev = curr;
            curr = next;
            next = curr->sibling;
            continue;
        }
        
        if (binomial_heap_compare(curr, next, compare) <= 0)
        {
            curr->sibling = next->sibling;
            curr = binomial_heap_merge_trees(curr, next, compare);
        }
        else
        {
            if (prev)
            {
                prev->sibling = next;
            }
            else
            {
                head = next;
            }

            next = binomial_heap_merge_trees(next, curr, compare);
            curr = next;
        }
        prev = curr;
        next = curr->sibling;
    }
    
    return head;
}

void binomial_heap_init(BinomialHeap* heap, size_t data_size, CompareFunc compare)
{
    ASSERT_ALL(heap != NULL, data_size > 0, compare != NULL);
    
    heap->head      = NULL;
    heap->data_size = data_size;
    heap->compare   = compare;
}

Status binomial_heap_insert(BinomialHeap* heap, const void* key)
{
    ASSERT_ALL(heap != NULL, key != NULL);
    
    Node* node = binomial_heap_create_node(key, heap->data_size);
    if (!node) return ERROR;
    
    heap->head = binomial_heap_insert_root(heap->head, node);
    heap->head = binomial_heap_fix_heap   (heap->head, heap->compare);
    
    return OK;
}

static void binomial_heap_free_node(Node* node)
{
    if (!node) return;
    
    binomial_heap_free_node(node->child);
    binomial_heap_free_node(node->sibling);
    free(node);
}

void binomial_heap_clear(BinomialHeap* heap)
{
    ASSERT_ALL(heap != NULL);
    
    if (heap->head)
    {
        binomial_heap_free_node(heap->head);
        heap->head = NULL;
    }
}