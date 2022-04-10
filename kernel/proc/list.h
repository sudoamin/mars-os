#ifndef _PROC_LIST_H
#define _PROC_LIST_H

#include "stdbool.h"

struct list {
  struct proc *head;
  struct proc *tail;
};

void list_append(struct list *list, struct proc *proc);
struct proc *list_remove_head(struct list *list);
struct proc *list_remove(struct list *list, int wait);
bool list_is_empty(struct list *list);

#endif