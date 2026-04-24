#include "binomial_heap.h"
#include <stdlib.h>
#include <assert.h>

static Node* binomial_heap_create_node(int key)
{
    Node* node = (Node*)calloc(1, sizeof(Node));
    assert(node != NULL);
    
    node->key     = key;
    node->degree  = 0;
    node->parent  = NULL;
    node->child   = NULL;
    node->sibling = NULL;
    return node;
}

static Node* binomial_heap_merge_trees(Node* tree1, Node* tree2)
{
    assert(tree1 != NULL);
    assert(tree2 != NULL);

    if (tree1->key > tree2->key)
    {
        Node* temp = tree1;
                     tree1 = tree2;
                             tree2 = temp;
    }

    assert(tree1->key <= tree2->key);
    
    tree2->parent  = tree1;
    tree2->sibling = tree1->child;
    tree1->child   = tree2;
    tree1->degree++;
    
    return tree1;
}

static Node* binomial_heap_merge_heaps(Node* head1, Node* head2)
{
    if (!head1) return head2;
    if (!head2) return head1;
    
    Node*  result = NULL;
    Node** pos = &result;
    
    while (head1 && head2)
    {
        assert(head1->degree >= 0);
        assert(head2->degree >= 0);

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

static Node* binomial_heap_union_heaps(Node* head1, Node* head2)
{
    if (!head1) return head2;
    if (!head2) return head1;
    
    Node* new_head = binomial_heap_merge_heaps(head1, head2);
    assert(new_head != NULL);
    
    Node* prev = NULL;
    Node* curr = new_head;
    Node* next = curr->sibling;
    
    while (next)
    {
        assert(curr != NULL);
        assert(next != NULL);

        if (curr->degree != next->degree || 
            (next->sibling && next->sibling->degree == curr->degree))
        {
            prev = curr;
            curr = next;
        }
        else
        {
            if (curr->key <= next->key)
            {
                curr->sibling = next->sibling;
                curr = binomial_heap_merge_trees(curr, next);
                assert(curr != NULL);
            }
            else
            {
                if (prev)
                    prev->sibling = next;
                else
                    new_head = next;

                next = binomial_heap_merge_trees(next, curr);
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

static Node* binomial_heap_fix_heap(Node* head)
{
    if (!head) return NULL;
    
    Node* prev = NULL;
    Node* curr = head;
    Node* next = curr->sibling;
    
    while (next)
    {
        if (curr->degree != next->degree ||
            (next->sibling && next->sibling->degree == curr->degree))
        {
            prev = curr;
            curr = next;
            next = curr->sibling;
        }
        else
        {
            if (curr->key <= next->key)
            {
                curr->sibling = next->sibling;
                curr = binomial_heap_merge_trees(curr, next);
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
                next = binomial_heap_merge_trees(next, curr);
                curr = next;
            }
        }
        next = curr->sibling;
    }
    
    return head;
}

void binomial_heap_init(BinomialHeap* heap)
{
    assert(heap != NULL);
    
    heap->head = NULL;
}

Status binomial_heap_insert(BinomialHeap* heap, int key)
{
    assert(heap != NULL);
    
    Node* node = binomial_heap_create_node(key);
    if (!node) return ERROR;
    
    heap->head = binomial_heap_insert_root(heap->head, node);
    heap->head = binomial_heap_fix_heap   (heap->head);
    
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
