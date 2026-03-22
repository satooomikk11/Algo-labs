#include "sortings/quick_sort_partition.h"
#include <stdlib.h>

static size_t partition_lomuto(int* arr, size_t low, size_t high)
{
    int pivot = arr[high];
    size_t i  = low;
    
    for (size_t j = low; j < high; j++)
    {
        if (arr[j] <= pivot)
        {
            int temp = arr[i];
                       arr[i] = arr[j];
                                arr[j] = temp;
            i++;
        }
    }
    
    int temp = arr[i];
               arr[i] = arr[high];
                        arr[high] = temp;
    
    return i;
}

static void quick_sort_lomuto_rec(int* arr, size_t low, size_t high)
{
    if (low >= high) return;
    
    size_t pi = partition_lomuto(arr, low, high);
    
    if (pi > low)  quick_sort_lomuto_rec(arr, low, pi - 1);
    if (pi < high) quick_sort_lomuto_rec(arr, pi + 1, high);
}

void quick_sort_lomuto(int* arr, size_t n)
{
    if (n <= 1) return;
    quick_sort_lomuto_rec(arr, 0, n - 1);
}

static size_t partition_hoare(int* arr, size_t low, size_t high)
{
    int pivot = arr[low + (high - low) / 2];
    size_t i = low  - 1;
    size_t j = high + 1;
    
    while (1)
    {
        do { i++; } while (arr[i] < pivot);
        do { j--; } while (arr[j] > pivot);
        
        if (i >= j) return j;
        
        int temp = arr[i];
                   arr[i] = arr[j];
                            arr[j] = temp;
    }
}

static void quick_sort_hoare_rec(int* arr, size_t low, size_t high)
{
    if (low >= high) return;
    
    size_t pi = partition_hoare(arr, low, high);
    quick_sort_hoare_rec(arr, low, pi);
    quick_sort_hoare_rec(arr, pi + 1, high);
}

void quick_sort_hoare(int* arr, size_t n)
{
    if (n <= 1) return;
    quick_sort_hoare_rec(arr, 0, n - 1);
}

static void partition_3way(int* arr, size_t low, size_t high, size_t* lt, size_t* gt)
{
    int pivot = arr[low];
    size_t i = low + 1;
    *lt = low;
    *gt = high;
    
    while (i <= *gt)
    {
        if (arr[i] < pivot)
        {
            int temp = arr[*lt];
                       arr[*lt] = arr[i];
                                  arr[i] = temp;
            (*lt)++;
            i++;
        }
        else if (arr[i] > pivot)
        {
            int temp = arr[*gt];
                       arr[*gt] = arr[i];
                                  arr[i] = temp;
            (*gt)--;
        }
        else
        {
            i++;
        }
    }
}

static void quick_sort_3way_rec(int* arr, size_t low, size_t high)
{
    if (low >= high) return;
    
    size_t lt = 0, gt = 0;
    partition_3way(arr, low, high, &lt, &gt);
    
    if (lt > low)  quick_sort_3way_rec(arr, low, lt - 1);
    if (gt < high) quick_sort_3way_rec(arr, gt + 1, high);
}

void quick_sort_3way(int* arr, size_t n)
{
    if (n <= 1) return;
    quick_sort_3way_rec(arr, 0, n - 1);
}