#include "proc.h"
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