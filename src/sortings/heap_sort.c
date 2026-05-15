#include "sortings/heap_sort.h"

void bottom_up_heapify(int arr[], size_t n, size_t i, int k)
{
    while (1)
    {
        size_t first_child = i * k + 1;
        if (first_child >= n)
        {
            break;
        }
        
        int max_child = first_child;
        int last_child = (i * k + k < n) ? i * k + k : n - 1;
        
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

void build_k_heap(int arr[], size_t n, int k)
{
    for (int i = (n - 1) / k; i >= 0; i--)
    {
        bottom_up_heapify(arr, n, i, k);
    }
}

void heap_sort_k(int arr[], size_t n, int k)
{
    if (n <= 1) return;
    
    build_k_heap(arr, n, k);
    
    for (int i = n - 1; i > 0; i--)
    {
        int temp = arr[0];
                   arr[0] = arr[i];
                            arr[i] = temp;
        
        bottom_up_heapify(arr, i, 0, k);
    }
}