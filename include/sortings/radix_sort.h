#ifndef RADIX_SORT_H
#define RADIX_SORT_H

#include <stddef.h>

void radix_sort_lsd(int* arr, size_t size);
void radix_sort_msd(int* arr, size_t size);

#endif // RADIX_SORT_H