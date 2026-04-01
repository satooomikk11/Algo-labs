#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#define MAX_HEAP_SIZE 1000000

typedef enum
{
    OK    = 0,
    ERROR = -1
} Status;

typedef struct
{
    long long data[MAX_HEAP_SIZE];
    int size;
} BinaryHeap;

void   binary_heap_init      (BinaryHeap* heap);
Status binary_heap_insert    (BinaryHeap* heap, long long x);
void   binary_heap_linear    (BinaryHeap* heap, long long values[], int n);
void   binary_heap_insertions(BinaryHeap* heap, long long values[], int n);
Status binary_heap_is_valid  (BinaryHeap* heap);
void   binary_heap_clear     (BinaryHeap* heap);

#endif // BINARY_HEAP_H