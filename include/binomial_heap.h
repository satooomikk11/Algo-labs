#ifndef BINOMIAL_HEAP_H
#define BINOMIAL_HEAP_H

#include <stddef.h>

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
    char data[];
} Node;

typedef struct
{
    Node* head;
    size_t data_size;
} BinomialHeap;

void   binomial_heap_init  (BinomialHeap* heap, size_t data_size);
Status binomial_heap_insert(BinomialHeap* heap, const void* key);
void   binomial_heap_clear (BinomialHeap* heap);

#endif // BINOMIAL_HEAP_H