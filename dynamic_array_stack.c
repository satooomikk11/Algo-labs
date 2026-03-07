#include "dynamic_array_stack.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

Stack* array_stack_ctr(size_t size, size_t element_size)
{
    Stack* st = (Stack*)calloc(1, sizeof(Stack));
    if (!st) return NULL;
    
    st->data = calloc(size, element_size);
    if (!st->data)
    {
        free(st);
        return NULL;
    }
    
    st->size = 0;
    st->capacity = size;
    st->element_size = element_size;
    st->ctr  = array_stack_ctr;
    st->push = array_stack_push;
    st->top  = array_stack_top;
    st->pop  = array_stack_pop;
    st->dtr  = array_stack_dtr;
    
    return st;
}

int array_stack_push(Stack* st, void* buffer)
{
    if (!st || !buffer) return 0;

    assert(st->element_size > 0);
    assert(st->data != NULL);
    
    // если массив заполнен - размер*2
    if (st->size >= st->capacity)
    {
        size_t new_capacity = st->capacity * 2;
        if (new_capacity < st->capacity) return 0; // переполнение

        void* new_data = realloc(st->data, new_capacity * st->element_size);
        if (!new_data) return 0;
        
        st->data = new_data;
        st->capacity = new_capacity;
    }
    
    // копируем новый элемент в конец массива
    memcpy((char*)st->data + st->size * st->element_size, buffer, st->element_size);
    st->size++;
    return 1;
}

int array_stack_top(Stack* st, void* buffer)
{
    if (!st || !buffer || st->size == 0) return 0;
    
    memcpy(buffer, (char*)st->data + (st->size - 1) * st->element_size, st->element_size);
    return 1;
}

int array_stack_pop(Stack* st)
{
    if (!st || st->size == 0) return 0;
    
    st->size--;
    return 1;
}

Stack* array_stack_dtr(Stack* st)
{
    if (st)
    {
        if (st->data) free(st->data);
        free(st);
    }
    return NULL;
}