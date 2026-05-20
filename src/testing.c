#include "testing.h"
#include "sorting_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_FILENAME_LEN 512

static int* read_array_from_file(const char* filename, size_t* size)
{
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        return NULL;
    }
    
    size_t arr_size = 0;
    if (fscanf(file, "%zu", &arr_size) != 1)
    {
        fclose(file);
        return NULL;
    }

    if (arr_size  == 0)
    {
        *size = 0;
        fclose(file);
        int* arr = malloc(0);
        return arr;
    }
    
    int* arr = calloc(arr_size, sizeof(int));
    if (!arr) return 0;
    
    for (size_t i = 0; i < arr_size; i++)
    {
        if (fscanf(file, "%d", &arr[i]) != 1)
        {
            free(arr);
            fclose(file);
            return NULL;
        }
    }
    
    fclose(file);
    *size = arr_size;
    return arr;
}

static int test_single_case(const char* test_dir, 
                            size_t size, 
                            int test_num,
                            sort_func_t sort_func,
                            double* time_ms)
{
    char in_filename [MAX_FILENAME_LEN] = "";
    char out_filename[MAX_FILENAME_LEN] = "";
    
    snprintf(in_filename,  sizeof(in_filename),  "%s/%zu_%d.in",  test_dir, size, test_num);
    snprintf(out_filename, sizeof(out_filename), "%s/%zu_%d.out", test_dir, size, test_num);
    
    size_t arr_size = 0;
    int* arr = read_array_from_file(in_filename, &arr_size);
    if (!arr)
    {
        return 0;
    }
    
    size_t expected_arr_size = 0;
    int* expected = read_array_from_file(out_filename, &expected_arr_size);
    if (!expected || arr_size != expected_arr_size)
    {
        free(arr);
        if (expected) free(expected);
        return 0;
    }
    
    int* arr_copy = calloc(arr_size, sizeof(int));
    if (!arr_copy)
    {
        free(arr);
        free(expected);
        return 0;
    }

    copy_array(arr_copy, arr, arr_size);
    
    double start_time = get_time_ms();
    sort_func(arr_copy, arr_size);
    double end_time = get_time_ms();
    
    *time_ms = end_time - start_time;
    
    // сравниваем с ожидаемым результатом
    for (size_t i = 0; i < arr_size; i++)
    {
        assert(arr_copy[i] == expected[i] && "Element mismatch!");
    }
    assert(is_sorted(arr_copy, arr_size) && "Array is not sorted!");
    
    free(arr);
    free(expected);
    free(arr_copy);
    
    return 1; // тест пройден
}

test_result_t* test_sorting(const char* tests_dir,
                            sort_func_t sort_func,
                            size_t from, 
                            size_t to, 
                            size_t step,
                            int num_tests_per_size,
                            size_t* result_count)
{
    size_t num_sizes = ((to - from) / step) + 1;
    test_result_t* results = calloc(num_sizes, sizeof(test_result_t));
    if (!results)
    {
        fprintf(stderr, "Error: Memory allocation failed\n");
        *result_count = 0;
        return NULL;
    }
    *result_count = num_sizes;
    
    printf("Testing sorting function on %zu different sizes...\n", num_sizes);
    
    size_t result_idx = 0;
    for (size_t size = from; size <= to; size += step)
    {
        printf("  Testing size = %zu ", size);
        fflush(stdout);
        
        double total_time = 0.0;
        int valid_tests   = 0;
        
        for (int test_num = 0; test_num < num_tests_per_size; test_num++)
        {
            double time_ms = 0.0;
            if (test_single_case(tests_dir, size, test_num, sort_func, &time_ms))
            {
                total_time += time_ms;
                valid_tests++;
            }
        }
        
        if (valid_tests > 0)
        {
            results[result_idx].size      = size;
            results[result_idx].avg_time  = total_time / valid_tests;
            results[result_idx].num_tests = valid_tests;
            printf("OK (avg = %.3f ms, tests = %d)\n", 
                   results[result_idx].avg_time, valid_tests);
        }
        else
        {
            results[result_idx].size      = size;
            results[result_idx].avg_time  = 0.0;
            results[result_idx].num_tests = 0;
            printf("NO TESTS FOUND\n");
        }
        
        result_idx++;
    }
    
    return results;
}

// результаты в CSV файле
void save_results(const char* filename, 
                  test_result_t* results, 
                  size_t count)
{
    FILE* file = fopen(filename, "w");
    if (!file)
    {
        fprintf(stderr, "Error: Cannot open file %s for writing\n", filename);
        return;
    }
    
    fprintf(file, "size,time_ms,num_tests\n");
    
    for (size_t i = 0; i < count; i++)
    {
        fprintf(file, "%zu,%.6f,%d\n", 
                results[i].size, 
                results[i].avg_time,
                results[i].num_tests);
    }
    
    fclose(file);
    printf("Results saved to %s\n", filename);
}

void free_results(test_result_t* results)
{
    free(results);
}