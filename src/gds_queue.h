#ifndef QUEUE_UNIDIRECTIONAL_LIST
#define QUEUE_UNIDIRECTIONAL_LIST

#include <stdlib.h>
#include "gds_consts.h"

struct gds_queue {
	struct gds_list *head;  /*stores the first list*/
	struct gds_list *tail;  /*stores the last list*/
	void			*start; /*current pointer of first list*/
	void			*end;   /*current pointer of last list*/
	size_t			 size;  /*size of one element*/
};

void queue_create(struct gds_queue *queue, size_t size);
void queue_delete(struct gds_queue *queue);

int    queue_is_empty(struct gds_queue *queue);
void   queue_clear(struct gds_queue *queue); 
size_t queue_count(struct gds_queue *queue);

void   queue_enqueue(struct gds_queue *queue, void *value);
void   queue_dequeue(struct gds_queue *queue, void *value);
void   queue_peek(struct gds_queue *queue, void *value);
#endif
