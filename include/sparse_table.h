#ifndef SPARSE_TABLE_H
#define SPARSE_TABLE_H

typedef struct
{
    long long** st;
    int n;             // размер массива
    int logn;          // floor(log2(n)) + 1
    int variant;
    int* fl_log;       // предподсчитанные floor(log2(k))
} SparseTable;

SparseTable* st_create_v1(long long* a, int n);
SparseTable* st_create_v2(long long* a, int n);
long long    st_query    (SparseTable* st, int l, int r);
void         st_free     (SparseTable* st);

#endif // SPARSE_TABLE_H
