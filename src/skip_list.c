#include "../include/skip_list.h"
#include <stdlib.h>
#include <time.h>

static int random_level(void)
{
    static int initialized = 0;
    if (!initialized)
    {
        srand(time(NULL));
        initialized = 1;
    }
    int level = 0;
    while (level < MAX_LEVEL - 1 && (rand() % 2)) level++;
    return level;
}

SkipList* sl_create(void)
{
    SkipList* list = (SkipList*)calloc(1, sizeof(SkipList));
    if (list == NULL) return NULL;
    
    list->header = (SkipListNode*)calloc(1, sizeof(SkipListNode));
    if (list->header == NULL)
    {
        free(list);
        return NULL;
    }
    
    list->header->forward = (SkipListNode**)calloc(sizeof(SkipListNode*), MAX_LEVEL);
    for (int i = 0; i < MAX_LEVEL; i++)
    {
        list->header->forward[i] = NULL;
    }
    
    list->level = 0;
    list->probability = 0.5;
    
    return list;
}

void sl_insert(SkipList* list, int key)
{
    SkipListNode* update[MAX_LEVEL];
    SkipListNode* current = list->header;
    
    for (int i = list->level; i >= 0; i--)
    {
        while (current->forward[i] && current->forward[i]->key < key)
        {
            current = current->forward[i];
        }
        update[i] = current;
    }
    
    current = current->forward[0];
    
    if (current && current->key == key) return;
    
    int new_level = random_level();
    if (new_level > list->level)
    {
        for (int i = list->level + 1; i <= new_level; i++) 
        {
            update[i] = list->header;
        }
        list->level = new_level;
    }
    
    SkipListNode* new_node = (SkipListNode*)calloc(1, sizeof(SkipListNode));
    new_node->key = key;
    new_node->forward = (SkipListNode**)calloc(sizeof(SkipListNode*), (new_level + 1));
    
    for (int i = 0; i <= new_level; i++)
    {
        new_node->forward[i]  = update[i]->forward[i];
        update[i]->forward[i] = new_node;
    }
}

void sl_delete(SkipList* list, int key)
{
    SkipListNode* update[MAX_LEVEL];
    SkipListNode* current = list->header;
    
    for (int i = list->level; i >= 0; i--)
    {
        while (current->forward[i] && current->forward[i]->key < key)
        {
            current = current->forward[i];
        }
        update[i] = current;
    }
    
    current = current->forward[0];
    
    if (current == NULL || current->key != key) return;
    
    for (int i = 0; i <= list->level; i++)
    {
        if (update[i]->forward[i] != current) break;
        update[i]->forward[i] = current->forward[i];
    }
    
    while (list->level > 0 && list->header->forward[list->level] == NULL)
    {
        list->level--;
    }
    
    free(current->forward);
    free(current);
}

SkipListNode* sl_find(SkipList* list, int key)
{
    SkipListNode* current = list->header;
    
    for (int i = list->level; i >= 0; i--)
    {
        while (current->forward[i] && current->forward[i]->key < key) 
        {
            current = current->forward[i];
        }
    }
    
    current = current->forward[0];
    
    if (current && current->key == key) return current;
    return NULL;
}

void sl_free(SkipList* list)
{
    SkipListNode* current = list->header->forward[0];
    SkipListNode* next;
    
    while (current)
    {
        next = current->forward[0];
        free(current->forward);
        free(current);
        current = next;
    }
    
    free(list->header->forward);
    free(list->header);
    free(list);
}

int sl_height(SkipList* list) 
{
    return list->level;
}
