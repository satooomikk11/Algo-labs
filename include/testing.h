#ifndef TESTING_H
#define TESTING_H

#include <stddef.h>

typedef void (*sort_func_t)(int* arr, size_t n);

typedef struct
{
    size_t size;        // размер массива
    double avg_time;    // среднее время сортировки
    int num_tests;      // количество успешных тестов
} test_result_t;

test_result_t* test_sorting(const char* tests_dir,
                            sort_func_t sort_func,
                            size_t from, 
                            size_t to, 
                            size_t step,
                            int num_tests_per_size,
                            size_t* result_count);

void save_results(const char* filename, 
                  test_result_t* results, 
                  size_t count);

void free_results(test_result_t* results);

#endif // TESTING_H