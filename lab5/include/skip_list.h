#ifndef SKIP_LIST_H
#define SKIP_LIST_H

#define MAX_LEVEL 20

typedef struct SkipListNode
{
    int key;
    struct SkipListNode** forward;
} SkipListNode;

typedef struct
{
    SkipListNode* header;
    int    level;
    double probability;
} SkipList;

SkipList* sl_create  (void);
void      sl_insert  (SkipList* list, int key);
void      sl_delete  (SkipList* list, int key);
SkipListNode* sl_find(SkipList* list, int key);
void      sl_free    (SkipList* list);
int       sl_height  (SkipList* list);

#endif // SKIP_LIST_H
