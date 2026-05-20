#include "sortings/heap_sort.h"

void bottom_up_heapify(int arr[], size_t size, size_t i, int k)
{
    while (1)
    {
        size_t first_child = i * k + 1;
        if (first_child >= size)
        {
            break;
        }
        
        int max_child = first_child;
        int last_child = (i * k + k < size) ? i * k + k : size - 1;
        
        for (int child = first_child + 1; child <= last_child; child++)
        {
            if (arr[child] > arr[max_child])
            {
                max_child = child;
            }
        }
        
        if (arr[i] < arr[max_child])
        {
            int temp = arr[i];
                       arr[i] = arr[max_child];
                                arr[max_child] = temp;
            i = max_child;
        }
        else
        {
            break;
        }
    }
}

void build_k_heap(int arr[], size_t size, int k)
{
    for (int i = (size - 1) / k; i >= 0; i--)
    {
        bottom_up_heapify(arr, size, i, k);
    }
}

void heap_sort_k(int arr[], size_t size, int k)
{
    if (size <= 1) return;
    
    build_k_heap(arr, size, k);
    
    for (int i = size - 1; i > 0; i--)
    {
        int temp = arr[0];
                   arr[0] = arr[i];
                            arr[i] = temp;
        
        bottom_up_heapify(arr, i, 0, k);
    }
}