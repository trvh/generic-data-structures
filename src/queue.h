#ifndef QUEUE_UNIDIRECTIONAL_LIST
#define QUEUE_UNIDIRECTIONAL_LIST

#include <stdlib.h>
#include "common_consts.h"

struct queue_gds {
	struct list_gds *head;  /*stores the first list*/
	struct list_gds *tail;  /*stores the last list*/
	void			*start; /*current pointer of first list*/
	void			*end;   /*current pointer of last list*/
	size_t			 size;  /*size of one element*/
};

void queue_create(struct queue_gds *queue, size_t size);
void queue_delete(struct queue_gds *queue);

int    queue_is_empty(struct queue_gds *queue);
void   queue_clear(struct queue_gds *queue); 
size_t queue_count(struct queue_gds *queue);

void   queue_enqueue(struct queue_gds *queue, void *value);
void   queue_dequeue(struct queue_gds *queue, void *value);
void   queue_peek(struct queue_gds *queue, void *value);
#endif
