#ifndef SPARSE_TABLE_H
#define SPARSE_TABLE_H

typedef struct
{
    long long** table_data;
    int array_size;          // размер массива
    int log_size;            // floor(log2(n)) + 1
    int storage_variant;
    int* floor_log_cache;    // предподсчитанные floor(log2(k))
} SparseTable;

SparseTable* st_create_v1(long long* source_array, int array_size);
SparseTable* st_create_v2(long long* source_array, int array_size);
long long    st_query    (SparseTable* sparse_table, int left_bound, int right_bound);
void         st_free     (SparseTable* sparse_table);

#endif // SPARSE_TABLE_H
