#include "sortings/merge_sort.h"
#include <stdlib.h>

static void merge(int* arr, int* temp, size_t left, size_t mid, size_t right)
{
    size_t i = left, j = mid + 1, k = left;
    
    while (i <= mid && j <= right)
    {
        if (arr[i] <= arr[j])
        {
            temp[k++] = arr[i++];
        }
        else
        {
            temp[k++] = arr[j++];
        }
    }
    
    while (i <= mid)   temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    
    for (size_t i = left; i <= right; i++)
    {
        arr[i] = temp[i];
    }
}

static void merge_sort_rec(int* arr, int* temp, size_t left, size_t right)
{
    if (left >= right) return;
    
    size_t mid = left + (right - left) / 2;
    merge_sort_rec(arr, temp, left, mid);
    merge_sort_rec(arr, temp, mid + 1, right);
    merge(arr, temp, left, mid, right);
}

void merge_sort_recursive(int* arr, size_t n)
{
    if (n <= 1) return;
    
    int* temp = calloc(n, sizeof(int));
    merge_sort_rec(arr, temp, 0, n - 1);
    free(temp);
}

void merge_sort_iterative(int* arr, size_t n)
{
    if (n <= 1) return;
    
    int* temp = calloc(n, sizeof(int));
    
    for (size_t width = 1; width < n; width *= 2)
    {
        for (size_t i = 0; i < n; i += 2 * width)
        {
            size_t left  = i;
            size_t mid   = (i + width - 1     < n - 1) ? i + width - 1     : n - 1;
            size_t right = (i + 2 * width - 1 < n - 1) ? i + 2 * width - 1 : n - 1;
            
            if (mid < right)
            {
                merge(arr, temp, left, mid, right);
            }
        }
    }
    
    free(temp);
}