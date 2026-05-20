#include "sortings/selection_sort.h"

void selection_sort(int* arr, size_t size)
{
    if (arr == NULL || size <= 1)
    {
        return;
    }
    
    for (size_t i = 0; i < size - 1; i++)
    {
        size_t min_idx = i;
        for (size_t j = i + 1; j < size; j++)
        {
            if (arr[j] < arr[min_idx])
            {
                min_idx = j;
            }
        }
        if (min_idx != i)
        {
            int temp = arr[i];
                       arr[i] = arr[min_idx];
                                arr[min_idx] = temp;
        }
    }
}