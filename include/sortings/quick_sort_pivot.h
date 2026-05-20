#ifndef QUICK_SORT_PIVOT_H
#define QUICK_SORT_PIVOT_H

#include <stddef.h>

void quick_sort_central       (int* arr, size_t size);
void quick_sort_median3       (int* arr, size_t size);
void quick_sort_random        (int* arr, size_t size);
void quick_sort_median3_random(int* arr, size_t size);

#endif // QUICK_SORT_PIVOT_H