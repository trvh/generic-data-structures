#include <string.h>
#include <assert.h>
#include "gds_queue.h"

/* 2 ^ SHIFT = COUNT_ELEMENTS in buffer*/
#define SHIFT	       13
#define COUNT_ELEMENTS 8192

#define TO_NEXT(buffer, size) (((char *) (buffer)) + (size))
#define TO_PREV(buffer, size) (((char *) (buffer)) - (size))

struct gds_list {
	struct gds_list *next;	
	void			*start; /*pointer to the beginning of the buffer*/
	void			*end;   /*pointer on end of buffer*/
};

static struct gds_list *list_create(size_t size);

void
queue_create(struct gds_queue *queue, size_t size)
{	
	struct gds_list *list;

	assert(queue != NULL);
	assert(size > 0);
	

	/*queue is empty, create new list*/
	list = list_create(size);
	
	/*remember new list in queue*/
	queue->head  = queue->tail = list;
	queue->start = queue->end = list->start;
	queue->size  = size;
	queue->count = 0;
}

void
queue_delete(struct gds_queue *queue)
{
	struct gds_list *list, *next;
	
	assert(queue != NULL);
	
	list = queue->head;
	while (list != NULL) {
		next = list->next;
		free(list->start);
		free(list);
		list = next;
	}
}

void
queue_clear(struct gds_queue *queue)
{
	struct gds_list *list, *next;

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
	queue->count = 0;
}

struct gds_list *
list_create(size_t size)
{
	struct gds_list *list;
	void  *buffer;
	
	list = (struct gds_list *) malloc(sizeof(struct gds_list));
	assert(list != NULL);
	
	size = size << SHIFT;
	buffer = malloc(size);
	assert(buffer != NULL);
	
	list->start = buffer;
	list->end   = TO_NEXT(buffer, size); 
	list->next  = NULL;

	return list;
}

void
queue_enqueue(struct gds_queue *queue, void *src)
{
	struct gds_list *tail, *list;
	void  *dst;
	size_t size;

	assert(queue != NULL);
	assert(src != NULL);
	
	size = queue->size;
	tail = queue->tail;
	dst  = queue->end;
	
	if (dst == tail->end) {
		/*queue is full, add new list*/
		list = list_create(size);	
		tail->next  = list;
		queue->tail = list;
		dst = list->start;
	}
	memcpy(dst, src, size);
	queue->end = TO_NEXT(dst, size);
	queue->count++;
}

void
queue_dequeue(struct gds_queue *queue, void *dst)
{
	struct gds_list *head, *list;
	void  *src;
	size_t size;

	assert(queue != NULL);
	assert(dst != NULL);
	
	size = queue->size;
	head = queue->head;
	src  = queue->start;

	if (src == head->end) {
		/*head is end, remove list*/
		list = head->next;
		free(head->start);
		free(head);
		queue->head = list;
		src = list->start;
	}
	memcpy(dst, src, size);
	queue->start = TO_NEXT(src, size);
	queue->count--;
}

void
queue_peek(struct gds_queue *queue, void *dst)
{
	struct gds_list *head, *list;
	void  *src;
	size_t size;

	assert(queue != NULL);
	assert(dst != NULL);
	
	size = queue->size;
	head = queue->head;
	src  = queue->start;

	if (src == head->end) {
		/*head is end, remove list*/
		list = head->next;
		free(head->start);
		free(head);
		queue->head = list;
		src = list->start;
	}
	memcpy(dst, src, size);
	queue->start = src;
}
