#ifndef SLIST_HEADER
#define SLIST_HEADER

#include <stdlib.h>

struct gds_node {
	struct gds_node *next;
	void			*data;
};

struct gds_slist {
	struct gds_node *head;
	size_t			 size;
	size_t			 count;
};

void slist_create(struct gds_slist *list, size_t size);
void slist_delete(struct gds_slist *list);

void slist_insert(struct gds_slist *list, void *data);
void slist_remove(struct gds_slist *list, void *data);

void *slist_search(struct gds_slist *list, void *data);
void *slist_index(struct gds_slist *list, size_t index);
#endif
