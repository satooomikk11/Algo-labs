#ifndef SORTING_UTILS_H
#define SORTING_UTILS_H

#include <stddef.h>

double get_time_ms(void);
int  is_sorted(int* arr, size_t size);
void copy_array(int* dest, const int* src, size_t count);

#endif // SORTING_UTILS_H