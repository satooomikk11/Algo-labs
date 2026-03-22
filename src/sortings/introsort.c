#include "sortings/introsort.h"
#include "sortings/quick_sort_partition.h"
#include "sortings/shell_sort.h"
#include <stddef.h>

static void introsort_rec(int* arr, size_t low, size_t high, int threshold)
{
    size_t n = high - low + 1;
    
    if (n <= (size_t)threshold)
    {
        shell_sort(arr + low, n);
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
        
        introsort_rec(arr, low, j, threshold);
        introsort_rec(arr, j + 1, high, threshold);
    }
}

void introsort(int* arr, size_t n, int threshold)
{
    if (n <= 1) return;
    introsort_rec(arr, 0, n - 1, threshold);
}