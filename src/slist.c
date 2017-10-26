#include <string.h>
#include <assert.h>

#include "slist.h"

void
slist_create(struct slist_gds *list, size_t size)
{
	assert(list != NULL);
	assert(size > 0);

	list->head  = NULL;
	list->size  = size;
	list->count = 0;
}

void
slist_delete(struct slist_gds *list)
{
	struct node_gds *node, *next;

	assert(list != NULL);
	
	for (node = list->head; node != NULL; node = next) {
		next = node->next;
		free(node->data);
		free(node);
	}
}

void
slist_insert(struct slist_gds *list, void *data)
{
	struct node_gds  *node;
	void  *buffer;
	size_t size;
	
	assert(list != NULL);
	assert(data != NULL);
	
	size   = list->size;
	buffer = malloc(size);
	
	memcpy(buffer, data, size);
	
	node = (struct node_gds *) malloc(sizeof(struct node_gds));
	
	node->data = buffer;
	node->next = list->head;
	list->head = node;
	list->count++;
}

void
slist_remove(struct slist_gds *list, void *data)
{
	struct node_gds *prev, *node, *next;
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
slist_search(struct slist_gds *list, void *data)
{
	struct node_gds *node, *next;
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
slist_index(struct slist_gds *list, size_t index)
{
	struct node_gds *node;
		
	assert(list != NULL);
	assert(list->count > 0);
	assert(list->count - 1 >= index);

	for (node = list->head; index != 0; node = node->next, index--)
		{}
	return node->data;
}
