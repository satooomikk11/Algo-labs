#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "binary_heap.h"

#define SEED 42
#define TIME_MEASURE_ERROR -1LL

long long measure_linear_build(int n)
{
    assert(n > 0 && n <= MAX_HEAP_SIZE);

    long long* values = (long long*)calloc(n, sizeof(long long));
    if (!values) return TIME_MEASURE_ERROR;
    
    srand(SEED);
    for (int i = 0; i < n; i++)
    {
        values[i] = rand() % 1000000;
    }
    
    BinaryHeap heap;
    
    clock_t start = clock();
    binary_heap_linear(&heap, values, n);
    clock_t end = clock();
    
    assert(binary_heap_is_valid(&heap) == OK);
    
    long long time_us = (end - start) * 1000000LL / CLOCKS_PER_SEC;
    
    free(values);
    return time_us;
}

long long measure_insertions_build(int n)
{
    assert(n > 0 && n <= MAX_HEAP_SIZE);

    long long* values = (long long*)calloc(n, sizeof(long long));
    if (!values) return TIME_MEASURE_ERROR;
    
    srand(SEED);
    for (int i = 0; i < n; i++)
    {
        values[i] = rand() % 1000000;
    }
    
    BinaryHeap heap;
    
    clock_t start = clock();
    binary_heap_insertions(&heap, values, n);
    clock_t end = clock();
    
    assert(binary_heap_is_valid(&heap) == OK);
    
    long long time_us = (end - start) * 1000000LL / CLOCKS_PER_SEC;
    
    free(values);
    return time_us;
}

int main()
{
    printf("Построение бинарной кучи\n");
    printf("Размер\tЛинейный\tВставки\tВыигрыш\n");
    
    for (int size = 100000; size <= 1000000; size += 100000)
    {
        fflush(stdout);
        
        long long linear_time = measure_linear_build(size);
        assert(linear_time != TIME_MEASURE_ERROR);
        
        long long insertions_time = measure_insertions_build(size);
        assert(insertions_time != TIME_MEASURE_ERROR);
        
        double speedup = (double)insertions_time / linear_time;
        
        printf("%d\t%lld\t\t%lld\t\t%.2f\n", 
               size, linear_time, insertions_time, speedup);
        fflush(stdout);
    }
    
    return 0;
}
