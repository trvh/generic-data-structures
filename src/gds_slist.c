#include <string.h>
#include <assert.h>

#include "gds_slist.h"

void
slist_create(struct gds_slist *list, size_t size)
{
	assert(list != NULL);
	assert(size > 0);

	list->head  = NULL;
	list->size  = size;
	list->count = 0;
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
slist_insert(struct gds_slist *list, void *data)
{
	struct gds_node  *node;
	void  *buffer;
	size_t size;
	
	assert(list != NULL);
	assert(data != NULL);
	
	size   = list->size;
	buffer = malloc(size);
	
	memcpy(buffer, data, size);
	
	node = (struct gds_node *) malloc(sizeof(struct gds_node));
	
	node->data = buffer;
	node->next = list->head;
	list->head = node;
	list->count++;
}

void
slist_remove(struct gds_slist *list, void *data)
{
	struct gds_node *prev, *node, *next;
	size_t size = list->size;
	
	assert(list != NULL);
	assert(data != NULL);
	
	for (node = list->head; node != NULL; node = next) {
		next = node->next;
		if (memcmp(node->data, data, size) == 0) {
			free(node->data);
			free(node);
			list->count--;

			if (node == list->head)
				list->head = next;
			else
				prev->next = next;
			continue;
		}
		prev = node;
	}
}

void *
slist_search(struct gds_slist *list, void *data)
{
	struct gds_node *node, *next;
	size_t size = list->size;
	
	assert(list != NULL);
	assert(data != NULL);
	
	for (node = list->head; node != NULL; node = next) {
		next = node->next;
		if (memcmp(node->data, data, size) == 0)
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
