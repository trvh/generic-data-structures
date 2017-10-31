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
	void			 *current; /*pointer on free place in buffer*/
	void			 *end;	  /*pointer on end of buffer*/
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
	stack->list   = node;
	/*shift to start of buffer*/
	node->current = TO_NEXT(node, sizeof(struct gds_snode));
	stack->count  = 0;
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
	buffer        = TO_NEXT(node, sizeof(struct gds_snode));
	node->current = buffer;
	/*shift to end of buffer*/
	node->end     = TO_NEXT(buffer, bsize);
	
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
	dst  = node->current;
	if (dst == node->end) {		
		/*stack is full, add new node*/
		node_new = node_create(size);
		/*put new node on top of the stack*/
		stack->list = node_new;
		/*remember previous node*/
		node_new->prev = node;
		/*change current node*/
		node = node_new;
		dst  = node_new->current;	
	}
	memcpy(dst, src, size);
	node->current = TO_NEXT(dst, size);
	stack->count++;
}

void
stack_pop(struct gds_stack *stack, void *dst)
{
	struct gds_snode *node;
	void  *src, *start;
	size_t size;
	
	assert(stack != NULL);
	assert(dst != NULL);
	
	size  = stack->size;
	node  = stack->list;
	src   = node->current;
	/*shift to start of buffer*/
	start = TO_NEXT(node, sizeof(struct gds_snode));
	if (src == start) {
		/*currentent node is empty*/
		stack->list = node->prev;
		/*remove node*/
		free(node);
		/*change current node*/
		node = stack->list;
		src  = node->current;
	}
	src = TO_PREV(src, size);
	memcpy(dst, src, size);
	node->current = src;	
	stack->count--;
}

void
stack_pop2(struct gds_stack *stack)
{
	struct gds_snode *node;
	void  *src, *start;
	size_t size;
	
	assert(stack != NULL);
	
	size  = stack->size;
	node  = stack->list;
	src   = node->current;
	start = TO_NEXT(node, sizeof(struct gds_snode));
	if (src == start) {
		/*currentent node is empty*/
		stack->list = node->prev;
		free(node);
		node = stack->list;
		src  = node->current;
	}
	node->current = TO_PREV(src, size);	
	stack->count--;
}

void
stack_peek(struct gds_stack *stack, void *dst)
{
	struct gds_snode *node;
	void  *src, *start;
	size_t size;
	
	assert(stack != NULL);
	assert(dst != NULL);
	
	size = stack->size;
	node = stack->list;
	src  = node->current;
	start = TO_NEXT(node, sizeof(struct gds_snode));
	if (src == start)
		src = node->prev->current;
	src = TO_PREV(src, size);	
	memcpy(dst, src, size);
}
