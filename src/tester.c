#include "testing.h"
#include "sorting_utils.h"
#include "sortings/insertion_sort.h"
#include "sortings/bubble_sort.h"
#include "sortings/selection_sort.h"
#include "sortings/shell_sort.h"
#include "sortings/heap_sort.h"
#include "sortings/merge_sort.h"
#include "sortings/quick_sort_partition.h"
#include "sortings/quick_sort_pivot.h"
#include "sortings/introsort.h"
#include "sortings/introspective.h"
#include "sortings/radix_sort.h"
#include <stdio.h>
#include <stdlib.h>

// изменить 1/0 чтобы включить/выключить пункты
#define RUN_POINT_1   1
#define RUN_POINT_2   1
#define RUN_POINT_3   1
#define RUN_POINT_4   1
#define RUN_POINT_5   1
#define RUN_POINT_6   1
#define RUN_POINT_7   1
#define RUN_POINT_9   1
#define RUN_POINT_10  1

static int g_heap_k;
static int g_introsort_threshold;
static int g_introspective_depth;
static int g_introspective_threshold;
static int g_introspective_c;


void heap_sort_wrapper(int* arr, size_t n)
{
    heap_sort_k(arr, n, g_heap_k);
}

int qsort_cmp(const void* a, const void* b)
{
    return *(int*)a - *(int*)b;
}

void qsort_wrapper(int* arr, size_t n)
{
    qsort(arr, n, sizeof(int), qsort_cmp);
}

void introsort_wrapper(int* arr, size_t n)
{
    introsort(arr, n, g_introsort_threshold);
}

void introspective_wrapper(int* arr, size_t n)
{
    introspective_sort(arr, n, g_introspective_depth, g_introspective_threshold);
}

int main()
{
    test_result_t* results;
    size_t count = 0;

    #if RUN_POINT_1
    printf("\nПункт 1: Квадратичные (и не совсем) сортировки\n");
    
    results = test_sorting("small_tests", insertion_sort, 0, 1000, 50, 5, &count);
    save_results("results/insertion_small.csv", results, count);
    free_results(results);
    
    results = test_sorting("small_tests", bubble_sort,    0, 1000, 50, 5, &count);
    save_results("results/bubble_small.csv",    results, count);
    free_results(results);
    
    results = test_sorting("small_tests", selection_sort, 0, 1000, 50, 5, &count);
    save_results("results/selection_small.csv", results, count);
    free_results(results);
    
    results = test_sorting("small_tests", shell_sort,     0, 1000, 50, 5, &count);
    save_results("results/shell_small.csv",     results, count);
    free_results(results);
    #endif

    #if RUN_POINT_2
    printf("\nПункт 2: Пирамидальные сортировки\n");
    
    for (int k = 2; k <= 10; k++)
    {
        g_heap_k = k;
        results = test_sorting("big_tests", heap_sort_wrapper, 0, 1000000, 10000, 5, &count);
        char filename[256] = "";
        snprintf(filename, sizeof(filename), "results/heap_k%d_big.csv", k);
        save_results(filename, results, count);
        free_results(results);
    }
    #endif

    #if RUN_POINT_3
    printf("\nПункт 3: Сортировки слиянием\n");
    
    results = test_sorting("big_tests", merge_sort_recursive, 0, 1000000, 10000, 5, &count);
    save_results("results/merge_recursive_big.csv", results, count);
    free_results(results);
    
    results = test_sorting("big_tests", merge_sort_iterative, 0, 1000000, 10000, 5, &count);
    save_results("results/merge_iterative_big.csv", results, count);
    free_results(results);
    #endif

    #if RUN_POINT_4
    printf("\nПункт 4: Быстрые сортировки (партиционирования и оптимизации)\n");
    
    results = test_sorting("big_tests", quick_sort_lomuto, 0, 1000000, 10000, 5, &count);
    save_results("results/quick_lomuto_big.csv", results, count);
    free_results(results);
    
    results = test_sorting("big_tests", quick_sort_hoare,  0, 1000000, 10000, 5, &count);
    save_results("results/quick_hoare_big.csv", results, count);
    free_results(results);
    
    results = test_sorting("big_tests", quick_sort_3way,   0, 1000000, 10000, 5, &count);
    save_results("results/quick_3way_big.csv", results, count);
    free_results(results);
    
    results = test_sorting("test_most_dublicates", quick_sort_lomuto, 0, 1000000, 10000, 5, &count);
    save_results("results/quick_lomuto_dub.csv", results, count);
    free_results(results);
    
    results = test_sorting("test_most_dublicates", quick_sort_hoare,  0, 1000000, 10000, 5, &count);
    save_results("results/quick_hoare_dub.csv", results, count);
    free_results(results);
    
    results = test_sorting("test_most_dublicates", quick_sort_3way,   0, 1000000, 10000, 5, &count);
    save_results("results/quick_3way_dub.csv", results, count);
    free_results(results);
    #endif

    #if RUN_POINT_5
    printf("\nПункт 5: Быстрые сортировки (разные стратегии выбора, прочее)\n");
    
    results = test_sorting("big_tests", quick_sort_central, 0, 1000000, 10000, 5, &count);
    save_results("results/quick_central_big.csv", results, count);
    free_results(results);
    
    results = test_sorting("big_tests", quick_sort_median3, 0, 1000000, 10000, 5, &count);
    save_results("results/quick_median3_big.csv", results, count);
    free_results(results);
    
    results = test_sorting("big_tests", quick_sort_random,  0, 1000000, 10000, 5, &count);
    save_results("results/quick_random_big.csv", results, count);
    free_results(results);

    results = test_sorting("big_tests", quick_sort_median3_random, 0, 1000000, 10000, 5, &count);
    save_results("results/quick_median3_random_big.csv", results, count);
    free_results(results);
    #endif

    #if RUN_POINT_6
    printf("\nПункт 6: Выбор размера блоков для Introsort\n");

    for (int threshold = 5; threshold <= 50; threshold += 5)
    {
        printf("  Testing threshold = %d...\n", threshold);
        
        g_introsort_threshold = threshold;
        results = test_sorting("big_tests", introsort_wrapper, 10000, 1000000, 10000, 5, &count);
        
        char filename[256] = "";
        snprintf(filename, sizeof(filename), "results/introsort_t%d_big.csv", threshold);
        save_results(filename, results, count);
        free_results(results);
    }

    printf("  Testing quick_sort_hoare (baseline)...\n");
    results = test_sorting("big_tests", quick_sort_hoare, 10000, 1000000, 10000, 5, &count);
    save_results("results/quick_hoare_baseline_big.csv", results, count);
    free_results(results);
    #endif

    #if RUN_POINT_7
    printf("\nПункт 7: Introspective Sorting\n");

    int optimal_threshold = 40;

    for (int c = 1; c <= 5; c++)
    {
        int depth_limit = c * 20; // log2(1000000) = 20 примерно
        printf("    Testing C = %d (depth_limit = %d)...\n", c, depth_limit);
        
        g_introspective_depth = depth_limit;
        g_introspective_threshold = optimal_threshold;
        
        results = test_sorting("big_tests", introspective_wrapper, 10000, 1000000, 10000, 5, &count);
        
        char filename[256] = "";
        snprintf(filename, sizeof(filename), "results/introspective_c%d_big.csv", c);
        save_results(filename, results, count);
        free_results(results);
    }

    printf("  Testing quick_sort_central (baseline)...\n");
    results = test_sorting("big_tests", quick_sort_central, 10000, 1000000, 10000, 5, &count);
    save_results("results/quick_central_baseline_big.csv", results, count);
    free_results(results);

    printf("  Testing introspective_sort with optimal C...\n");
    int optimal_c = 2;  // экспериментально получилось C = 2
    int optimal_depth = optimal_c * 20;
    g_introspective_depth = optimal_depth;
    g_introspective_threshold = optimal_threshold;

    results = test_sorting("big_tests", introspective_wrapper, 10000, 1000000, 10000, 5, &count);
    save_results("results/introspective_optimal_big.csv", results, count);
    free_results(results);
    #endif

    #if RUN_POINT_9
    printf("\nПункт 9: Сортировки, основанные не на сравнениях\n");
    
    results = test_sorting("big_tests", radix_sort_lsd, 0, 1000000, 10000, 5, &count);
    save_results("results/radix_lsd_big.csv", results, count);
    free_results(results);
    
    results = test_sorting("big_tests", radix_sort_msd, 0, 1000000, 10000, 5, &count);
    save_results("results/radix_msd_big.csv", results, count);
    free_results(results);
    #endif

    #if RUN_POINT_10
    printf("\nПункт 10: Сравнение лучших сортировок\n");

    // 1. Shell Sort
    printf("  Testing Shell Sort...\n");
    results = test_sorting("big_tests", shell_sort, 10000, 1000000, 10000, 5, &count);
    save_results("results/best_shell_big.csv", results, count);
    free_results(results);

    // 2. k = 4
    printf("  Testing Heap Sort (k = 4)...\n");
    g_heap_k = 4;
    results = test_sorting("big_tests", heap_sort_wrapper, 10000, 1000000, 10000, 5, &count);
    save_results("results/best_heap_k5_big.csv", results, count);
    free_results(results);

    // 3. итеративная
    printf("  Testing Iterative Merge Sort...\n");
    results = test_sorting("big_tests", merge_sort_iterative, 10000, 1000000, 10000, 5, &count);
    save_results("results/best_merge_iterative_big.csv", results, count);
    free_results(results);

    // 4. сортировка Хоара
    printf("  Testing Quick Sort (Hoare)...\n");
    results = test_sorting("big_tests", quick_sort_hoare, 10000, 1000000, 10000, 5, &count);
    save_results("results/best_quick_hoare_big.csv", results, count);
    free_results(results);

    // 5. pivot - центральный элемент
    printf("  Testing Quick Sort (central pivot)...\n");
    results = test_sorting("big_tests", quick_sort_central, 10000, 1000000, 10000, 5, &count);
    save_results("results/best_quick_central_big.csv", results, count);
    free_results(results);

    // 6. threshold = 40
    printf("  Testing Introsort (threshold=40)...\n");
    g_introsort_threshold = 40;
    results = test_sorting("big_tests", introsort_wrapper, 10000, 1000000, 10000, 5, &count);
    save_results("results/best_introsort_t40_big.csv", results, count);
    free_results(results);

    // 7. C = 2
    printf("  Testing Introspective Sort (C = 2)...\n");
    g_introspective_c = 2;
    g_introspective_threshold = 20;
    results = test_sorting("big_tests", introspective_wrapper, 10000, 1000000, 10000, 5, &count);
    save_results("results/best_introspective_c2_big.csv", results, count);
    free_results(results);

    // 9. LSD
    printf("  Testing LSD Radix Sort...\n");
    results = test_sorting("big_tests", radix_sort_lsd, 10000, 1000000, 10000, 5, &count);
    save_results("results/best_radix_lsd_big.csv", results, count);
    free_results(results);

    // 10. qsort для сравнения
    printf("  Testing qsort (standard library)...\n");
    results = test_sorting("big_tests", qsort_wrapper, 10000, 1000000, 10000, 5, &count);
    save_results("results/qsort_big.csv", results, count);
    free_results(results);
    #endif

    printf("\nРезультаты в папке results/\n");
    
    return 0;
}