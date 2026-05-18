#include "binary_heap.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "assert_all.h"

static void swap(long long arr[], int i, int j)
{
    ASSERT_ALL(arr != NULL, i >= 0 && j >= 0);

    long long tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}

static void sift_up(long long arr[], int idx)
{
        ASSERT_ALL(arr != NULL, idx >= 0);

    while (idx > 0)
    {
        int parent = (idx - 1) / 2;
        if (arr[parent] > arr[idx])
        {
            swap(arr, parent, idx);
            idx = parent;
        }
        else
        {
            break;
        }
    }
}

static void sift_down(long long arr[], int size, int idx)
{
    ASSERT_ALL(arr != NULL, size >= 0, idx >= 0 && idx < size);

    while (1)
    {
        int left  = 2 * idx + 1;
        int right = 2 * idx + 2;
        int smallest = idx;
        
        if (left  < size && arr[left]  < arr[smallest])
            smallest = left;
        if (right < size && arr[right] < arr[smallest])
            smallest = right;
        
        if (smallest != idx)
        {
            swap(arr, idx, smallest);
            idx = smallest;
        }
        else
        {
            break;
        }
    }
}

void binary_heap_init(BinaryHeap* heap)
{
    assert(heap != NULL);

    heap->size = 0;
}

Status binary_heap_insert(BinaryHeap* heap, long long x)
{
    assert(heap != NULL);

    if (heap->size >= MAX_HEAP_SIZE)
    {
        return ERROR;
    }
    
    heap->data[heap->size] = x;
    sift_up(heap->data, heap->size);
    heap->size++;
    return OK;
}

void binary_heap_linear(BinaryHeap* heap, long long values[], size_t n)
{
    ASSERT_ALL(heap != NULL, values != NULL, n > 0, n <= MAX_HEAP_SIZE);

    heap->size = n;
    memcpy(heap->data, values, n * sizeof(long long));
    
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        sift_down(heap->data, heap->size, i);
    }
}

void binary_heap_insertions(BinaryHeap* heap, long long values[], size_t n)
{
    ASSERT_ALL(heap != NULL, values != NULL, n > 0, n <= MAX_HEAP_SIZE);

    binary_heap_init(heap);
    
    for (size_t i = 0; i < n; i++)
    {
        binary_heap_insert(heap, values[i]);
    }
}

Status binary_heap_is_valid(const BinaryHeap* heap)
{
    assert(heap != NULL);

    for (size_t i = 0; i < heap->size; i++)
    {
        size_t left  = 2 * i + 1;
        size_t right = 2 * i + 2;
        
        if (left  < heap->size && heap->data[left]  < heap->data[i])
            return ERROR;
        if (right < heap->size && heap->data[right] < heap->data[i])
            return ERROR;
    }
    return OK;
}

void binary_heap_clear(BinaryHeap* heap)
{
    assert(heap != NULL);
    
    heap->size = 0;
}
