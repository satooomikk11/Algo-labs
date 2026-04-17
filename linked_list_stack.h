/**
 * @file linked_list_stack.h
 * @brief Реализация стека на односвязном списке.
 * 
 * Содержит определение узла списка и функций для работы со стеком,
 * где каждый элемент хранится в отдельном узле, выделенном в куче.
 */

#ifndef LINKED_LIST_STACK_H
#define LINKED_LIST_STACK_H

#include "stack_struct.h"

/**
 * @brief Узел односвязного списка для хранения данных стека.
 */
typedef struct Node
{
    void* data;           ///< Указатель на данные элемента.
    struct Node* next;    ///< Указатель на следующий узел.
} Node;

/**
 * @brief Структура стека на списке.
 * 
 * Расширяет базовый стек `Stack`, добавляя указатель на голову списка.
 */
typedef struct
{
    Stack base;   ///< Базовая часть стека (общие поля и функции).
    Node* head;   ///< Указатель на вершину стека (первый элемент списка).
} ListStack;

/**
 * @brief Создаёт новый стек на основе односвязного списка.
 * 
 * @param size          Параметр не используется для списка, но сохраняется для единообразия.
 * @param element_size  Размер одного элемента в байтах.
 * @return Stack*       Указатель на созданный стек (как `ListStack`, приведённый к `Stack*`).
 */
Stack* list_stack_ctr (size_t size, size_t element_size);

/**
 * @brief Добавляет элемент на вершину стека (в начало списка).
 * 
 * @param st      Указатель на стек.
 * @param buffer  Указатель на данные для добавления.
 * @return int    STACK_OK при успехе, STACK_ERROR при ошибке выделения памяти.
 */
int list_stack_push(Stack* st, void* buffer);

/**
 * @brief Копирует верхний элемент стека в буфер.
 * 
 * @param st      Указатель на стек.
 * @param buffer  Буфер для приёма данных.
 * @return int    STACK_OK, если стек не пуст, иначе STACK_ERROR.
 */
int list_stack_top (Stack* st, void* buffer);

/**
 * @brief Удаляет верхний элемент стека (освобождая память узла).
 * 
 * @param st    Указатель на стек.
 * @return int  STACK_OK, если стек не пуст, иначе STACK_ERROR.
 */
int list_stack_pop (Stack* st);

/**
 * @brief Уничтожает стек и освобождает память всех узлов.
 * 
 * @param st       Указатель на стек.
 * @return Stack*  Всегда возвращает NULL.
 */
Stack* list_stack_dtr (Stack* st);

#endif // LINKED_LIST_STACK_H