#include <string.h>
#include <assert.h>

#include "gds_dlist.h"
#include "gds_common.h"

void
dlist_create(struct gds_dlist *list, size_t size)
{
	assert(list != NULL);
	assert(size > 0);
	
	list->head  = list->tail = NULL;
	list->size  = size;
	list->count = 0;
}

void
dlist_delete(struct gds_dlist *list)
{
	struct gds_dnode *node, *next;

	assert(list != NULL);
	
	for (node = list->head; node != NULL; node = next) {
		next = node->next;
		free(node);
	}
}

struct gds_dnode *
dlist_inhead(struct gds_dlist *list, void *src)
{
	struct gds_dnode *new, *head;
	void  *dst;
	size_t size;
	
	assert(list != NULL);
	assert(src != NULL);
	
	/*create buffer, which will contents header and data*/
	size = sizeof(struct gds_dnode) + list->size;
	new  = (struct gds_dnode *) malloc(size);
	assert(new != NULL);
	
	/*offset from header of node, here start of buffer for data*/
	dst = TO_NEXT(new, sizeof(struct gds_dnode));
	/*copy data in buffer*/
	memcpy(dst, src, list->size);
	
	head = list->head;
	if (head == NULL) {
		/*it is first node*/
		list->head = list->tail = new;
		new->prev  = new->next  = NULL;
	} else {
		new->next  = head;
		head->prev = new;
		new->prev  = NULL;
		list->head = new;
	}
	list->count++;
	return new;
}

struct gds_dnode *
dlist_intail(struct gds_dlist *list, void *src)
{
	struct gds_dnode *new, *tail;
	void  *dst;
	size_t size;
	
	assert(list != NULL);
	assert(src != NULL);
	
	size = sizeof(struct gds_dnode) + list->size;
	new  = (struct gds_dnode *) malloc(size);
	assert(new != NULL);
	
	dst = TO_NEXT(new, sizeof(struct gds_dnode));
	memcpy(dst, src, list->size);
	
	tail = list->tail;
	if (tail == NULL) {
		/*it is first node*/
		list->head = list->tail = new;
		new->prev  = new->next  = NULL;
	} else {
		new->prev  = tail;
		tail->next = new;
		new->next  = NULL;
		list->tail = new;
	}
	list->count++;
	return new;
}

struct gds_dnode *
dlist_after(struct gds_dlist *list, struct gds_dnode *node, void *src)
{
	struct gds_dnode *new, *tail;
	void  *dst;
	size_t size;
	
	assert(list != NULL);
	assert(list->head != NULL);
	assert(node != NULL);
	assert(src != NULL);
	
	size = sizeof(struct gds_dnode) + list->size;	
	new  = (struct gds_dnode *) malloc(size);
	assert(new != NULL);
	
	dst = TO_NEXT(new, sizeof(struct gds_dnode));
	memcpy(dst, src, list->size);
	
	tail = list->tail;
	if (tail == node) {		
		new->prev  = tail;
		tail->next = new;
		new->next  = NULL;
		list->tail = new;
	} else {
		new->next  = node->next;
		node->next = new;
		new->prev  = node;
	}
	list->count++;
	return new;
}

struct gds_dnode *
dlist_before(struct gds_dlist *list, struct gds_dnode *node, void *src)
{
	struct gds_dnode *new, *head;
	void  *dst;
	size_t size;
	
	assert(list != NULL);
	assert(list->head != NULL);
	assert(node != NULL);
	assert(src != NULL);
	
	size = sizeof(struct gds_dnode) + list->size;	
	new  = (struct gds_dnode *) malloc(size);
	assert(new != NULL);
	
	dst = TO_NEXT(new, sizeof(struct gds_dnode));
	memcpy(dst, src, list->size);
	
	head = list->head;
	if (head == node) {		
		new->next  = head;
		head->prev = new;
		new->prev  = NULL;
		list->head = new;
	} else {
		new->next  = node;
		new->prev  = node->prev;
		node->prev = new;
	}
	list->count++;
	return new;
}

void
dlist_remove(struct gds_dlist *list, struct gds_dnode *node)
{
	struct gds_dnode *head, *prev;

	assert(list != NULL);
	assert(list->head != NULL);
	assert(node != NULL);

	head = list->head;
	if (node == head) {
		if (head == list->tail) /*list was had one node?*/
			list->head = list->tail = NULL;
		else {
			head = head->next;
			head->prev = NULL;
			list->head = head;
		}	
	} else {
		prev       = node->prev;
		prev->next = node->next;
		if (node == list->tail) /*node was tail?*/
			list->tail = prev;
	}
	free(node);
	list->count--;
}
