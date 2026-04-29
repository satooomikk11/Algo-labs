#include "binomial_heap.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "assert_all.h"

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

static int binomial_heap_compare(Node* a, Node* b, size_t data_size)
{
    int a_val = *(int*)a->data;
    int b_val = *(int*)b->data;
    return a_val - b_val;
}

static Node* binomial_heap_merge_trees(Node* tree1, Node* tree2, size_t data_size)
{
    ASSERT_ALL(tree1 != NULL, tree2 != NULL, tree1->degree == tree2->degree);
    
    if (binomial_heap_compare(tree1, tree2, data_size) > 0)
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

static Node* binomial_heap_merge_heaps(Node* head1, Node* head2, size_t data_size)
{
    if (!head1) return head2;
    if (!head2) return head1;
    
    Node*  result = NULL;
    Node** pos = &result;
    
    while (head1 && head2)
    {
        ASSERT_ALL(head1->degree >= 0, head2->degree >= 0);

        if (head1->degree <= head2->degree)
        {
            *pos  = head1;
            head1 = head1->sibling;
        }
        else
        {
            *pos  = head2;
            head2 = head2->sibling;
        }
        pos = &((*pos)->sibling);
    }
    
    if (head1) *pos = head1;
    if (head2) *pos = head2;
    
    return result;
}

static Node* binomial_heap_union_heaps(Node* head1, Node* head2, size_t data_size)
{
    if (!head1) return head2;
    if (!head2) return head1;
    
    Node* new_head = binomial_heap_merge_heaps(head1, head2, data_size);
    if (!new_head) return NULL;
    
    Node* prev = NULL;
    Node* curr = new_head;
    Node* next = curr->sibling;
    
    while (next)
    {
        ASSERT_ALL(curr != NULL, next != NULL);

        if (curr->degree != next->degree)
        {
            break;
        }
        else if (next->sibling && next->sibling->degree == curr->degree)
        {
            prev = curr;
            curr = next;
            next = curr->sibling;
            continue;
        }
        else
        {
            int cmp = binomial_heap_compare(curr, next, data_size);
            
            if (cmp <= 0)  // curr->key <= next->key
            {
                curr->sibling = next->sibling;
                curr = binomial_heap_merge_trees(curr, next, data_size);
                assert(curr != NULL);
            }
            else           // curr->key > next->key
            {
                if (prev)
                    prev->sibling = next;
                else
                    new_head = next;

                next = binomial_heap_merge_trees(next, curr, data_size);
                assert(next != NULL);
                curr = next;
            }
        }
        next = curr->sibling;
    }
    
    return new_head;
}

static Node* binomial_heap_insert_root(Node* head, Node* node)
{
    node->sibling = head;
    return node;
}

static Node* binomial_heap_fix_heap(Node* head, size_t data_size)
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
        
        if (binomial_heap_compare(curr, next, data_size) <= 0)
        {
            curr->sibling = next->sibling;
            curr = binomial_heap_merge_trees(curr, next, data_size);
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
            next = binomial_heap_merge_trees(next, curr, data_size);
            curr = next;
        }
        prev = curr;
        next = curr->sibling;
    }
    
    return head;
}

void binomial_heap_init(BinomialHeap* heap, size_t data_size)
{
    assert(heap != NULL);

    heap->head = NULL;
    heap->data_size = data_size;
}

Status binomial_heap_insert(BinomialHeap* heap, const void* key)
{
    assert(heap != NULL);
    
    Node* node = binomial_heap_create_node(key, heap->data_size);
    if (!node) return ERROR;
    
    heap->head = binomial_heap_insert_root(heap->head, node);
    heap->head = binomial_heap_fix_heap(heap->head, heap->data_size);
    
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
    assert(heap != NULL);

    if (heap->head)
    {
        binomial_heap_free_node(heap->head);
        heap->head = NULL;
    }
}
