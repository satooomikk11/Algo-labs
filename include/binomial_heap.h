#ifndef BINOMIAL_HEAP_H
#define BINOMIAL_HEAP_H

typedef enum
{
    OK    = 0,
    ERROR = -1
} Status;

typedef struct Node
{
    int key;
    int degree;
    struct Node* parent;
    struct Node* child;
    struct Node* sibling;
} Node;

typedef struct
{
    Node* head;
} BinomialHeap;

void   binomial_heap_init  (BinomialHeap* heap);
Status binomial_heap_insert(BinomialHeap* heap, int key);
void   binomial_heap_clear (BinomialHeap* heap);

#endif // BINOMIAL_HEAP_H