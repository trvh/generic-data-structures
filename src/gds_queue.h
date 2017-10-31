#ifndef GDS_QUEUE_HEADER
#define GDS_QUEUE_HEADER

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

struct gds_queue {
	struct gds_qnode *head;
	struct gds_qnode *tail;
	void  *start; /*current pointer on data of head*/
	void  *end;   /*current pointer on data of tail*/
	size_t size;  /*size of one element*/
	size_t count; /*current number of elements in queue*/
};

void queue_create(struct gds_queue *queue, size_t size);
void queue_delete(struct gds_queue *queue);

void queue_clear(struct gds_queue *queue); 

void queue_add(struct gds_queue *queue, void *src);
void queue_pop(struct gds_queue *queue, void *dst);

void queue_front(struct gds_queue *queue, void *dst);
void queue_back(struct gds_queue *queue, void *dst);

#ifdef __cplusplus
}
#endif

#endif
