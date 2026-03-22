#include "sortings/quick_sort_pivot.h"
#include <stdlib.h>
#include <time.h>

static size_t partition_hoare(int* arr, size_t low, size_t high, int pivot)
{
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

static void quick_sort_central_rec(int* arr, size_t low, size_t high)
{
    if (low >= high) return;
    
    int pivot = arr[low + (high - low) / 2];
    size_t pi = partition_hoare(arr, low, high, pivot);
    
    quick_sort_central_rec(arr, low, pi);
    quick_sort_central_rec(arr, pi + 1, high);
}

void quick_sort_central(int* arr, size_t n)
{
    if (n <= 1) return;
    quick_sort_central_rec(arr, 0, n - 1);
}

static int median_of_three(int* arr, size_t low, size_t high)
{
    size_t mid = low + (high - low) / 2;
    
    if (arr[low] > arr[mid])
    {
        int temp = arr[low];
                   arr[low] = arr[mid];
                              arr[mid] = temp;
    }
    if (arr[low] > arr[high])
    {
        int temp = arr[low];
        arr[low] = arr[high];
        arr[high] = temp;
    }
    if (arr[mid] > arr[high])
    {
        int temp = arr[mid];
                   arr[mid] = arr[high];
                              arr[high] = temp;
    }
    
    return arr[mid];
}

static void quick_sort_median3_rec(int* arr, size_t low, size_t high)
{
    if (low >= high) return;
    
    int pivot = median_of_three(arr, low, high);
    size_t pi = partition_hoare(arr, low, high, pivot);
    
    quick_sort_median3_rec(arr, low, pi);
    quick_sort_median3_rec(arr, pi + 1, high);
}

void quick_sort_median3(int* arr, size_t n)
{
    if (n <= 1) return;
    quick_sort_median3_rec(arr, 0, n - 1);
}

static int random_pivot(int* arr, size_t low, size_t high)
{
    static int initialized = 0;
    if (!initialized)
    {
        srand(time(NULL));
        initialized = 1;
    }
    
    size_t pivot_idx = low + rand() % (high - low + 1);
    return arr[pivot_idx];
}

static void quick_sort_random_rec(int* arr, size_t low, size_t high)
{
    if (low >= high) return;
    
    int pivot = random_pivot(arr, low, high);
    size_t pi = partition_hoare(arr, low, high, pivot);
    
    quick_sort_random_rec(arr, low, pi);
    quick_sort_random_rec(arr, pi + 1, high);
}

void quick_sort_random(int* arr, size_t n)
{
    if (n <= 1) return;
    quick_sort_random_rec(arr, 0, n - 1);
}

static int median_of_three_random(int* arr, size_t low, size_t high)
{
    static int initialized = 0;
    if (!initialized)
    {
        srand(time(NULL));
        initialized = 1;
    }
    
    size_t idx1 = low + rand() % (high - low + 1);
    size_t idx2 = low + rand() % (high - low + 1);
    size_t idx3 = low + rand() % (high - low + 1);
    
    int a = arr[idx1], b = arr[idx2], c = arr[idx3];
    
    if ((a <= b && b <= c) || (c <= b && b <= a)) return b;
    if ((b <= a && a <= c) || (c <= a && a <= b)) return a;
    return c;
}

static void quick_sort_median3_random_rec(int* arr, size_t low, size_t high)
{
    if (low >= high) return;
    
    int pivot = median_of_three_random(arr, low, high);
    size_t pi = partition_hoare(arr, low, high, pivot);
    
    quick_sort_median3_random_rec(arr, low, pi);
    quick_sort_median3_random_rec(arr, pi + 1, high);
}

void quick_sort_median3_random(int* arr, size_t n)
{
    if (n <= 1) return;
    quick_sort_median3_random_rec(arr, 0, n - 1);
}