#include <stdio.h>
#include <stdlib.h>

int cmp(const void* a, const void* b)
{
    return *(int*)a - *(int*)b;
}

int main()
{
    size_t n = 0;
    scanf("%zu", &n);
    
    int* arr = calloc(n, sizeof(int));
    for (size_t i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
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