#include "../include/sparse_table.h"
#include <stdlib.h>
#include <math.h>
#include <assert.h>

static long long min_ll(long long first_value, long long second_value)
{
    return first_value < second_value ? first_value : second_value;
}

static int* precompute_fl_log(int max_value)
{
    assert(!(max_value < 0));
    
    int* floor_log_array = (int*)calloc((max_value + 1), sizeof(int));
    if (!floor_log_array) return NULL;
    
    if (max_value >= 1)
    {
        floor_log_array[1] = 0;
    }

    for (int current_index = 2; current_index <= max_value; current_index++)
    {
        floor_log_array[current_index] = floor_log_array[current_index / 2] + 1;
    }

    return floor_log_array;
}

SparseTable* st_create_v1(long long* source_array, int array_size)
{
    assert(source_array && array_size > 0);
    
    SparseTable* sparse_table = (SparseTable*)calloc(1, sizeof(SparseTable));
    if (!sparse_table) return NULL;
    
    sparse_table->storage_variant = 1;
    sparse_table->array_size = array_size;
    sparse_table->log_size   = (int)(log2(array_size)) + 1;
    sparse_table->floor_log_cache = precompute_fl_log(array_size);
    if (!sparse_table->floor_log_cache)
    {
        free(sparse_table);
        return NULL;
    }
    
    sparse_table->table_data = (long long**)calloc(array_size, sizeof(long long*));
    if (!sparse_table->table_data)
    {
        free(sparse_table->floor_log_cache);
        free(sparse_table);
        return NULL;
    }
    
    for (int row_index = 0; row_index < array_size; row_index++)
    {
        sparse_table->table_data[row_index] = (long long*)calloc(sparse_table->log_size, sizeof(long long));
        if (!sparse_table->table_data[row_index])
        {
            for (int prev_row = 0; prev_row < row_index; prev_row++) free(sparse_table->table_data[prev_row]);
            free(sparse_table->table_data);
            free(sparse_table->floor_log_cache);
            free(sparse_table);
            return NULL;
        }
        sparse_table->table_data[row_index][0] = source_array[row_index];
    }
    
    for (int power_index = 1; power_index < sparse_table->log_size; power_index++)
    {
        int step = 1 << (power_index - 1);
        for (int start_index = 0; start_index + (1 << power_index) <= array_size; start_index++)
        {
            sparse_table->table_data[start_index][power_index] = min_ll(
                sparse_table->table_data[start_index][power_index - 1], 
                sparse_table->table_data[start_index + step][power_index - 1]
            );
        }
    }
    
    return sparse_table;
}

SparseTable* st_create_v2(long long* source_array, int array_size)
{
    assert(source_array && array_size > 0);
    
    SparseTable* sparse_table = (SparseTable*)calloc(1, sizeof(SparseTable));
    if (!sparse_table) return NULL;
    
    sparse_table->storage_variant = 2;
    sparse_table->array_size = array_size;
    sparse_table->log_size = (int)(log2(array_size)) + 1;
    sparse_table->floor_log_cache = precompute_fl_log(array_size);
    if (!sparse_table->floor_log_cache)
    {
        free(sparse_table);
        return NULL;
    }
    
    sparse_table->table_data = (long long**)calloc(sparse_table->log_size, sizeof(long long*));
    if (!sparse_table->table_data)
    {
        free(sparse_table->floor_log_cache);
        free(sparse_table);
        return NULL;
    }
    
    for (int power_index = 0; power_index < sparse_table->log_size; power_index++)
    {
        sparse_table->table_data[power_index] = (long long*)calloc(array_size, sizeof(long long));
        if (!sparse_table->table_data[power_index])
        {
            for (int prev_power = 0; prev_power < power_index; prev_power++) free(sparse_table->table_data[prev_power]);
            free(sparse_table->table_data);
            free(sparse_table->floor_log_cache);
            free(sparse_table);
            return NULL;
        }
    }
    
    for (int element_index = 0; element_index < array_size; element_index++)
    {
        sparse_table->table_data[0][element_index] = source_array[element_index];
    }

    for (int power_index = 1; power_index < sparse_table->log_size; power_index++)
    {
        int step = 1 << (power_index - 1);
        for (int start_index = 0; start_index + (1 << power_index) <= array_size; start_index++)
        {
            sparse_table->table_data[power_index][start_index] = min_ll(
                sparse_table->table_data[power_index - 1][start_index], 
                sparse_table->table_data[power_index - 1][start_index + step]
            );
        }
    }
    
    return sparse_table;
}

long long st_query(SparseTable* sparse_table, int left_bound, int right_bound)
{
    assert(sparse_table && left_bound >= 0 && right_bound >= 0 && left_bound <= right_bound && right_bound < sparse_table->array_size);
    
    int segment_length = right_bound - left_bound + 1;
    int power_index = sparse_table->floor_log_cache[segment_length];
    
    if (sparse_table->storage_variant == 1)
        return min_ll(
            sparse_table->table_data[left_bound][power_index], 
            sparse_table->table_data[right_bound - (1 << power_index) + 1][power_index]
        );
    else
        return min_ll(
            sparse_table->table_data[power_index][left_bound], 
            sparse_table->table_data[power_index][right_bound - (1 << power_index) + 1]
        );
}

void st_free(SparseTable* sparse_table)
{
    if (!sparse_table) return;
    
    if (sparse_table->storage_variant == 1)
    {
        for (int row_index = 0; row_index < sparse_table->array_size; row_index++)
        {
            if (sparse_table->table_data && sparse_table->table_data[row_index]) free(sparse_table->table_data[row_index]);
        }
    }
    else
    {
        for (int power_index = 0; power_index < sparse_table->log_size; power_index++)
        {
            if (sparse_table->table_data && sparse_table->table_data[power_index]) free(sparse_table->table_data[power_index]);
        }
    }
    
    if (sparse_table->table_data)     free(sparse_table->table_data);
    if (sparse_table->floor_log_cache) free(sparse_table->floor_log_cache);
    free(sparse_table);
}
