#ifndef SLIST_HEADER
#define SLIST_HEADER

#include <stdlib.h>

enum {
	ALL_NODES,
	ANY_NODE,
};

struct gds_node {
	struct gds_node *next;
	void *data;
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

void slist_append(struct gds_slist *list, void *src);
void slist_insert(struct gds_slist *list, void *src, size_t index);

void slist_remove(struct gds_slist *list, void *pattern, int option);

void *slist_search(struct gds_slist *list, void *pattern);
void *slist_index(struct gds_slist *list, size_t index);

void slist_getdata(struct gds_slist *list, void *dst, size_t index);
#endif
