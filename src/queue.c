#include <string.h>
#include <assert.h>
#include "queue.h"

/* 2 ^ SHIFT = COUNT_ELEMENTS in buffer*/
#define SHIFT	       13
#define COUNT_ELEMENTS 8192

#define TO_NEXT(buffer, size) (((char *) buffer) + size)
#define TO_PREV(buffer, size) (((char *) buffer) - size)

#define GET_END(buffer, size) (((char *) buffer) + (size << SHIFT))

#define COPY(target, source, size) memcpy(target, source, size);

#define GET_REMAINDER(start, end, size) (( ((char *) end) - ((char *) start) ) / size)

struct list_gds {
	struct list_gds *next;	
	void			*start; /*pointer to the beginning of the buffer*/
	void			*end;   /*pointer on end of buffer*/
};

static struct list_gds *list_create(size_t size);

void
queue_create(struct queue_gds *queue, size_t size)
{	
	struct list_gds *list;

	assert(queue != NULL);
	assert(size > 0);
	
	/*setup actual size of elements*/
	queue->size = size;

	/*queue is empty, create new list*/
	list = list_create(size);
	list->next = NULL;
	
	/*remember new list in queue*/
	queue->head = queue->tail = list;
	queue->start = queue->end = list->start;
}

void
queue_delete(struct queue_gds *queue)
{
	assert(queue != NULL);
	
	/*delete all lists of queue*/
	queue_clear(queue);
	
	free(queue->head->start);
	free(queue->head);
}

void
queue_clear(struct queue_gds *queue)
{
	struct list_gds *list, *next;

	assert(queue != NULL);

	/*delete all lists before first*/
	list = queue->head->next;
	while (list != NULL) {
		next = list->next;
		free(list->start);
		free(list);
		list = next;
	}
	
	queue->tail = queue->head;
	queue->end  = queue->start;
	queue->head->next = NULL;
}

size_t
queue_count(struct queue_gds *queue)
{
	struct list_gds *next;
	size_t count;

	assert(queue != NULL);
	
	if (queue->head == queue->tail) {
		count = GET_REMAINDER(queue->start, queue->end, queue->size);
	} else {
		count = GET_REMAINDER(queue->start, queue->head->end, queue->size);
		for (next = queue->head->next; next != queue->tail; next = next->next)
			count += COUNT_ELEMENTS;
		count += GET_REMAINDER(queue->tail->start, queue->end, queue->size);
	}
	return count;
}

struct list_gds *
list_create(size_t size)
{
	struct list_gds *list;
	void  *buffer;
	
	list = (struct list_gds *) malloc(sizeof(struct list_gds));
	assert(list != NULL);
	
	/*size of buffer of data*/	
	buffer = malloc(size << SHIFT);
	assert(buffer != NULL);
	
	list->start = buffer;
	list->end   = GET_END(buffer, size); 

	return list;
}

void
queue_enqueue(struct queue_gds *queue, void *value)
{
	struct list_gds *tail, *list;
	void  *end;
	size_t size;

	assert(queue != NULL);
	assert(value != NULL);
	
	end  = queue->end;
	size = queue->size;
	tail = queue->tail;

	if (end < tail->end) {
		COPY(end, value, size)
		queue->end = TO_NEXT(end, size);
	} else {
		/*queue is full, add new list*/
		list = list_create(size);
		
		/*remember reference on old list*/
		tail->next = list;

		queue->tail = list;
		
		COPY(list->start, value, size)
		
		queue->end = TO_NEXT(list->start, size);
	}
}

void
queue_dequeue(struct queue_gds *queue, void *value)
{
	struct list_gds *head, *list;
	void  *start;
	size_t size;

	assert(queue != NULL);
	assert(value != NULL);
	
	start = queue->start;
	size  = queue->size;
	head  = queue->head;

	if (start < head->end) {
		COPY(value, start, size)
		queue->start = TO_NEXT(start, size);
	} else {
		/*queue is end, remove list*/
		list = head->next;
		free(head->start);
		free(head);

		/*remember reference on next list*/
		queue->head = list;

		COPY(value, list->start, size)
		
		queue->start = TO_NEXT(list->start, size);
	}
}

void
queue_peek(struct queue_gds *queue, void *value)
{
	struct list_gds *head, *list;
	void  *start;
	size_t size;

	assert(queue != NULL);
	assert(value != NULL);
	
	start = queue->start;
	size  = queue->size;
	head  = queue->head;

	if (start < head->end) {
		COPY(value, start, size)
	} else {
		/*queue is end, remove list*/
		list = head->next;
		free(head->start);
		free(head);

		/*remember reference on next list*/
		queue->head = list;

		COPY(value, list->start, size)
		
		queue->start = list->start;
	}
}

int
queue_is_empty(struct queue_gds *queue)
{
	assert(queue != NULL);
	
	if (queue->head == queue->tail)
		if(queue->start == queue->end)
			return IS_EMPTY;
	return IS_FILLED;
}
