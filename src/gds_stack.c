#include <string.h>
#include <assert.h>
#include "gds_stack.h"

/* 2 ^ SHIFT = COUNT_ELEMENTS in buffer*/
#define SHIFT	       13
#define COUNT_ELEMENTS 8192

#define TO_NEXT(buffer, size) (((char *) (buffer)) + (size))
#define TO_PREV(buffer, size) (((char *) (buffer)) - (size))

struct gds_list {
	struct gds_list *prev;	
	void			*start;   /*pointer to start of the buffer*/
	void			*current; /*pointer on free place in buffer*/
	void			*end;	  /*pointer on end of buffer*/
};

static struct gds_list *list_create(size_t size);

void
stack_create(struct gds_stack *stack, size_t size)
{	
	struct gds_list *list;
	
	assert(stack != NULL);
	assert(size > 0);
	
	/*stack is empty, create new list*/
	list = list_create(size);
	list->prev = NULL;
	
	stack->list  = list;
	stack->size  = size;
	stack->count = 0;
}

void
stack_delete(struct gds_stack *stack)
{
	struct gds_list *list, *prev;

	assert(stack != NULL);
	
	list = stack->list;
	while (list != NULL) {
		prev = list->prev;
		free(list->start);
		free(list);
		list = prev;
	}
}

void
stack_clear(struct gds_stack *stack)
{
	struct gds_list *list, *prev;

	assert(stack != NULL);
	
	/*delete all lists before first*/
	list = stack->list;
	while (list->prev != NULL) {
		prev = list->prev;
		free(list->start);
		free(list);
		list = prev;
	}
	stack->list   = list;
	stack->count  = 0;
	list->current = list->start;
}

struct gds_list *
list_create(size_t size)
{
	struct gds_list *list;
	void  *buffer;
	
	list = (struct gds_list *) malloc(sizeof(struct gds_list));
	assert(list != NULL);
	
	buffer = malloc(size << SHIFT);
	assert(buffer != NULL);
	
	list->start = list->current = buffer;
	size = size << SHIFT;
	list->end = TO_NEXT(buffer, size);
	return list;
}

void
stack_push(struct gds_stack *stack, void *src)
{
	struct gds_list *list, *list_new;
	void  *dst;
	size_t size;

	assert(stack != NULL);
	assert(src != NULL);
	
	size = stack->size;	
	list = stack->list;
	dst  = list->current;

	if (dst == list->end) {		
		/*stack is full, add new list*/
		list_new = list_create(size);
		stack->list = list_new;
		list_new->prev = list;
		dst = list_new->current;	
	}
	memcpy(dst, src, size);
	list->current = TO_NEXT(dst, size);
	stack->count++;
}

void
stack_pop(struct gds_stack *stack, void *dst)
{
	struct gds_list *list;
	void  *src;
	size_t size;
	
	assert(stack != NULL);
	assert(dst != NULL);
	
	size = stack->size;
	list = stack->list;
	src  = list->current;

	if (src == list->start) {
		/*if currentent list is empty*/
		stack->list = list->prev;
		free(list->start);
		free(list);
		list = stack->list;
		src  = list->current;
	}
	src = TO_PREV(src, size);	
	memcpy(dst, src, size);
	list->current = src;	
	stack->count--;
}

void
stack_pop2(struct gds_stack *stack)
{
	struct gds_list *list;
	void  *src;
	size_t size;
	
	assert(stack != NULL);
	
	size = stack->size;
	list = stack->list;
	src  = list->current;

	if (src == list->start) {
		/*if currentent list is empty*/
		stack->list = list->prev;
		free(list->start);
		free(list);
		list = stack->list;
		src  = list->current;
	}
	list->current = TO_PREV(src, size);	
	stack->count--;
}

void
stack_peek(struct gds_stack *stack, void *dst)
{
	struct gds_list *list;
	void  *src;
	size_t size;
	
	assert(stack != NULL);
	assert(dst != NULL);
	
	size = stack->size;
	list = stack->list;
	src  = list->current;

	if (src == list->start) {
		/*if currentent list is empty*/
		src = list->prev->current;
	}
	src = TO_PREV(src, size);	
	memcpy(dst, src, size);
}
