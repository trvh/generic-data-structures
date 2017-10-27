#include <string.h>
#include <assert.h>

#include "gds_slist.h"

struct gds_node {
	struct gds_node *next;
	void			*data;
};

void
slist_create(struct gds_slist *list, size_t size,
			 int (*cmp)(void *data, void *pattern))
{
	assert(list != NULL);
	assert(cmp  != NULL);
	assert(size > 0);

	list->head  = list->tail = NULL;
	list->size  = size;
	list->count = 0;
	list->cmp   = cmp;
}

void
slist_delete(struct gds_slist *list)
{
	struct gds_node *node, *next;

	assert(list != NULL);
	
	for (node = list->head; node != NULL; node = next) {
		next = node->next;
		free(node->data);
		free(node);
	}
}

void
slist_insert(struct gds_slist *list, void *data, size_t index)
{
	struct gds_node *node, *head, *tail, *prev;
	void  *buffer;
	size_t size, count;
	
	assert(list != NULL);
	assert(data != NULL);
	
	size   = list->size;	
	buffer = malloc(size);		
	assert(buffer != NULL);
	
	memcpy(buffer, data, size);
	
	node = (struct gds_node *) malloc(sizeof(struct gds_node));
	assert(node != NULL);
	
	node->data = buffer;
	
	count = list->count;	
	assert(count >= index);

	if (count == index) {
		tail = list->tail;
		if (count == 0) {
			/*it is first node*/
			list->head = list->tail = node;
		} else {
			/*add in end*/
			tail->next = node;
			list->tail = node;
		}
		node->next = NULL;
	} else {
		head = list->head;
		if (index == 0) {
			/*add in start*/
			node->next = head;
			list->head = node;
		} else {
			/*find previous node*/
			for (prev = head; index != 1; index--, prev = prev->next)
				{}
			/*insert between nodes*/
			node->next = prev->next;
			prev->next = node;
		}
	}
	list->count++;
}

void
slist_append(struct gds_slist *list, void *data)
{
	struct gds_node *node, *head;
	void  *buffer;
	size_t size;
	
	assert(list != NULL);
	assert(data != NULL);
	
	size   = list->size;	
	buffer = malloc(size);		
	assert(buffer != NULL);
	
	memcpy(buffer, data, size);
	
	node = (struct gds_node *) malloc(sizeof(struct gds_node));
	assert(node != NULL);
	
	node->data = buffer;
	node->next = NULL;
	
	head = list->head;
	if (head == NULL) {		
		list->head = list->tail = node;
	} else {
		head->next = node;
		list->head = node;
	}
	list->count++;
}

void
slist_remove(struct gds_slist *list, void *data, int option)
{
	struct gds_node *prev, *node, *next;
	int (*cmp)(void *data, void *pattern); 
	
	assert(list != NULL);
	assert(data != NULL);
	
	cmp = list->cmp;
	for (node = list->head; node != NULL; node = next) {
		next = node->next;
		if ((*cmp)(node->data, data) == 0) {
			free(node->data);
			free(node);
			list->count--;

			if (node == list->head)
				list->head = next;
			else
				prev->next = next;

			if (option == ALL_NODES)
				continue;
			else
				break;
		}
		prev = node;
	}
}

void *
slist_search(struct gds_slist *list, void *data)
{
	struct gds_node *node, *next;
	int (*cmp)(void *data, void *pattern); 
	
	assert(list != NULL);
	assert(data != NULL);
	
	cmp = list->cmp;
	for (node = list->head; node != NULL; node = next) {
		next = node->next;
		if ((*cmp)(node->data, data) == 0)
			break;
	}
	return (node != NULL) ? node->data : NULL;
}

void *
slist_index(struct gds_slist *list, size_t index)
{
	struct gds_node *node;
		
	assert(list != NULL);
	assert(list->count > 0);
	assert(list->count - 1 >= index);

	for (node = list->head; index != 0; node = node->next, index--)
		{}
	return node->data;
}
