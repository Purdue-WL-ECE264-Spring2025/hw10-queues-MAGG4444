#include "linked_list.h"

#include <stdlib.h>

struct list_node *new_node(size_t value) {
  struct list_node *node = malloc(sizeof(struct list_node));
  if (node == NULL) {
    return NULL;
  }
  node->value = value;
  node->next = NULL;
  return node;
}

void insert_at_head(struct linked_list *list, size_t value) {
  struct list_node *new_head_node = new_node(value);
  if (new_head_node == NULL) {
    return;
  }
  new_head_node->next = list->head;
  list->head = new_head_node;
}

void insert_at_tail(struct linked_list *list, size_t value) {
  if (list->head == NULL) {
    insert_at_head(list, value);
    return;
  }

  struct list_node *current = list->head;
  while (current->next != NULL) {
    current = current->next;
  }

  struct list_node *new_tail_node = new_node(value);
  if (new_tail_node == NULL) {
    return;
  }
  current->next = new_tail_node;
}

size_t remove_from_head(struct linked_list *list) {
  if (list->head == NULL) {
    return -1;
  }

  struct list_node *node_to_remove = list->head;
  size_t val = node_to_remove->value;
  list->head = node_to_remove->next;
  free(node_to_remove);

  return val;
}

size_t remove_from_tail(struct linked_list *list) {
  if (list->head == NULL) {
    return -1;
  }

  if (list->head->next == NULL) {
    return remove_from_head(list);
  }

  struct list_node *prev = list->head;
  struct list_node *curr = prev->next;

  while (curr->next != NULL) {
    prev = curr;
    curr = curr->next;
  }

  size_t val = curr->value;
  prev->next = NULL;
  free(curr);

  return val;
}

void free_list(struct linked_list list) {
  struct list_node *curr = list.head;
  struct list_node *next_node = NULL;

  while (curr != NULL) {
    next_node = curr->next;
    free(curr);
    curr = next_node;
  }
}

void dump_list(FILE *fp, struct linked_list list) {
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}