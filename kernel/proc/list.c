#include "list.h"

#include <kernel/include/proc.h>
#include <stddef.h>

void list_append(struct list *list, struct node *node) {
  node->next = NULL;

  if (list_is_empty(list)) {
    list->head = node;
    list->tail = node;
  } else {
    list->tail->next = node;
    list->tail = node;
  }
}

struct node *list_remove_head(struct list *list) {
  struct node *item;

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

struct node *list_remove(struct list *list, int wait) {
  struct node *current = list->head;
  struct node *prev = (struct node *)list;
  struct node *item = NULL;

  while (current != NULL) {
    if (((struct proc *)current)->wait == wait) {
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

  return item;
}

bool list_is_empty(struct list *list) { return (list->head == NULL); }