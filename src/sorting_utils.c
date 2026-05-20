#include "sorting_utils.h"
#include <sys/time.h>
#include <string.h>

double get_time_ms()
{
    struct timeval tv = {};
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0;
}

int is_sorted(int* arr, size_t size)
{
    for (size_t i = 1; i < size; i++)
    {
        if (arr[i] < arr[i-1])
        {
            return 0;
        }
    }
    return 1;
}

void copy_array(int* dest, const int* src, size_t count)
{
    memcpy(dest, src, count * sizeof(int));
}