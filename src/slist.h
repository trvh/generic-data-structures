#ifndef SLIST_HEADER
#define SLIST_HEADER

#include <stdlib.h>

struct node_gds {
	struct node_gds *next;
	void			*data;
};

struct slist_gds {
	struct node_gds *head;
	size_t			 size;
	size_t			 count;
};

void slist_create(struct slist_gds *list, size_t size);
void slist_delete(struct slist_gds *list);

void slist_insert(struct slist_gds *list, void *data);
void slist_remove(struct slist_gds *list, void *data);

void *slist_search(struct slist_gds *list, void *data);
void *slist_index(struct slist_gds *list, size_t index);
#endif
