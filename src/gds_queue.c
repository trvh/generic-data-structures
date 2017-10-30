#include <string.h>
#include <assert.h>

#include "gds_queue.h"
#include "gds_common.h"

/*number of elements in one list*/
#define COUNT_ELEMENTS 256
/* 2 ^ SHIFT = COUNT_ELEMENTS*/
#define SHIFT	       8

struct gds_list {
	struct gds_list *next;
	void			*end; /*pointer on end of buffer*/
	/*hidden buffer for data will place here*/
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
	/*remember buffer for data*/
	queue->start = queue->end  = TO_NEXT(list, sizeof(struct gds_list));
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
		free(list);
		list = next;
	}

	list = queue->head;
	queue->tail  = list;
	queue->start = queue->end = TO_NEXT(list, sizeof(struct gds_list));
	list->next   = NULL;
	queue->count = 0;
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
	
	/*get start of buffer*/
	buffer      = TO_NEXT(list, sizeof(struct gds_list));
	/*remember end of buffer*/
	list->end   = TO_NEXT(buffer, bsize); 
	list->next  = NULL;
	return list;
}

void
queue_add(struct gds_queue *queue, void *src)
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
		/*remember new list*/
		queue->tail = list;
		dst = TO_NEXT(list, sizeof(struct gds_list));
	}
	memcpy(dst, src, size);
	queue->end = TO_NEXT(dst, size);
	queue->count++;
}

void
queue_pop(struct gds_queue *queue, void *dst)
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
		/*head is end, get next list*/
		list = head->next;
		/*remove empty list*/
		free(head);
		/*remember current list*/
		queue->head = list;
		/*get start of buffer for data*/
		src = TO_NEXT(list, sizeof(struct gds_list));
	}
	memcpy(dst, src, size);
	queue->start = TO_NEXT(src, size);
	queue->count--;
}

void
queue_front(struct gds_queue *queue, void *dst)
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
		list = head->next;
		free(head);
		queue->head = list;
		src = TO_NEXT(list, sizeof(struct gds_list));
		queue->start = src;
	}
	memcpy(dst, src, size);
}

void
queue_back(struct gds_queue *queue, void *dst)
{
	void  *src;
	size_t size;

	assert(queue != NULL);
	assert(dst != NULL);
	
	size = queue->size;
	src  = TO_PREV(queue->end, size);
	memcpy(dst, src, size);
}
