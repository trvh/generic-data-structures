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
	struct gds_dnode *new_node, *head;
	void  *dst;
	size_t size;
	
	assert(list != NULL);
	assert(src != NULL);
	
	size = list->size;
	new_node = (struct gds_dnode *) malloc(sizeof(struct gds_dnode) + size);
	assert(new_node != NULL);
	
	/*skip header*/
	dst = TO_NEXT(new_node, sizeof(struct gds_dnode));
	memcpy(dst, src, size);
	
	head = list->head;
	if (head == NULL) /*it is first node?*/
		list->tail = new_node;
	else
		head->prev = new_node;

	new_node->next = head;
	new_node->prev = NULL;
	list->head = new_node;
	list->count++;
	return new_node;
}

struct gds_dnode *
dlist_intail(struct gds_dlist *list, void *src)
{
	struct gds_dnode *new_node, *tail;
	void  *dst;
	size_t size;
	
	assert(list != NULL);
	assert(src != NULL);
	
	size = list->size;
	new_node = (struct gds_dnode *) malloc(sizeof(struct gds_dnode) + size);
	assert(new_node != NULL);
	
	/* skip header*/
	dst = TO_NEXT(new_node, sizeof(struct gds_dnode));
	memcpy(dst, src, size);
	
	tail = list->tail;
	if (tail == NULL) { /*it is first node?*/
		list->head = new_node;
	else
		tail->next = new_node;	

	new_node->prev = tail;
	new_node->next = NULL;
	list->tail = new_node;
	list->count++;
	return new_node;
}

struct gds_dnode *
dlist_after(struct gds_dlist *list, struct gds_dnode *node, void *src)
{
	struct gds_dnode *new_node, *tail;
	void  *dst;
	size_t size;
	
	assert(list != NULL);
	assert(list->head != NULL);
	assert(node != NULL);
	assert(src != NULL);
	
	size = list->size;
	new_node = (struct gds_dnode *) malloc(sizeof(struct gds_dnode) + size);
	assert(new_node != NULL);
	
	dst = TO_NEXT(new_node, sizeof(struct gds_dnode));
	memcpy(dst, src, size);
	
	tail = list->tail;
	if (tail == node) {
		new_node->prev = tail;
		tail->next = new_node;
		new_node->next = NULL;
		list->tail = new_node;
	} else {
		new_node->next = node->next;
		node->next = new_node;
		new_node->prev = node;
	}
	list->count++;
	return new_node;
}

struct gds_dnode *
dlist_before(struct gds_dlist *list, struct gds_dnode *node, void *src)
{
	struct gds_dnode *new_node, *head;
	void  *dst;
	size_t size;
	
	assert(list != NULL);
	assert(list->head != NULL);
	assert(node != NULL);
	assert(src != NULL);
	
	size = list->size;
	new_node = (struct gds_dnode *) malloc(sizeof(struct gds_dnode) + size);
	assert(new_node != NULL);
	
	dst = TO_NEXT(new_node, sizeof(struct gds_dnode));
	memcpy(dst, src, size);
	
	head = list->head;
	if (head == node) {		
		new_node->next = head;
		head->prev = new_node;
		new_node->prev = NULL;
		list->head = new_node;
	} else {
		new_node->next = node;
		new_node->prev = node->prev;
		node->prev = new_node;
	}
	list->count++;
	return new_node;
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
		prev = node->prev;
		prev->next = node->next;
		if (node == list->tail) /*node was tail?*/
			list->tail = prev;
	}
	free(node);
	list->count--;
}
