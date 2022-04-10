#include "list.h"

#include <kernel/include/proc.h>
#include <stddef.h>

void list_append(struct list *list, struct proc *proc) {
  proc->next = NULL;

  if (list_is_empty(list)) {
    list->head = proc;
    list->tail = proc;
  } else {
    list->tail->next = proc;
    list->tail = proc;
  }
}

struct proc *list_remove_head(struct list *list) {
  struct proc *item;

  if (list_is_empty(list)) {
    return NULL;
  }

  item = list->head;
  list->head = item->next;

  if (list->head == NULL) {
    list->tail = NULL;
  }

  return item;
}

struct proc *list_remove(struct list *list, int wait) {
  struct proc *current = list->head;
  struct proc *prev = list;
  struct proc *item = NULL;

  while (current != NULL) {
    if (current->wait == wait) {
      prev->next = current->next;
      item = current;

      if (list->head == NULL) {
        list->tail = NULL;
      } else if (current->next == NULL) {
        list->tail = prev;
      }

      break;
    }

    prev = current;
    current = current->next;
  }

    // printk("%u *", ((struct proc *)item)->pid);

  return item;
}

bool list_is_empty(struct list *list) { return (list->head == NULL); }