#ifndef STACK_STRUCT_H
#define STACK_STRUCT_H

#include <stddef.h>

// cтатусы выполнения операций
typedef enum 
{
    STACK_OK    = 1,
    STACK_ERROR = 0
} StackStatus;

typedef struct Stack 
{
    void* data;
    size_t size;
    size_t capacity;
    size_t element_size;
    struct Stack* (*ctr)(size_t, size_t);
    int (*push)(struct Stack*, void*);
    int (*top )(struct Stack*, void*);
    int (*pop )(struct Stack*);
    struct Stack* (*dtr)(struct Stack*);
} Stack;

#endif // STACK_STRUCT_H