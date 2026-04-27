#include "dynamic_array_stack.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// коэффициенты управления размером массива
const int ARRAY_GROWTH_FACTOR = 2;
const int ARRAY_SHRINK_FACTOR = 4;
const int ARRAY_MIN_CAPACITY  = 1000;

static void array_stack_resize(Stack* st, size_t new_capacity)
{
    if (!st || new_capacity == 0) return;
    
    void* new_data = realloc(st->data, new_capacity * st->element_size);
    if (new_data)
    {
        st->data = new_data;
        st->capacity = new_capacity;
    }
}

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
    if (!st || !buffer) return STACK_ERROR;

    assert(st->element_size > 0);
    assert(st->data != NULL);
    
    // если массив заполнен - размер*2
    if (st->size >= st->capacity)
    {
        size_t new_capacity = st->capacity * ARRAY_GROWTH_FACTOR;
        if (new_capacity < st->capacity) return STACK_ERROR; // переполнение

        array_stack_resize(st, new_capacity);
        if (st->size >= st->capacity) return STACK_ERROR; // проверка, что resize удался
    }
    
    // копируем новый элемент в конец массива
    memcpy((char*)st->data + st->size * st->element_size, buffer, st->element_size);
    st->size++;
    return STACK_OK;
}

int array_stack_top(Stack* st, void* buffer)
{
    if (!st || !buffer || st->size == 0) return STACK_ERROR;
    
    memcpy(buffer, (char*)st->data + (st->size - 1) * st->element_size, st->element_size);
    return STACK_OK;
}

int array_stack_pop(Stack* st)
{
    if (!st || st->size == 0) return STACK_ERROR;
    
    st->size--;

    // сужение массива, если размер стал меньше 1/4 от capacity
    // и capacity больше минимального размера
    if (st->size > 0 && st->size <= st->capacity / ARRAY_SHRINK_FACTOR && st->capacity > ARRAY_MIN_CAPACITY)
    {
        size_t new_capacity = st->capacity / ARRAY_GROWTH_FACTOR;
        array_stack_resize(st, new_capacity);
    }

    return STACK_OK;
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