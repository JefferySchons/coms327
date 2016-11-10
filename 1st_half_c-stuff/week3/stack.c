#include <stdlib.h>
#include <stdio.h>

#include "stack.h"

int stack_init(stack_t *s)
{
  s->size = 0;
  s->top = NULL;

  return 0;
}

int stack_delete(stack_t *s)
{
  stack_node_t *tmp;

  s->size = 0;

  while (s->top) {
    tmp = s->top;
    s->top = s->top->next;
    free(tmp);
  }

  return 0;
}

int stack_push(stack_t *s, int v)
{
  stack_node_t *n;

  if (!(n = malloc(sizeof (*n)))) {
    return 1;
  }

  n->value = v;
  s->size++;
  n->next = s->top;
  s->top = n;

  return 0;
}

int stack_pop(stack_t *s, int *v)
{
  stack_node_t *tmp;

  *v = s->top->value;
  s->size--;
  tmp = s->top;
  s->top = s->top->next;
  free(tmp);

  return 0;
}

int stack_is_empty(stack_t *s)
{
  return !s->top;
}

int stack_size(stack_t *s)
{
  return s->size;
}

