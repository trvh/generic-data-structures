#include <string.h>
#include <assert.h>

#include "gds_stack.h"
#include "gds_common.h"

/*number of elements in one list*/
#define COUNT_ELEMENTS 256
/* 2 ^ SHIFT = COUNT_ELEMENTS*/
#define SHIFT	       8

struct gds_list {
	struct gds_list *prev;
	void			*current; /*pointer on free place in buffer*/
	void			*end;	  /*pointer on end of buffer*/
	/*hidden buffer for data will place here*/
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
		free(list);
		list = prev;
	}
	stack->list   = list;
	/*shift to start of buffer*/
	list->current = TO_NEXT(list, sizeof(struct gds_list));
	stack->count  = 0;
}

struct gds_list *
list_create(size_t size)
{
	struct gds_list *list;
	void  *buffer;
	size_t bsize;
	
	assert(size > 0);

	bsize = size << SHIFT; /*size of buffer for data*/
	size  = sizeof(struct gds_list) + bsize; /*size with header and buffer*/
	list  = (struct gds_list *) malloc(size);
	assert(list != NULL);
	
	/*shift to start of buffer*/
	buffer        = TO_NEXT(list, sizeof(struct gds_list));
	list->current = buffer;
	/*shift to end of buffer*/
	list->end     = TO_NEXT(buffer, bsize);
	
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
		/*put new list on top of the stack*/
		stack->list = list_new;
		/*remember previous list*/
		list_new->prev = list;
		/*change current list*/
		list = list_new;
		dst  = list_new->current;	
	}
	memcpy(dst, src, size);
	list->current = TO_NEXT(dst, size);
	stack->count++;
}

void
stack_pop(struct gds_stack *stack, void *dst)
{
	struct gds_list *list;
	void  *src, *start;
	size_t size;
	
	assert(stack != NULL);
	assert(dst != NULL);
	
	size  = stack->size;
	list  = stack->list;
	src   = list->current;
	/*shift to start of buffer*/
	start = TO_NEXT(list, sizeof(struct gds_list));
	if (src == start) {
		/*currentent list is empty*/
		stack->list = list->prev;
		/*remove list*/
		free(list);
		/*change current list*/
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
	void  *src, *start;
	size_t size;
	
	assert(stack != NULL);
	
	size  = stack->size;
	list  = stack->list;
	src   = list->current;
	start = TO_NEXT(list, sizeof(struct gds_list));
	if (src == start) {
		/*currentent list is empty*/
		stack->list = list->prev;
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
	void  *src, *start;
	size_t size;
	
	assert(stack != NULL);
	assert(dst != NULL);
	
	size = stack->size;
	list = stack->list;
	src  = list->current;
	start = TO_NEXT(list, sizeof(struct gds_list));
	if (src == start)
		src = list->prev->current;
	src = TO_PREV(src, size);	
	memcpy(dst, src, size);
}
