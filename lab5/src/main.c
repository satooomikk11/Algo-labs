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

        snprintf(filename, sizeof(filename), "data/keys_%d.bin", size);
        FILE* f = fopen(filename, "wb");
        if (f == NULL) return;
        
        int* keys = (int*)calloc(sizeof(int), size);
        for (int i = 0; i < size; i++) keys[i] = rand();
        
        fwrite(keys, sizeof(int), size, f);
        fclose(f);
        
        printf("Generated data for size %d\n", size);
        free(keys);
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
    
    printf("All data generated successfully!\n\n");
}

int* load_keys(const char* filename, int* count)
{
    FILE* f = fopen(filename, "rb");
    if (f == NULL) return NULL;
    
    int* keys = (int*)calloc(sizeof(int), (*count));
    if (keys == NULL)
    {
        fclose(f);
        return NULL;
    }
    
    fread(keys, sizeof(int), *count, f);
    fclose(f);
    
    return keys;
}

void test_naive_tree()
{
    printf("Testing Naive Tree\n");
    
    int size = 100000;
    int* keys = load_keys("data/keys_100000.bin", &size);
    if (keys == NULL)
    {
        printf("Cannot load keys\n");
        return;
    }
    
    double total_insert = 0, total_delete = 0;
    
    for (int rep = 0; rep < TEST_REPEATS; rep++)
    {
        NaiveNode* root = NULL;
        
        double start = get_time_ms();
        for (int i = 0; i < 100000; i++) root = naive_insert(root, keys[i]);
        double end = get_time_ms();
        total_insert += (end - start);
        
        start = get_time_ms();
        for (int i = 0; i < 50000; i++) root = naive_delete(root, keys[i]);
        end = get_time_ms();
        total_delete += (end - start);
        
        naive_free(root);
    }
    
    printf("Random inserts (100k): %.2f ms\n", total_insert / TEST_REPEATS);
    printf("Random deletes (50k):  %.2f ms\n\n", total_delete / TEST_REPEATS);
    free(keys);
    
    int sorted_keys[10000];
    for (int i = 0; i < 10000; i++) sorted_keys[i] = i;
    
    total_insert = 0;
    total_delete = 0;
    
    for (int rep = 0; rep < TEST_REPEATS; rep++)
    {
        NaiveNode* root = NULL;
        
        double start = get_time_ms();
        for (int i = 0; i < 10000; i++) root = naive_insert(root, sorted_keys[i]);
        double end = get_time_ms();
        total_insert += (end - start);
        
        start = get_time_ms();
        for (int i = 0; i < 5000; i++) root = naive_delete(root, sorted_keys[i]);
        end = get_time_ms();
        total_delete += (end - start);
        
        naive_free(root);
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
    BNode** ptr = (BNode**)calloc(1, sizeof(BNode*));
    *ptr = b_create();
    return ptr;
}
void b_insert_wrapper(void* tree, int key) 
{ 
    BNode** ptr = (BNode**)tree;
    *ptr = b_insert(*ptr, key);
}
void b_delete_wrapper(void* tree, int key) 
{ 
    BNode** ptr = (BNode**)tree;
    *ptr = b_delete(*ptr, key);
}
void b_free_wrapper(void* tree) 
{ 
    BNode** ptr = (BNode**)tree;
    b_free(*ptr);
    free(tree);
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
    printf("\nTesting AVL with Sorted Data\n");
    
    FILE* f = fopen("results/avl_sorted_results.txt", "w");
    if (f == NULL)
    {
        printf("Cannot open results file for AVL sorted\n");
        return;
    }
    fprintf(f, "size insert_time_ms delete_time_ms\n");
    
    for (int size = START_SIZE; size <= MAX_SIZE; size += STEP_SIZE)
    {
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
               void  (*free_func)  (void*))
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
            char filepath[256];
            snprintf(filepath, sizeof(filepath), "data/keys_%d.bin", size);
            int count = size;
            int* keys = load_keys(filepath, &count);
            
            if (keys == NULL)
            {
                printf("Cannot open data file for size %d\n", size);
                fclose(f);
                return;
            }
            
            void* tree = create_func();
            if (tree == NULL)
            {
                printf("Failed to create %s\n", name);
                free(keys);
                fclose(f);
                return;
            }
            
            double start = get_time_ms();
            for (int i = 0; i < size; i++) insert_func(tree, keys[i]);
            double end = get_time_ms();
            total_insert += (end - start);
            
            start = get_time_ms();
            for (int i = 0; i < size / 2; i++) delete_func(tree, keys[i]);
            end = get_time_ms();
            total_delete += (end - start);
            
            free_func(tree);
            free(keys);
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
    test_tree("RB",       rb_create_wrapper,    rb_insert_wrapper,    rb_delete_wrapper,    rb_free_wrapper);
    test_tree("B",        b_create_wrapper,     b_insert_wrapper,     b_delete_wrapper,     b_free_wrapper);
    test_tree("SkipList", sl_create_wrapper,    sl_insert_wrapper,    sl_delete_wrapper,    sl_free_wrapper);
    
    printf("\nAll tests completed!\n");
    printf("Results saved in results/ directory\n");
    
    return 0;
}