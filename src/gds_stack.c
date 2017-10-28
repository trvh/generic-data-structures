#include <string.h>
#include <assert.h>
#include "gds_stack.h"

/* 2 ^ SHIFT = COUNT_ELEMENTS in buffer*/
#define SHIFT	       13
#define COUNT_ELEMENTS 8192

#define TO_NEXT(buffer, size) (((char *) buffer) + size)
#define TO_PREV(buffer, size) (((char *) buffer) - size)

#define GET_REMAINDER(list, size) ((((char *) list->current) -\
									((char *) list->start)) / size)

#define COPY_IN_STACK(current, value, size)   memcpy(current, value, size);
#define COPY_FROM_STACK(value, current, size) memcpy(value, current, size);

struct gds_list {
	struct gds_list *prev;	
	void			*start;   /*pointer to the beginning of the buffer*/
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
	
	/*setup actual size of elements*/
	stack->size  = size;

	/*stack is empty, create new list*/
	list = list_create(size);
	list->prev = NULL;
	
	/*remember new list in stack*/
	stack->list = list;
}

void
stack_delete(struct gds_stack *stack)
{
	assert(stack != NULL);
	
	stack_clear(stack);
	free(stack->list->start);
	free(stack->list);
}

void
stack_clear(struct gds_stack *stack)
{
	struct gds_list *list, *prev;

	assert(stack != NULL);
	
	/*delete all lists before first*/
	list = stack->list->prev;
	while (list != NULL) {
		prev = list->prev;
		free(list->start);
		free(list);
		list = prev;
	}
	list = stack->list;
	list->prev = NULL;
	list->current = list->start;
}

int
stack_is_empty(struct gds_stack *stack)
{
	struct gds_list *list;
	
	assert(stack != NULL);
	
	if (stack->list->prev == NULL) {
		list = stack->list;
		if (list->current == list->start)
			return IS_EMPTY;
	}
	return IS_FILLED;
}

size_t
stack_count(struct gds_stack *stack)
{
	struct gds_list *list, *prev;
	size_t count;
	
	assert(stack != NULL);
	
	list = stack->list;	
	for (count = 0; list->prev != NULL; ) {
		prev   = list->prev;
		count += COUNT_ELEMENTS;
		list   = prev;
	}
	count += GET_REMAINDER(list, stack->size);
	return count;
}

struct gds_list *
list_create(size_t size)
{
	struct gds_list *list;
	void  *buffer;
	
	list = (struct gds_list *) malloc(sizeof(struct gds_list));
	assert(list != NULL);
	
	/*buffer for data*/	
	buffer = malloc(size << SHIFT);
	assert(buffer != NULL);
	
	/*setup actual values*/
	list->start = list->current = buffer;
	list->end   = TO_NEXT(buffer, (size << SHIFT));

	return list;
}

void
stack_push(struct gds_stack *stack, void *value)
{
	struct gds_list *list, *list_new;
	void  *current;
	size_t size;

	assert(stack != NULL);
	assert(value != NULL);
	
	size = stack->size;	
	list = stack->list;
	current = list->current;

	if (current < list->end) {		
		COPY_IN_STACK(current, value, size)
		/*shift top of stack*/
		current = TO_NEXT(current, size);
		/*remember current position of top of stack*/
		list->current = current;
	} else {
		/*stack is full, add new list*/
		list_new = list_create(size);
		/*remember reference on old list*/
		list_new->prev = list;
		current = list_new->current;	
		
		COPY_IN_STACK(current, value, size)	
		current = TO_NEXT(current, size);
		list_new->current = current;
		/*remember new list in stack*/
		stack->list = list_new;
	}
}

void
stack_pop(struct gds_stack *stack, void *value)
{
	struct gds_list *list;
	void  *current;
	size_t size;
	
	assert(stack != NULL);
	assert(value != NULL);
	
	size = stack->size;
	list = stack->list;
	current = list->current;

	if (current > list->start) {
		/*copy top element of stack*/
		current = TO_PREV(current, size);
		
		COPY_FROM_STACK(value, current, size)
		
		list->current = current;
	} else { 
	/*if currentent list is empty*/
		/*remember previous list*/
		stack->list = list->prev;
		/*free last buffer and list*/
		free(list->start);
		free(list);
		/*get actual list*/
		list = stack->list;
		/*get pointer on top value*/
		current = TO_PREV(list->current, size);
		
		COPY_FROM_STACK(value, current, size)
		
		list->current = current;
	}
}

void
stack_pop2(struct gds_stack *stack)
{
	struct gds_list *list;
	void  *current;
	size_t size;
	
	assert(stack != NULL);
	
	size = stack->size;
	list = stack->list;
	current = list->current;

	if (current > list->start) {
		current = TO_PREV(current, size);
		list->current = current;
	} else { 
		/*remember previous list*/
		stack->list = list->prev;
		/*free buffer and list*/
		free(list->start);
		free(list);
		/*get actual list*/
		list = stack->list;
		/*get pointer on top value*/
		current = TO_PREV(list->current, size);
		list->current = current;
	}
}


void
stack_peek(struct gds_stack *stack, void *value)
{
	struct gds_list *list;
	void  *current;
	size_t size;
	
	assert(stack != NULL);
	assert(value != NULL);
	
	size = stack->size;
	list = stack->list;
	current = list->current;

	if (current > list->start) {
		/*copy top element of stack*/
		current = TO_PREV(current, size);
		
		COPY_FROM_STACK(value, current, size)
	
	} else { 
	/*if currentent list is empty*/
		list = list->prev;
		current = list->current;
		/*get pointer on top value*/
		current = TO_PREV(current, size);
		
		COPY_FROM_STACK(value, current, size)
	
	}
}
