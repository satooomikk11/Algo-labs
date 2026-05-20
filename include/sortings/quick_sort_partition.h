#ifndef QUICK_SORT_H
#define QUICK_SORT_H

#include <stddef.h>

void quick_sort_lomuto(int* arr, size_t size);
void quick_sort_hoare (int* arr, size_t size);
void quick_sort_3way  (int* arr, size_t size);

#endif // QUICK_SORT_H