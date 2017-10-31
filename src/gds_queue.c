#include <string.h>
#include <assert.h>

#include "gds_queue.h"
#include "gds_common.h"

/*number of elements in one node*/
#define COUNT_ELEMENTS 256

/* 2 ^ SHIFT = COUNT_ELEMENTS*/
#define SHIFT	       8

struct gds_qnode {
	struct gds_qnode *next;
	void			 *end; /*pointer on end of buffer*/
	/*hidden buffer for data will starte here*/
};

static struct gds_qnode *node_create(size_t size);

void
queue_create(struct gds_queue *queue, size_t size)
{	
	struct gds_qnode *node;

	assert(queue != NULL);
	assert(size > 0);

	/*queue is empty, create new node*/
	node = node_create(size);	
	/*remember new node in queue*/
	queue->head  = queue->tail = node;
	/*remember buffer for data*/
	queue->start = queue->end  = TO_NEXT(node, sizeof(struct gds_qnode));
	queue->size  = size;
	queue->count = 0;
}

void
queue_delete(struct gds_queue *queue)
{
	struct gds_qnode *node, *next;
	
	assert(queue != NULL);
	
	node = queue->head;
	while (node != NULL) {
		next = node->next;
		free(node);
		node = next;
	}
}

void
queue_clear(struct gds_queue *queue)
{
	struct gds_qnode *node, *next;

	assert(queue != NULL);

	/*delete all nodes before first*/
	node = queue->head->next;
	while (node != NULL) {
		next = node->next;
		free(node);
		node = next;
	}

	node = queue->head;
	queue->tail  = node;
	queue->start = queue->end = TO_NEXT(node, sizeof(struct gds_qnode));
	node->next   = NULL;
	queue->count = 0;
}

struct gds_qnode *
node_create(size_t size)
{
	struct gds_qnode *node;
	void  *buffer;
	size_t bsize;
	
	assert(size > 0);
	
	bsize = size << SHIFT; /*size of buffer for data*/
	size  = sizeof(struct gds_qnode) + bsize; /*size with header and buffer*/
	node  = (struct gds_qnode *) malloc(size);
	assert(node != NULL);
	
	/*get start of buffer*/
	buffer      = TO_NEXT(node, sizeof(struct gds_qnode));
	/*remember end of buffer*/
	node->end   = TO_NEXT(buffer, bsize); 
	node->next  = NULL;
	return node;
}

void
queue_add(struct gds_queue *queue, void *src)
{
	struct gds_qnode *tail, *node;
	void  *dst;
	size_t size;

	assert(queue != NULL);
	assert(src != NULL);
	
	size = queue->size;
	tail = queue->tail;
	dst  = queue->end;
	if (dst == tail->end) {
		/*queue is full, add new node*/
		node = node_create(size);	
		tail->next  = node;
		/*remember new node*/
		queue->tail = node;
		dst = TO_NEXT(node, sizeof(struct gds_qnode));
	}
	memcpy(dst, src, size);
	queue->end = TO_NEXT(dst, size);
	queue->count++;
}

void
queue_pop(struct gds_queue *queue, void *dst)
{
	struct gds_qnode *head, *node;
	void  *src;
	size_t size;

	assert(queue != NULL);
	assert(dst != NULL);
	
	size = queue->size;
	head = queue->head;
	src  = queue->start;

	if (src == head->end) {
		/*head is end, get next node*/
		node = head->next;
		/*remove empty node*/
		free(head);
		/*remember current node*/
		queue->head = node;
		/*get start of buffer for data*/
		src = TO_NEXT(node, sizeof(struct gds_qnode));
	}
	memcpy(dst, src, size);
	queue->start = TO_NEXT(src, size);
	queue->count--;
}

void
queue_front(struct gds_queue *queue, void *dst)
{
	struct gds_qnode *head, *node;
	void  *src;
	size_t size;

	assert(queue != NULL);
	assert(dst != NULL);
	
	size = queue->size;
	head = queue->head;
	src  = queue->start;
	if (src == head->end) {
		node = head->next;
		free(head);
		queue->head = node;
		src = TO_NEXT(node, sizeof(struct gds_qnode));
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
