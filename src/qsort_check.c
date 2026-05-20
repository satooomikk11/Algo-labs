#include <stdio.h>
#include <stdlib.h>

int cmp(const void* a, const void* b)
{
    return (*(int*)a > *(int*)b) - (*(int*)a < *(int*)b);
}

int main()
{
    size_t size = 0;
    if (scanf("%zu", &size) != 1) return 1;
    
    int* arr = calloc(size, sizeof(int));
    if (!arr) return 1;
    for (size_t i = 0; i < size; i++)
    {
        if (scanf("%d", &arr[i]) != 1) return 1;
    }
    
    qsort(arr, size, sizeof(int), cmp);
    
    printf("%zu\n", size);
    for (size_t i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    
    free(arr);
    return 0;
}