#include "sortings/bubble_sort.h"

void bubble_sort(int* arr, size_t n)
{
    if (arr == NULL || n <= 1)
    {
        return;
    }

    for (size_t i = 0; i < n - 1; i++)
    {
        int swapped = 0;
        for (size_t j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                        arr[j] = arr[j + 1];
                                 arr[j + 1] = temp;
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}