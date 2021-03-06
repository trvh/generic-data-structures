#ifndef GDS_STACK_HEADER
#define GDS_STACK_HEADER

#include <stdlib.h>

struct gds_stack {
	void  *start; /*pointer to start of buffer*/
	void  *end;   /*pointer to end of buffer*/
	void  *top;   /*pointer to current position in buffer*/
	size_t count; /*number of elements*/
	size_t size;  /*size of one element*/
};

#ifdef __cplusplus
extern "C" {
#endif

void stack_create(struct gds_stack *stack, size_t size);
void stack_delete(struct gds_stack *stack);
void stack_clear(struct gds_stack *stack);

void stack_push(struct gds_stack *stack, void *src);
void stack_pop(struct gds_stack *stack, void *dst);
void stack_peek(struct gds_stack *stack, void *dst);

#ifdef __cplusplus
}
#endif

#endif
