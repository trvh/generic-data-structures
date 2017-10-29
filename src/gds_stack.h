#ifndef STACK_SINGLE_LIST_HEADER
#define STACK_SINGLE_LIST_HEADER

#include <stdlib.h>

struct gds_stack {
	struct gds_list *list;
	size_t count;
	size_t size;
};

void stack_create(struct gds_stack *stack, size_t size);
void stack_delete(struct gds_stack *stack);
void stack_clear(struct gds_stack *stack);

void stack_push(struct gds_stack *stack, void *src);
void stack_pop(struct gds_stack *stack, void *dst);
void stack_pop2(struct gds_stack* stack);
void stack_peek(struct gds_stack* stack, void *dst);
#endif
