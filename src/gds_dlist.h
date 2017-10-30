#ifndef GDS_DOUBLE_LIST_HEADER
#define GDS_DOUBLE_LIST_HEADER

#include <stdlib.h>

/*offset from header of node, here start of buffer for data*/
#define DATA_DNODE(node) \
	((void *) (((char *) (node)) + (sizeof(struct gds_dnode))))

struct gds_dnode {
	struct gds_dnode *next;
	struct gds_dnode *prev;
	/*hidden buffer for data will place here*/
};

struct gds_dlist {
	struct gds_dnode *head;
	struct gds_dnode *tail;
	size_t			  size;
	size_t			  count;
};

void dlist_create(struct gds_dlist *list, size_t size);
void dlist_delete(struct gds_dlist *list);

struct gds_dnode *dlist_inhead(struct gds_dlist *list, void *src);
struct gds_dnode *dlist_intail(struct gds_dlist *list, void *src);

struct gds_dnode *dlist_after(struct gds_dlist *list,
							 struct gds_dnode *node, void *src);
struct gds_dnode *dlist_before(struct gds_dlist *list,
							  struct gds_dnode *node, void *src);

void dlist_remove(struct gds_dlist *list, struct gds_dnode *node);
#endif
