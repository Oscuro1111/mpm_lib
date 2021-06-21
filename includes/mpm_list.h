#ifndef __MPM_LIST__
#define __MPM_LIST__

#include <stdint.h>

typedef struct mpm_node {
  void *element;
  struct mpm_node *next;
  struct mpm_node *prev;
} Node;

typedef struct mpm_list {
  Node *head;
  Node *tail;

  uint32_t length;
} List;

#define IS_NULL(exp)                                                           \
  if (exp == NULL)                                                             \
  return -1


//Allocate new list on heap. 
//call "free_list" for freeing list and its nodes with elements 
List *allocate_list();

// Operations on state of list
int mpm_list_add(List *, void *ele);

int mpm_list_delete(List *, void *ele);

Node *mpm_list_next(List *, void **);

void free_list(List *, void (*func)(void *));

void free_node(Node *node);

uint8_t is_null(void *exp);

//find consition functions
uint8_t find_string(Node *,void *ele);

/******************************************/
// Find element node by given  conditional function
Node *mpm_list_find(List *list, void *ele,
                    uint8_t (*find_cond)(Node *, void *));

#endif
