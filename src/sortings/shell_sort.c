#include "sortings/shell_sort.h"

void shell_sort(int* arr, size_t n)
{
    if (arr == NULL || n <= 1)
    {
        return;
    }
    
    size_t h = 1;
    while (h < n / 3)
    {
        h = 3 * h + 1;
    }
    
    while (h >= 1)
    {
        for (size_t i = h; i < n; i++)
        {
            int temp = arr[i];
            size_t j = i;
            while (j >= h && arr[j - h] > temp)
            {
                arr[j] = arr[j - h];
                j -= h;
            }
            arr[j] = temp;
        }
        h /= 3;
    }
}