#ifndef LINKED_LIST_STACK_H
#define LINKED_LIST_STACK_H

#include "stack_struct.h"

// узел односвязного списка
typedef struct Node
{
    void* data;
    struct Node* next;
} Node;

typedef struct
{
    Stack base; // базовый стек
    Node* head;
} ListStack;

Stack* list_stack_ctr (size_t size, size_t element_size);
int    list_stack_push(Stack* st, void* buffer);
int    list_stack_top (Stack* st, void* buffer);
int    list_stack_pop (Stack* st);
Stack* list_stack_dtr (Stack* st);

#endif // LINKED_LIST_STACK_H