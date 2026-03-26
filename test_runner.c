#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include "dynamic_array_stack.h"
#include "linked_list_stack.h"

const int ITERATIONS       = 3;        // количество повторений тестов
const int MILLION          = 1000000;
const int HUNDRED_THOUSAND = 100000;
const int TEN_THOUSAND     = 10000;
const int QUARTER_MILLION  = 250000;

#define TESTS       \
    ADD(test_test1) \
    ADD(test_test2) \
    ADD(test_test3) \
    ADD(test_test4)

#define CTEST_IMPLEMENTATION
#include "ctest.h"

double get_time()
{
    clock_t ticks = clock();
    return (double)ticks / CLOCKS_PER_SEC;
}

// начальный размер 1000000, удаляем половину, добавляем четверть от исходного (9 итераций)
void run_test1(Stack* (*create_stack)(size_t, size_t), const char* name)
{
    double total_time = 0;
    
    assert(create_stack != NULL);

    for (int iter = 0; iter < ITERATIONS; iter++)
    {
        Stack* st = create_stack(MILLION, sizeof(int));
        if (!st)
        {
            printf("Ошибка создания стека\n");
            return;
        }
        
        int value = 666;
        for (int i = 0; i < MILLION; i++)
        {
            st->push(st, &value);
        }
        
        double start = get_time();
        
        for (int iteration = 0; iteration < 9; iteration++)
        {
            int to_remove = st->size / 2;
            for (int i = 0; i < to_remove; i++)
            {
                st->pop(st);
            }
            
            for (int i = 0; i < QUARTER_MILLION; i++)
            {
                st->push(st, &value);
            }
        }
        
        double end = get_time();
        total_time += (end - start);
        
        st->dtr(st);
    }
    
    printf("%s: %.6f секунд\n", name, total_time / ITERATIONS);
}

// 100 раз удалить, добавить 10000, потом 9 итераций, потом снова 100 раз
void run_test2(Stack* (*create_stack)(size_t, size_t), const char* name)
{
    double total_time = 0;

    assert(create_stack != NULL);
    
    for (int iter = 0; iter < ITERATIONS; iter++)
    {
        Stack* st = create_stack(MILLION, sizeof(int));
        if (!st) return;
        
        int value = 666;
        for (int i = 0; i < MILLION; i++)
        {
            st->push(st, &value);
        }
        
        double start = get_time();
        
        // 100 циклов удаления 10000 и добавления 10000
        for (int cycle = 0; cycle < 100; cycle++)
        {
            for (int i = 0; i < TEN_THOUSAND; i++) st->pop(st);
            for (int i = 0; i < TEN_THOUSAND; i++) st->push(st, &value);
        }
        
        // 9 итераций удаления половины и добавления четверти
        for (int iteration = 0; iteration < 9; iteration++)
        {
            int to_remove = st->size / 2;
            for (int i = 0; i < to_remove; i++) st->pop(st);
        
            for (int i = 0; i < QUARTER_MILLION; i++) st->push(st, &value);
        }
        
        // 100 циклов удаления 10000 и добавления 10000
        for (int cycle = 0; cycle < 100; cycle++)
        {
            for (int i = 0; i < TEN_THOUSAND; i++) st->pop(st);
            for (int i = 0; i < TEN_THOUSAND; i++) st->push(st, &value);
        }
        
        double end = get_time();
        total_time += (end - start);
        
        st->dtr(st);
    }
    
    printf("%s: %.6f секунд\n", name, total_time / ITERATIONS);
}

// довести до 1000000, затем 1000000 случайных операций (1 - push, 2 - pop)
void run_test3(Stack* (*create_stack)(size_t, size_t), const char* name)
{
    double total_time = 0;

    assert(create_stack != NULL);
    
    for (int iter = 0; iter < ITERATIONS; iter++)
    {
        Stack* st = create_stack(MILLION, sizeof(int));
        if (!st) return;
        
        int value = 666;
        for (int i = 0; i < MILLION; i++)
        {
            st->push(st, &value);
        }
        
        double start = get_time();
        
        // 1000000 случайных операций
        for (int i = 0; i < MILLION; i++)
        {
            int op = rand() % 2 + 1; // 1 или 2
            if (op == 1)
            {
                st->push(st, &value);
            }
            else
            {
                if (st->size > 0)
                {
                    st->pop(st);
                }
            }
        }
        
        double end = get_time();
        total_time += (end - start);
        
        st->dtr(st);
    }
    
    printf("%s: %.6f секунд\n", name, total_time / ITERATIONS);
}

// измерение времени вставки от 1000 до 1000000 элементов с шагом 1000
void run_test4()
{
    FILE* data_file = fopen("test4_data.txt", "w");
    if (!data_file) return;
    
    fprintf(data_file, "n array list\n");
    
    for (int n = 1000; n <= MILLION; n += 1000)
    {        
        // тест стека на массиве
        double array_time = 0;
        for (int iter = 0; iter < ITERATIONS; iter++)
        {
            Stack* arr_st = array_stack_ctr(1000, sizeof(int));
            if (!arr_st) continue;
            
            int value = 666;
            double start = get_time();
            for (int i = 0; i < n; i++)
            {
                arr_st->push(arr_st, &value);
            }
            double end = get_time();
            array_time += (end - start);
            
            arr_st->dtr(arr_st);
        }
        
        // тест стека на списке
        double list_time = 0;
        for (int iter = 0; iter < ITERATIONS; iter++)
        {
            Stack* list_st = list_stack_ctr(0, sizeof(int));
            if (!list_st) continue;
            
            int value = 666;
            double start = get_time();
            for (int i = 0; i < n; i++)
            {
                list_st->push(list_st, &value);
            }
            double end = get_time();
            list_time += (end - start);
            
            list_st->dtr(list_st);
        }
        
        fprintf(data_file, "%d %.6f %.6f\n", n, array_time/ITERATIONS, list_time/ITERATIONS);
    }
    
    fclose(data_file);
}

TEST(test_test1,
    printf("\nТест 1:\n");
    run_test1(array_stack_ctr, "Стек на массиве");
    run_test1(list_stack_ctr,  "Стек на списке ");
    ASSERT(1);
)

TEST(test_test2,
    printf("\nТест 2:\n");
    run_test2(array_stack_ctr, "Стек на массиве");
    run_test2(list_stack_ctr,  "Стек на списке ");
    ASSERT(1);
)

TEST(test_test3,
    printf("\nТест 3:\n");
    run_test3(array_stack_ctr, "Стек на массиве");
    run_test3(list_stack_ctr,  "Стек на списке ");
    ASSERT(1);
)

TEST(test_test4,
    printf("\nТест 4:\n");
    run_test4();
    ASSERT(1);
)

RUN_TESTS();