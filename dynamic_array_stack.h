#ifndef DYNAMIC_ARRAY_STACK_H
#define DYNAMIC_ARRAY_STACK_H

#include "stack_struct.h"

Stack* array_stack_ctr (size_t size, size_t element_size);
int    array_stack_push(Stack* st, void* buffer);
int    array_stack_top (Stack* st, void* buffer);
int    array_stack_pop (Stack* st);
Stack* array_stack_dtr (Stack* st);

#endif // DYNAMIC_ARRAY_STACK_H