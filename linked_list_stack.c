#include "linked_list_stack.h"
#include <stdlib.h>
#include <string.h>

Stack* list_stack_ctr(size_t size, size_t element_size)
{
    if (element_size == 0) return NULL;

    ListStack* lst = (ListStack*)calloc(1, sizeof(ListStack));
    if (!lst) return NULL;
    
    lst->base.data = NULL;
    lst->base.size = 0;
    lst->base.capacity = 0;
    lst->base.element_size = element_size;
    lst->base.ctr = list_stack_ctr;
    lst->base.push = list_stack_push;
    lst->base.top = list_stack_top;
    lst->base.pop = list_stack_pop;
    lst->base.dtr = list_stack_dtr;
    
    lst->head = NULL;
    
    return (Stack*)lst;
}

int list_stack_push(Stack* st, void* buffer)
{
    if (!st || !buffer) return STACK_ERROR;
    
    ListStack* lst = (ListStack*)st;
    Node* new_node = (Node*)calloc(1, sizeof(Node));
    if (!new_node) return STACK_ERROR;
    
    new_node->data = calloc(1, st->element_size);
    if (!new_node->data)
    {
        free(new_node);
        return STACK_ERROR;
    }
    
    memcpy(new_node->data, buffer, st->element_size);
    new_node->next = lst->head;
    lst->head = new_node;
    
    st->size++;
    return STACK_OK;
}

int list_stack_top(Stack* st, void* buffer)
{
    if (!st || !buffer || st->size == 0) return STACK_ERROR;
    
    ListStack* lst = (ListStack*)st;
    memcpy(buffer, lst->head->data, st->element_size);
    return STACK_OK;
}

int list_stack_pop(Stack* st)
{
    if (!st || st->size == 0) return STACK_ERROR;
    
    ListStack* lst = (ListStack*)st;
    Node* temp = lst->head;
    lst->head = lst->head->next;
    
    free(temp->data);
    free(temp);
    
    st->size--;
    return STACK_OK;
}

Stack* list_stack_dtr(Stack* st)
{
    if (st)
    {
        ListStack* lst = (ListStack*)st;
        Node* current = lst->head;
        while (current)
        {
            Node* next = current->next;
            free(current->data);
            free(current);
            current = next;
        }
        free(st);
    }
    return NULL;
}