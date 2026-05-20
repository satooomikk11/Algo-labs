#include "sortings/radix_sort.h"
#include <stdlib.h>
#include <string.h>

#define BITS_PER_BYTE 8
#define BYTES_IN_INT  4
#define RADIX_SIZE    256

void radix_sort_lsd(int* arr, size_t size)
{
    if (size <= 1) return;
    
    int min = arr[0];
    for (size_t i = 1; i < size; i++)
    {
        if (arr[i] < min) min = arr[i];
    }
    
    if (min < 0)
    {
        for (size_t i = 0; i < size; i++)
        {
            arr[i] -= min;
        }
    }
    
    int* output = calloc(size, sizeof(int));
    int* count  = calloc(RADIX_SIZE, sizeof(int));
    int* buffer = calloc(size, sizeof(int));

    if (!output || !count || !buffer)
    {
        free(output);
        free(count);
        free(buffer);
        return;
    }
    
    int* src = arr;
    int* dst = output;
    
    for (int byte = 0; byte < BYTES_IN_INT; byte++)
    {
        memset(count, 0, RADIX_SIZE * sizeof(int));
        
        for (size_t i = 0; i < size; i++)
        {
            int digit = (src[i] >> (byte * BITS_PER_BYTE)) & (RADIX_SIZE - 1);
            count[digit]++;
        }
        
        for (int i = 1; i < RADIX_SIZE; i++)
        {
            count[i] += count[i - 1];
        }
        
        for (size_t i = size; i > 0; i--)
        {
            int digit = (src[i - 1] >> (byte * BITS_PER_BYTE)) & (RADIX_SIZE - 1);
            dst[--count[digit]] = src[i - 1];
        }
        
        int* temp = src;
                    src = dst;
                          dst = temp;
    }
    
    if (src != arr)
    {
        memcpy(arr, src, size * sizeof(int));
    }
    
    free(output);
    free(count);
    free(buffer);
    
    if (min < 0)
    {
        for (size_t i = 0; i < size; i++)
        {
            arr[i] += min;
        }
    }
}

static void radix_sort_msd_rec(int* arr, size_t size, int byte, 
                               int* count, int* output)
{
    if (size <= 1 || byte < 0) return;
    
    memset(count, 0, RADIX_SIZE * sizeof(int));
    
    for (size_t i = 0; i < size; i++)
    {
        int digit = (arr[i] >> (byte * BITS_PER_BYTE)) & (RADIX_SIZE - 1);
        count[digit]++;
    }
    
    for (int i = 1; i < RADIX_SIZE; i++)
    {
        count[i] += count[i - 1];
    }
    
    for (size_t i = size; i > 0; i--)
    {
        int digit = (arr[i - 1] >> (byte * BITS_PER_BYTE)) & (RADIX_SIZE - 1);
        output[--count[digit]] = arr[i - 1];
    }
    
    memcpy(arr, output, size * sizeof(int));
    
    int start = 0;
    for (int i = 0; i < RADIX_SIZE; i++)
    {
        size_t group_size = 0;
        for (size_t j = start; j < size && ((arr[j] >> (byte * BITS_PER_BYTE)) & (RADIX_SIZE - 1)) == i; j++)
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

void radix_sort_msd(int* arr, size_t size)
{
    if (size <= 1) return;
    
    int min = arr[0];
    for (size_t i = 1; i < size; i++)
    {
        if (arr[i] < min) min = arr[i];
    }
    
    if (min < 0)
    {
        for (size_t i = 0; i < size; i++)
        {
            arr[i] -= min;
        }
    }
    
    int* count  = calloc(RADIX_SIZE, sizeof(int));
    int* output = calloc(size, sizeof(int));
    
    if (!count || !output)
    {
        free(count);
        free(output);
        
        if (min < 0)
        {
            for (size_t i = 0; i < size; i++)
            {
                arr[i] += min;
            }
        }
        return;
    }
    
    radix_sort_msd_rec(arr, size, BYTES_IN_INT - 1, count, output);
    
    free(count);
    free(output);
    
    if (min < 0)
    {
        for (size_t i = 0; i < size; i++)
        {
            arr[i] += min;
        }
    }
}