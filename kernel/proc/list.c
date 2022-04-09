#include <kernel/include/proc.h>

#include "stddef.h"

void append_list_tail(struct HeadList *list, struct List *item) {
  item->next = NULL;

  if (is_list_empty(list)) {
    list->next = item;
    list->tail = item;
  } else {
    list->tail->next = item;
    list->tail = item;
  }
}

struct List *remove_list(struct HeadList *list, int wait) {
  struct List *current = list->next;
  struct List *prev = (struct List *)list;
  struct List *item = NULL;

  while (current != NULL) {
    if (((struct proc *)current)->wait == wait) {
      prev->next = current->next;
      item = current;

      if (list->next == NULL) {
        list->tail = NULL;
      } else if (current->next == NULL) {
        list->tail = prev;
      }

      break;
    }

    prev = current;
    current = current->next;
  }

  return item;
}

struct List *remove_list_head(struct HeadList *list) {
  struct List *item;

  if (is_list_empty(list)) {
    return NULL;
  }

  item = list->next;
  list->next = item->next;

  if (list->next == NULL) {
    list->tail = NULL;
  }

  return item;
}

bool is_list_empty(struct HeadList *list) { return (list->next == NULL); }