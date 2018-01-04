#include <string.h>
#include <assert.h>

#include "gds_stack.h"
#include "gds_common.h"

/*number of elements in one node*/
#define COUNT_ELEMENTS 256

/* 2 ^ SHIFT = COUNT_ELEMENTS*/
#define SHIFT	       8

struct gds_snode {
	struct gds_snode *prev;
	void			 *end; /*pointer on end of buffer*/
	/*hidden buffer for data will starte here*/
};

static struct gds_snode *node_create(size_t size);

void
stack_create(struct gds_stack *stack, size_t size)
{	
	struct gds_snode *node;
	
	assert(stack != NULL);
	assert(size > 0);
	
	/*stack is empty, create new node*/
	node = node_create(size);
	node->prev = NULL;
	
	stack->list  = node;
	stack->top   = TO_NEXT(node, sizeof(struct gds_snode));
	stack->size  = size;
	stack->count = 0;
}

void
stack_delete(struct gds_stack *stack)
{
	struct gds_snode *node, *prev;

	assert(stack != NULL);
	
	node = stack->list;
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
	node = stack->list;
	while (node->prev != NULL) {
		prev = node->prev;
		free(node);
		node = prev;
	}
	stack->list = node;
	/*shift to start of buffer*/
	stack->top   = TO_NEXT(node, sizeof(struct gds_snode));
	stack->count = 0;
}

struct gds_snode *
node_create(size_t size)
{
	struct gds_snode *node;
	void  *buffer;
	size_t bsize;
	
	assert(size > 0);

	bsize = size << SHIFT; /*size of buffer for data*/
	size  = sizeof(struct gds_snode) + bsize; /*size with header and buffer*/
	node  = (struct gds_snode *) malloc(size);
	assert(node != NULL);
	
	/*shift to start of buffer*/
	buffer = TO_NEXT(node, sizeof(struct gds_snode));
	/*shift to end of buffer*/
	node->end = TO_NEXT(buffer, bsize);
	return node;
}

void
stack_push(struct gds_stack *stack, void *src)
{
	struct gds_snode *node, *node_new;
	void  *dst;
	size_t size;

	assert(stack != NULL);
	assert(src != NULL);
	
	size = stack->size;	
	node = stack->list;
	dst  = stack->top;
	if (dst == node->end) {		
		/*stack is full, add new node*/
		node_new = node_create(size);
		/*remember previous node*/
		node_new->prev = node;
		/*put new node on top of the stack*/
		stack->list = node_new;
		stack->top  = TO_NEXT(node_new, sizeof(struct gds_snode));
		/*change destination*/
		dst = stack->top;	
	}
	memcpy(dst, src, size);
	stack->top = TO_NEXT(dst, size);
	stack->count++;
}

void
stack_pop(struct gds_stack *stack, void *dst)
{
	struct gds_snode *node;
	void  *src, *start;
	size_t size;
	
	assert(stack != NULL);
	assert(stack->count != 0);
	assert(dst != NULL);
	
	size  = stack->size;
	node  = stack->list;
	src   = stack->top;
	/*shift to start of buffer*/
	start = TO_NEXT(node, sizeof(struct gds_snode));
	if (src == start) {
		/*current node is empty*/
		stack->list = node->prev;
		/*remove node*/
		free(node);
		src = stack->list->end;
	}
	src = TO_PREV(src, size);
	memcpy(dst, src, size);
	stack->top = src;	
	stack->count--;
}

void
stack_pop2(struct gds_stack *stack)
{
	struct gds_snode *node;
	void  *src, *start;
	size_t size;
	
	assert(stack != NULL);
	assert(stack->count != 0);
	
	size  = stack->size;
	node  = stack->list;
	src   = stack->top;
	start = TO_NEXT(node, sizeof(struct gds_snode));
	if (src == start) {
		/*current node is empty*/
		stack->list = node->prev;
		free(node);
		src = stack->list->end;
	}
	stack->top = TO_PREV(src, size);
	stack->count--;
}

void
stack_peek(struct gds_stack *stack, void *dst)
{
	struct gds_snode *node;
	void  *src, *start;
	size_t size;
	
	assert(stack != NULL);
	assert(stack->count != 0);
	assert(dst != NULL);
	
	size  = stack->size;
	node  = stack->list;
	src   = stack->top;
	start = TO_NEXT(node, sizeof(struct gds_snode));
	if (src == start)
		src = node->prev->end;
	src = TO_PREV(src, size);	
	memcpy(dst, src, size);
}
