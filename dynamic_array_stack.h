/**
 * @file dynamic_array_stack.h
 * @brief Реализация стека на динамическом массиве.
 * 
 * Содержит функции создания, удаления и работы со стеком,
 * где данные хранятся в непрерывной области памяти.
 */

#ifndef DYNAMIC_ARRAY_STACK_H
#define DYNAMIC_ARRAY_STACK_H

#include "stack_struct.h"

/**
 * @brief Создаёт новый стек на динамическом массиве.
 * 
 * @param size          Начальная вместимость стека (количество элементов).
 * @param element_size  Размер одного элемента в байтах.
 * @return Stack*       Указатель на созданный стек или NULL в случае ошибки.
 */
Stack* array_stack_ctr (size_t size, size_t element_size);

/**
 * @brief Добавляет элемент на вершину стека.
 * 
 * @param st      Указатель на стек.
 * @param buffer  Указатель на данные, которые нужно скопировать в стек.
 * @return int    STACK_OK при успехе, STACK_ERROR при ошибке.
 */
int array_stack_push(Stack* st, void* buffer);

/**
 * @brief Копирует верхний элемент стека в буфер, не удаляя его.
 * 
 * @param st      Указатель на стек.
 * @param buffer  Указатель на буфер для приёма данных.
 * @return int    STACK_OK, если стек не пуст, иначе STACK_ERROR.
 */
int array_stack_top (Stack* st, void* buffer);

/**
 * @brief Удаляет верхний элемент стека.
 * 
 * @param st    Указатель на стек.
 * @return int  STACK_OK, если стек не пуст, иначе STACK_ERROR.
 */
int array_stack_pop (Stack* st);

/**
 * @brief Уничтожает стек и освобождает выделенную память.
 * 
 * @param st       Указатель на стек.
 * @return Stack*  Всегда возвращает NULL.
 */
Stack* array_stack_dtr (Stack* st);

#endif // DYNAMIC_ARRAY_STACK_H