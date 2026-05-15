#ifndef HEAP_SORT_H
#define HEAP_SORT_H

#include <stddef.h>

void bottom_up_heapify(int arr[], size_t n, size_t i, int k);
void build_k_heap     (int arr[], size_t n, int k);
void heap_sort_k      (int arr[], size_t n, int k);

#endif // HEAP_SORT_H