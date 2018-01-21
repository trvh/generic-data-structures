#include <string.h>
#include <assert.h>

#include "gds_stack.h"
#include "gds_common.h"

/*number of elements in one node*/
#define COUNT_ELEMENTS 256

/* 2 ^ SHIFT = COUNT_ELEMENTS*/
#define SHIFT	       8

#define NODE_SIZE(size) (sizeof(struct gds_snode) + ((size) << SHIFT))

struct gds_snode {
	struct gds_snode *prev;
	/*hidden buffer for data will starte here*/
};

void
stack_create(struct gds_stack *stack, size_t size)
{	
	struct gds_snode *node;
	
	assert(stack != NULL);
	assert(size > 0);
	
	node = (struct gds_snode *) malloc(NODE_SIZE(size));
	assert(node != NULL);
	
	node->prev = NULL;
	
	stack->end   = TO_NEXT(node, NODE_SIZE(size));
	stack->start = stack->top = TO_NEXT(node, sizeof(struct gds_snode));
	stack->size  = size;
	stack->count = 0;
}

void
stack_delete(struct gds_stack *stack)
{
	struct gds_snode *node, *prev;

	assert(stack != NULL);
	
	node = TO_PREV(stack->start, sizeof(struct gds_snode));
	while (node != NULL) {
		prev = node->prev;
		free(node);
		node = prev;
	}
}

void
stack_clear(struct gds_stack *stack)
{
	struct gds_snode *node, *prev;

	assert(stack != NULL);
	
	/*delete all nodes before first*/
	node = TO_PREV(stack->start, sizeof(struct gds_snode));
	while (node->prev != NULL) {
		prev = node->prev;
		free(node);
		node = prev;
	}
	stack->end   = TO_NEXT(node, NODE_SIZE(stack->size));
	stack->start = stack->top = TO_NEXT(node, sizeof(struct gds_snode));
	stack->count = 0;
}

void
stack_push(struct gds_stack *stack, void *src)
{
	struct gds_snode *new_node;
	void  *top;
	size_t size;

	assert(stack != NULL);
	assert(src != NULL);
	
	size = stack->size;
	top  = stack->top;
	if (top == stack->end) {
		/*current node is full*/
		new_node = (struct gds_snode *) malloc(NODE_SIZE(size));
		assert(new_node != NULL);
		new_node->prev = TO_PREV(stack->start, sizeof(struct gds_snode));
		
		stack->end   = TO_NEXT(new_node, NODE_SIZE(size));
		stack->start = top = TO_NEXT(new_node, sizeof(struct gds_snode));
	}
	memcpy(top, src, size);
	stack->top = TO_NEXT(top, size);
	stack->count++;
}

void
stack_pop(struct gds_stack *stack, void *dst)
{
	struct gds_snode *node, *prev;
	void  *top;
	size_t size;
	
	assert(stack != NULL);
	assert(stack->count != 0);
	assert(dst != NULL);
	
	size  = stack->size;
	top   = stack->top;
	if (top == stack->start) {
		/*current node is empty*/
		node = TO_PREV(stack->start, sizeof(struct gds_snode));
		prev = node->prev;
		free(node);
		
		stack->end   = top = TO_NEXT(prev, NODE_SIZE(size));
		stack->start = TO_NEXT(prev, sizeof(struct gds_snode));
	}
	top = TO_PREV(top, size);
	memcpy(dst, top, size);
	stack->top = top;	
	stack->count--;
}

void
stack_peek(struct gds_stack *stack, void *dst)
{
	struct gds_snode *node, *prev;
	void  *top;
	size_t size;
	
	assert(stack != NULL);
	assert(stack->count != 0);
	assert(dst != NULL);
	
	size = stack->size;
	top  = stack->top;
	if (top == stack->start) {
		node = TO_PREV(stack->start, sizeof(struct gds_snode));
		prev = node->prev;
		top  = TO_NEXT(prev, NODE_SIZE(size));
	}
	top = TO_PREV(top, size);	
	memcpy(dst, top, size);
}
