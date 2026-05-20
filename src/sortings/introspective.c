#include "sortings/introspective.h"
#include "sortings/quick_sort_pivot.h"
#include "sortings/heap_sort.h"
#include "sortings/shell_sort.h"
#include <stddef.h>

static void introspective_rec(int* arr, size_t low, size_t high, int depth_limit, int threshold)
{
    size_t size = high - low + 1;
    
    if (size <= (size_t)threshold)
    {
        shell_sort(arr + low, size);
        return;
    }
    
    if (depth_limit <= 0)
    {
        heap_sort_k(arr + low, size, 4);  // k = 4
        return;
    }
    
    if (low < high)
    {
        int pivot = arr[low + (high - low) / 2];
        size_t i = low - 1;
        size_t j = high + 1;
        
        while (1)
        {
            do { i++; } while (arr[i] < pivot);
            do { j--; } while (arr[j] > pivot);
            
            if (i >= j) break;
            
            int temp = arr[i];
                       arr[i] = arr[j];
                                arr[j] = temp;
        }
        
        introspective_rec(arr, low, j, depth_limit - 1, threshold);
        introspective_rec(arr, j + 1, high, depth_limit - 1, threshold);
    }
}

void introspective_sort(int* arr, size_t size, int depth_limit, int threshold)
{
    if (size <= 1) return;
    introspective_rec(arr, 0, size - 1, depth_limit, threshold);
}