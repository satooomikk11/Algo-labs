#include "sortings/radix_sort.h"
#include <stdlib.h>
#include <string.h>

void radix_sort_lsd(int* arr, size_t n)
{
    if (n <= 1) return;
    
    int* output = calloc(n,   sizeof(int));
    int* count  = calloc(256, sizeof(int));
    int* buffer = calloc(n,   sizeof(int));

    if (!output || !count || !buffer)
    {
        free(output);
        free(count);
        return;
    }
    
    int* src = arr;
    int* dst = output;
    
    for (int byte = 0; byte < 4; byte++)
    {
        memset(count, 0, 256 * sizeof(int));
        
        for (size_t i = 0; i < n; i++)
        {
            int digit = (src[i] >> (byte * 8)) & 0xFF;
            count[digit]++;
        }
        
        for (int i = 1; i < 256; i++)
        {
            count[i] += count[i - 1];
        }
        
        for (size_t i = n; i > 0; i--)
        {
            int digit = (src[i - 1] >> (byte * 8)) & 0xFF;
            dst[--count[digit]] = src[i - 1];
        }
        
        int* temp = src;
                    src = dst;
                          dst = temp;
    }
    
    if (src != arr)
    {
        memcpy(arr, src, n * sizeof(int));
    }
    
    free(output);
    free(count);
    free(buffer);
}

static void radix_sort_msd_rec(int* arr, size_t n, int byte, 
                               int* count, int* output)
{
    if (n <= 1 || byte < 0) return;
    
    memset(count, 0, 256 * sizeof(int));
    
    for (size_t i = 0; i < n; i++)
    {
        int digit = (arr[i] >> (byte * 8)) & 0xFF;
        count[digit]++;
    }
    
    for (int i = 1; i < 256; i++)
    {
        count[i] += count[i - 1];
    }
    
    for (size_t i = n; i > 0; i--)
    {
        int digit = (arr[i - 1] >> (byte * 8)) & 0xFF;
        output[--count[digit]] = arr[i - 1];
    }
    
    memcpy(arr, output, n * sizeof(int));
    
    int start = 0;
    for (int i = 0; i < 256; i++)
    {
        size_t group_size = 0;
        for (size_t j = start; j < n && ((arr[j] >> (byte * 8)) & 0xFF) == i; j++)
        {
            group_size++;
        }
        
        if (group_size > 1)
        {
            radix_sort_msd_rec(arr + start, group_size, byte - 1, count, output);
        }
        
        start += group_size;
    }
}

void radix_sort_msd(int* arr, size_t n)
{
    if (n <= 1) return;
    
    int* count  = calloc(256, sizeof(int));
    int* output = calloc(n,   sizeof(int));
    
    if (!count || !output)
    {
        free(count);
        free(output);
        return;
    }
    
    radix_sort_msd_rec(arr, n, 3, count, output);
    
    free(count);
    free(output);
}