#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

// игнорируем предупреждения
#pragma GCC diagnostic ignored "-Wunused-result"
#pragma GCC diagnostic ignored "-Wunused-variable"

#include "../include/naive_tree.h"
#include "../include/avl_tree.h"
#include "../include/treap.h"
#include "../include/splay_tree.h"
#include "../include/rb_tree.h"
#include "../include/b_tree.h"
#include "../include/skip_list.h"

#define TEST_REPEATS       5
#define START_SIZE         100000
#define STEP_SIZE          100000
#define MAX_SIZE           1000000
#define MAX_FILENAME_LEN   256

static double get_time_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0;
}

void generate_all_data()
{
    srand(42);
    system("mkdir -p data");
    
    printf("\nGenerating test data...\n");
    
    for (int size = START_SIZE; size <= MAX_SIZE; size += STEP_SIZE)
    {
        char filename[MAX_FILENAME_LEN];
        
        snprintf(filename, sizeof(filename), "data/random_keys_%d.bin", size);
        FILE* f = fopen(filename, "wb");
        if (f == NULL) return;
        
        int* keys = (int*)calloc(sizeof(int), size);
        for (int i = 0; i < size; i++) keys[i] = rand();
        fwrite(keys, sizeof(int), size, f);
        fclose(f);
        free(keys);
        
        snprintf(filename, sizeof(filename), "data/delete_keys_%d.bin", size);
        f = fopen(filename, "wb");
        if (f == NULL) return;
        
        srand(42);
        for (int i = 0; i < size / 2; i++)
        {
            int key = rand();
            fwrite(&key, sizeof(int), 1, f);
        }
        fclose(f);
        
        printf("Generated data for size %d\n", size);
    }
    
    FILE* f = fopen("data/sorted_keys_1000000.bin", "wb");
    if (f != NULL)
    {
        for (int i = 0; i < 1000000; i++)
        {
            int key = i;
            fwrite(&key, sizeof(int), 1, f);
        }
        fclose(f);
    }
    
    int* keys_100k = (int*)calloc(sizeof(int), 100000);
    int* keys_10k  = (int*)calloc(sizeof(int), 10000);
    
    if (keys_100k && keys_10k)
    {
        srand(42);
        for (int i = 0; i < 100000; i++) keys_100k[i] = rand();
        for (int i = 0; i < 10000;  i++) keys_10k[i]  = rand();
        
        f = fopen("data/naive_random_100k.bin", "wb");
        if (f) { fwrite(keys_100k, sizeof(int), 100000, f); fclose(f); }
        
        f = fopen("data/naive_delete_50k.bin", "wb");
        if (f) { fwrite(keys_100k, sizeof(int), 50000, f); fclose(f); }
        
        f = fopen("data/naive_sorted_10k.bin", "wb");
        if (f)
        {
            for (int i = 0; i < 10000; i++)
            {
                int key = i;
                fwrite(&key, sizeof(int), 1, f);
            }
            fclose(f);
        }
        
        f = fopen("data/naive_delete_sorted_5k.bin", "wb");
        if (f)
        {
            for (int i = 0; i < 5000; i++)
            {
                int key = i;
                fwrite(&key, sizeof(int), 1, f);
            }
            fclose(f);
        }
    }
    
    free(keys_100k);
    free(keys_10k);
    
    printf("All data generated successfully!\n\n");
}


void test_naive_tree()
{
    printf("Testing Naive Tree\n");
    
    double total_insert = 0, total_delete = 0;
    
    for (int rep = 0; rep < TEST_REPEATS; rep++)
    {
        FILE* f_insert = fopen("data/naive_random_100k.bin", "rb");
        FILE* f_delete = fopen("data/naive_delete_50k.bin",  "rb");
        
        if (f_insert == NULL || f_delete == NULL)
        {
            printf("Cannot open data files\n");
            return;
        }
        
        int* keys_insert = (int*)calloc(sizeof(int), 100000);
        int* keys_delete = (int*)calloc(sizeof(int), 50000);
        
        fread(keys_insert, sizeof(int), 100000, f_insert);
        fread(keys_delete, sizeof(int), 50000,  f_delete);
        fclose(f_insert);
        fclose(f_delete);
        
        NaiveNode* root = NULL;
        
        double start = get_time_ms();
        for (int i = 0; i < 100000; i++) root = naive_insert(root, keys_insert[i]);
        double end = get_time_ms();
        total_insert += (end - start);
        
        start = get_time_ms();
        for (int i = 0; i < 50000; i++) root  = naive_delete(root, keys_delete[i]);
        end = get_time_ms();
        total_delete += (end - start);
        
        naive_free(root);
        free(keys_insert);
        free(keys_delete);
    }
    
    printf("Random inserts (100k): %.2f ms\n",   total_insert / TEST_REPEATS);
    printf("Random deletes (50k):  %.2f ms\n\n", total_delete / TEST_REPEATS);
    
    total_insert = 0;
    total_delete = 0;
    
    for (int rep = 0; rep < TEST_REPEATS; rep++)
    {
        FILE* f_insert = fopen("data/naive_sorted_10k.bin",       "rb");
        FILE* f_delete = fopen("data/naive_delete_sorted_5k.bin", "rb");
        
        if (f_insert == NULL || f_delete == NULL) return;
        
        int* keys_insert = (int*)calloc(sizeof(int), 10000);
        int* keys_delete = (int*)calloc(sizeof(int), 5000);
        
        fread(keys_insert, sizeof(int), 10000, f_insert);
        fread(keys_delete, sizeof(int), 5000,  f_delete);
        fclose(f_insert);
        fclose(f_delete);
        
        NaiveNode* root = NULL;
        
        double start = get_time_ms();
        for (int i = 0; i < 10000; i++) root = naive_insert(root, keys_insert[i]);
        double end = get_time_ms();
        total_insert += (end - start);
        
        start = get_time_ms();
        for (int i = 0; i < 5000; i++) root  = naive_delete(root, keys_delete[i]);
        end = get_time_ms();
        total_delete += (end - start);
        
        naive_free(root);
        free(keys_insert);
        free(keys_delete);
    }
    
    printf("Sorted inserts (10k):  %.2f ms\n", total_insert / TEST_REPEATS);
    printf("Sorted deletes (5k):   %.2f ms\n", total_delete / TEST_REPEATS);
}

// обертки для AVL
void* avl_create_wrapper(void) 
{ 
    AVLNode** ptr = (AVLNode**)calloc(1, sizeof(AVLNode*));
    *ptr = NULL;
    return ptr;
}
void avl_insert_wrapper(void* tree, int key) 
{ 
    AVLNode** ptr = (AVLNode**)tree;
    *ptr = avl_insert(*ptr, key);
}
void avl_delete_wrapper(void* tree, int key) 
{ 
    AVLNode** ptr = (AVLNode**)tree;
    *ptr = avl_delete(*ptr, key);
}
void avl_free_wrapper(void* tree) 
{ 
    AVLNode** ptr = (AVLNode**)tree;
    avl_free(*ptr);
    free(tree);
}

// обертки для Treap
void* treap_create_wrapper(void) 
{ 
    TreapNode** ptr = (TreapNode**)calloc(1, sizeof(TreapNode*));
    *ptr = NULL;
    return ptr;
}
void treap_insert_wrapper(void* tree, int key) 
{ 
    TreapNode** ptr = (TreapNode**)tree;
    *ptr = treap_insert(*ptr, key);
}
void treap_delete_wrapper(void* tree, int key) 
{ 
    TreapNode** ptr = (TreapNode**)tree;
    *ptr = treap_delete(*ptr, key);
}
void treap_free_wrapper(void* tree) 
{ 
    TreapNode** ptr = (TreapNode**)tree;
    treap_free(*ptr);
    free(tree);
}

// обертки для Splay
void* splay_create_wrapper(void) 
{ 
    SplayNode** ptr = (SplayNode**)calloc(1, sizeof(SplayNode*));
    *ptr = NULL;
    return ptr;
}
void splay_insert_wrapper(void* tree, int key) 
{ 
    SplayNode** ptr = (SplayNode**)tree;
    *ptr = splay_insert(*ptr, key);
}
void splay_delete_wrapper(void* tree, int key) 
{ 
    SplayNode** ptr = (SplayNode**)tree;
    *ptr = splay_delete(*ptr, key);
}
void splay_free_wrapper(void* tree) 
{ 
    SplayNode** ptr = (SplayNode**)tree;
    splay_free(*ptr);
    free(tree);
}

// обертки для RB Tree
void* rb_create_wrapper(void) 
{ 
    return rb_create(); 
}
void rb_insert_wrapper(void* tree, int key) 
{ 
    rb_insert((RBTree*)tree, key); 
}
void rb_delete_wrapper(void* tree, int key) 
{ 
    rb_delete((RBTree*)tree, key); 
}
void rb_free_wrapper(void* tree) 
{ 
    rb_free((RBTree*)tree); 
}

// обертки для B-tree
void* b_create_wrapper(void) 
{ 
    return b_create(); 
}
void b_insert_wrapper(void* tree, int key) 
{ 
    b_insert((BTree*)tree, key); 
}
void b_delete_wrapper(void* tree, int key) 
{ 
    b_delete((BTree*)tree, key); 
}
void b_free_wrapper(void* tree) 
{ 
    b_free((BTree*)tree); 
}

// обертки для Skip List
void* sl_create_wrapper(void) 
{ 
    return sl_create(); 
}
void sl_insert_wrapper(void* list, int key) 
{ 
    sl_insert((SkipList*)list, key); 
}
void sl_delete_wrapper(void* list, int key) 
{ 
    sl_delete((SkipList*)list, key); 
}
void sl_free_wrapper(void* list) 
{ 
    sl_free((SkipList*)list); 
}

// тест AVL дерева с отсортированными данными
void test_avl_sorted()
{
    printf("Testing AVL with Sorted Data\n");
    
    FILE* f = fopen("results/avl_sorted_results.txt", "w");
    if (f == NULL)
    {
        printf("Cannot open results file for AVL sorted\n");
        return;
    }
    fprintf(f, "size insert_time_ms delete_time_ms\n");
    
    int sorted_sizes[] = {100000, 200000, 300000, 400000, 500000, 
                          600000, 700000, 800000, 900000, 1000000};
    int num_sizes = sizeof(sorted_sizes) / sizeof(sorted_sizes[0]);
    
    for (int idx = 0; idx < num_sizes; idx++)
    {
        int size = sorted_sizes[idx];
        double total_insert = 0, total_delete = 0;
        
        for (int rep = 0; rep < TEST_REPEATS; rep++)
        {
            AVLNode* root = NULL;
            
            double start = get_time_ms();
            for (int i = 0; i < size; i++)
            {
                root = avl_insert(root, i);
            }
            double end = get_time_ms();
            total_insert += (end - start);
            
            start = get_time_ms();
            for (int i = 0; i < size / 2; i++)
            {
                root = avl_delete(root, i);
            }
            end = get_time_ms();
            total_delete += (end - start);
            
            avl_free(root);
        }
        
        double avg_insert = total_insert / TEST_REPEATS;
        double avg_delete = total_delete / TEST_REPEATS;
        fprintf(f, "%d %.2f %.2f\n", size, avg_insert, avg_delete);
        printf("Size %d: Insert %.2f ms, Delete %.2f ms\n", size, avg_insert, avg_delete);
    }
    
    fclose(f);
}

// общая функция для тестирования дерева
void test_tree(const char* name, 
               void* (*create_func)(void),
               void  (*insert_func)(void*, int),
               void  (*delete_func)(void*, int),
               void  (*free_func)(void*))
{
    printf("\nTesting %s\n", name);
    
    char filename[MAX_FILENAME_LEN];
    snprintf(filename, sizeof(filename), "results/%s_results.txt", name);
    FILE* f = fopen(filename, "w");
    if (f == NULL)
    {
        printf("Cannot open results file\n");
        return;
    }
    fprintf(f, "size insert_time_ms delete_time_ms\n");
    
    for (int size = START_SIZE; size <= MAX_SIZE; size += STEP_SIZE)
    {
        double total_insert = 0, total_delete = 0;
        
        for (int rep = 0; rep < TEST_REPEATS; rep++)
        {
            char filepath[MAX_FILENAME_LEN];
            snprintf(filepath, sizeof(filepath), "data/random_keys_%d.bin", size);
            FILE* f_keys = fopen(filepath, "rb");
            
            snprintf(filepath, sizeof(filepath), "data/delete_keys_%d.bin", size);
            FILE* f_del  = fopen(filepath, "rb");
            
            if (f_keys == NULL || f_del == NULL)
            {
                printf("Cannot open data files for size %d\n", size);
                if (f_keys) fclose(f_keys);
                if (f_del)  fclose(f_del);
                fclose(f);
                return;
            }
            
            int* keys     = (int*)calloc(sizeof(int), size);
            int* del_keys = (int*)calloc(sizeof(int), (size / 2));
            
            if (keys == NULL || del_keys == NULL)
            {
                free(keys);
                free(del_keys);
                fclose(f_keys);
                fclose(f_del);
                fclose(f);
                return;
            }
            
            fread(keys,     sizeof(int), size,     f_keys);
            fread(del_keys, sizeof(int), size / 2, f_del);
            fclose(f_keys);
            fclose(f_del);
            
            void* tree = create_func();
            if (tree == NULL)
            {
                printf("Failed to create %s\n", name);
                free(keys);
                free(del_keys);
                fclose(f);
                return;
            }
            
            double start = get_time_ms();
            for (int i = 0; i < size; i++) insert_func(tree, keys[i]);
            double end = get_time_ms();
            total_insert += (end - start);
            
            start = get_time_ms();
            for (int i = 0; i < size / 2; i++) delete_func(tree, del_keys[i]);
            end = get_time_ms();
            total_delete += (end - start);
            
            free_func(tree);
            free(keys);
            free(del_keys);
        }
        
        double avg_insert = total_insert / TEST_REPEATS;
        double avg_delete = total_delete / TEST_REPEATS;
        fprintf(f, "%d %.2f %.2f\n", size, avg_insert, avg_delete);
        printf("Size %d: Insert %.2f ms, Delete %.2f ms\n", size, avg_insert, avg_delete);
    }
    
    fclose(f);
}

int main()
{
    system("mkdir -p data results plots");
    
    generate_all_data();
    
    test_naive_tree();
    
    test_tree("AVL",      avl_create_wrapper,   avl_insert_wrapper,   avl_delete_wrapper,   avl_free_wrapper);
    test_avl_sorted();
    
    test_tree("Treap",    treap_create_wrapper, treap_insert_wrapper, treap_delete_wrapper, treap_free_wrapper);
    test_tree("Splay",    splay_create_wrapper, splay_insert_wrapper, splay_delete_wrapper, splay_free_wrapper);
    test_tree("RB",       rb_create_wrapper,    rb_insert_wrapper,    rb_delete_wrapper,    rb_free_wrapper   );
    test_tree("B",        b_create_wrapper,     b_insert_wrapper,     b_delete_wrapper,     b_free_wrapper    );
    test_tree("SkipList", sl_create_wrapper,    sl_insert_wrapper,    sl_delete_wrapper,    sl_free_wrapper   );
    
    printf("\nAll tests completed!\n");
    printf("Results saved in results/ directory\n");
    
    return 0;
}
