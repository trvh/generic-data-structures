#ifndef STACK_SINGLE_LIST_HEADER
#define STACK_SINGLE_LIST_HEADER

#include <stdlib.h>
#include "common_consts.h"

struct stack_gds {
	struct list_gds *list;
	size_t			 size;
};

void stack_create(struct stack_gds *stack, size_t size);
void stack_delete(struct stack_gds *stack);

void   stack_clear(struct stack_gds *stack);
int    stack_is_empty(struct stack_gds *stack);
size_t stack_count(struct stack_gds *stack);

void stack_push(struct stack_gds *stack, void *value);
void stack_pop(struct stack_gds *stack, void *value);
void stack_pop2(struct stack_gds* stack);
void stack_peek(struct stack_gds* stack, void *value);
#endif
