#ifndef SLIST_HEADER
#define SLIST_HEADER

#include <stdlib.h>

enum {
	ALL_NODES,
	ANY_NODE,
};

struct gds_slist {
	struct gds_node *head;
	struct gds_node *tail;
	size_t			 size;
	size_t			 count;
	int (*cmp)(void *data, void *pattern); 
};

void slist_create(struct gds_slist *list, size_t size,
				  int (*cmp)(void *data, void *pattern));
void slist_delete(struct gds_slist *list);

void slist_append(struct gds_slist *list, void *data);
void slist_insert(struct gds_slist *list, void *data, size_t index);

void slist_remove(struct gds_slist *list, void *data, int option);

void *slist_search(struct gds_slist *list, void *data);
void *slist_index(struct gds_slist *list, size_t index);
#endif
