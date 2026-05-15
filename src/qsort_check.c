#include <stdio.h>
#include <stdlib.h>

int cmp(const void* a, const void* b)
{
    return (*(int*)a > *(int*)b) - (*(int*)a < *(int*)b);
}

int main()
{
    size_t n = 0;
    if (scanf("%zu", &n) != 1) return 1;
    
    int* arr = calloc(n, sizeof(int));
    if (!arr) return 1;
    for (size_t i = 0; i < n; i++)
    {
        if (scanf("%d", &arr[i]) != 1) return 1;
    }
    
    qsort(arr, n, sizeof(int), cmp);
    
    printf("%zu\n", n);
    for (size_t i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    
    free(arr);
    return 0;
}