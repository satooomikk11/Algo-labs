#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "binomial_heap.h"

#define SEED 42
#define TIME_MEASURE_ERROR -1LL

int int_compare(const void* a, const void* b)
{
    int ia = *(int*)a;
    int ib = *(int*)b;
    return ia - ib;
}

long long measure_binomial_build_time(int n)
{
    assert(n > 0);

    int* values = (int*)calloc(n, sizeof(int));
    if (!values) return TIME_MEASURE_ERROR;
    
    srand(SEED);
    for (int i = 0; i < n; i++)
    {
        values[i] = rand() % 1000000;
    }
    
    BinomialHeap heap;
    binomial_heap_init(&heap, sizeof(int), int_compare);
    
    clock_t start = clock();
    
    for (int i = 0; i < n; i++)
    {
        Status status = binomial_heap_insert(&heap, &values[i]);
        if (status != OK)
        {
            binomial_heap_clear(&heap);
            free(values);
            return TIME_MEASURE_ERROR;
        }
    }
    
    clock_t end = clock();
    
    long long time_us = (end - start) * 1000000LL / CLOCKS_PER_SEC;
    
    binomial_heap_clear(&heap);
    free(values);
    
    return time_us;
}

int main()
{
    printf("Построение биномиальной кучи\n");
    printf("Размер\tВремя\n");
    
    for (int size = 100000; size <= 1000000; size += 100000)
    {
        fflush(stdout);
        
        long long build_time = measure_binomial_build_time(size);
        assert(build_time != TIME_MEASURE_ERROR);
        
        printf("%d\t%lld\n", size, build_time);
        fflush(stdout);
    }
    
    return 0;
}
