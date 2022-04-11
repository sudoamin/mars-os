#ifndef _PROC_LIST_H
#define _PROC_LIST_H

// #include <kernel/include/mem.h>
#include <stdbool.h>

struct list {
  struct proc *head;
  struct proc *tail;
  int size;
};


// void list_append(struct list *list, struct node *node);
// struct node *list_remove_head(struct list *list);
// struct node *list_remove(struct list *list, int wait);
// bool list_is_empty(struct list *list);

#endif