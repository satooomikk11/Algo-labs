#include "binary_heap.h"
#include <stdlib.h>
#include <assert.h>

static void swap(long long arr[], int i, int j)
{
    assert(arr != NULL);
    assert(i >= 0 && j >= 0);

    long long tmp = arr[i];
                    arr[i] = arr[j];
                             arr[j] = tmp;
}

static void sift_up(long long arr[], int idx)
{
    assert(arr != NULL);
    assert(idx >= 0);

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
    assert(arr  != NULL);
    assert(size >= 0);
    assert(idx  >= 0 && idx < size);

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

void binary_heap_linear(BinaryHeap* heap, long long values[], int n)
{
    assert(heap   != NULL);
    assert(values != NULL);
    assert(n > 0 && n <= MAX_HEAP_SIZE);

    heap->size = n;
    for (int i = 0; i < n; i++)
    {
        heap->data[i] = values[i];
    }
    
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        sift_down(heap->data, heap->size, i);
    }
}

void binary_heap_insertions(BinaryHeap* heap, long long values[], int n)
{
    assert(heap   != NULL);
    assert(values != NULL);
    assert(n > 0 && n <= MAX_HEAP_SIZE);

    binary_heap_init(heap);
    
    for (int i = 0; i < n; i++)
    {
        binary_heap_insert(heap, values[i]);
    }
}

Status binary_heap_is_valid(BinaryHeap* heap)
{
    assert(heap != NULL);

    for (int i = 0; i < heap->size; i++)
    {
        int left  = 2 * i + 1;
        int right = 2 * i + 2;
        
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
