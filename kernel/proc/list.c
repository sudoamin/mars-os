#include "list.h"

#include <kernel/include/proc.h>
#include <stddef.h>
#include <stdint.h>
// #include <stdio.h>

struct list* list_append(struct list* list, struct proc* proc) {
  if (proc == NULL) {
    return;
  }

  proc->next = NULL;

  if (list->size == 0) {
    list->head = proc;
    list->tail = proc;
  } else {
    list->tail->next = proc;
    list->tail = proc;
  }

  list->size = list->size + 1;

  return list;
}

struct proc* list_find(struct list* list, int wait) {
  if (list->head == NULL) {
    return NULL;
  }

  struct proc* tmp = list->head;
  while (tmp != NULL) {
    if (tmp->wait == wait) {
      tmp->next = NULL;  // NEXT IS NULL
      return tmp;
    }

    tmp = tmp->next;
  }

  return NULL;
}

struct list* list_remove_head(struct list* list) {
  if (list->head == NULL) {
    return NULL;
  }

  list->size = list->size - 1;
  list->head = list->head->next;

  return list;
}

// list_remove removes proc from list.
// it uses the pid for filtering
struct list* list_remove(struct list* list, struct proc* proc) {
  if (proc == NULL) {
    return;
  }

  if (list->head == NULL) {
    return NULL;
  }

  struct proc* prev = NULL;
  struct proc* current = list->head;

  while (current != NULL) {
    if (current->pid == proc->pid) {
      if (prev == NULL) {
        list->head = current->next;
      } else {
        prev->next = current->next;
      }

      list->size = list->size - 1;

      return list;
    }

    prev = current;
    current = current->next;
  }

  return list;
}

bool list_is_empty(struct list* list) { return list->head == NULL; }

// // TODO, add a tmp for iteration
// void list_print(struct list* list) {
//   printf("size: %d \n", list->size);

//   while (list->head != NULL) {
//     printf("pid: %d \n", list->head->pid);
//     list->head = list->head->next;
//   }
// }

// static struct proc ps_list[10];
// static struct list test_list;

// int main() {
//   struct proc* p1 = &ps_list[0];
//   p1->pid = 1;

//   struct proc* p2 = &ps_list[1];
//   p2->pid = 2;

//   struct proc* p3 = &ps_list[2];
//   p3->pid = 3;

//   test_list = *list_append(&test_list, p1);
//   test_list = *list_append(&test_list, p2);
//   test_list = *list_append(&test_list, p3);

//   struct proc* pf = list_find(&test_list, 2);
//   if (pf != NULL) {
//     printf("found: %d\n", pf->pid);
//   } else {
//     printf("* not found\n");
//   }

//   test_list = *list_remove_head(&test_list);

//   test_list = *list_remove(&test_list, pf);

//   list_print(&test_list);

//   return 0;
// }