#ifndef QUEUE_UNIDIRECTIONAL_LIST
#define QUEUE_UNIDIRECTIONAL_LIST

#include <stdlib.h>

struct gds_queue {
	struct gds_list *head;
	struct gds_list *tail;
	void  *start; /*current pointer on data of head*/
	void  *end;   /*current pointer on data of tail*/
	size_t size;  /*size of one element*/
	size_t count;
};

void queue_create(struct gds_queue *queue, size_t size);
void queue_delete(struct gds_queue *queue);

void queue_clear(struct gds_queue *queue); 

void queue_enqueue(struct gds_queue *queue, void *src);
void queue_dequeue(struct gds_queue *queue, void *dst);
void queue_peek(struct gds_queue *queue, void *dst);
#endif
