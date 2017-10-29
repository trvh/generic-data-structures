#include <stdio.h>
#include "../src/gds_dlist.h"

#define MAX 100000

void
test1()
{
	struct gds_dlist l1;
	struct gds_dnode *node, *next;
	int i;
	int *res;

	dlist_create(&l1, sizeof(int));

	i = 1;
	dlist_inhead(&l1, &i);
	i = 2;
	dlist_inhead(&l1, &i);
	i = 3;
	dlist_intail(&l1, &i);
	i = 777;
	dlist_intail(&l1, &i);
	
	for (node = l1.head; node != NULL; node = next) {
		next = node->next;
		res = (int *) DATA_DNODE(node);
		dlist_remove(&l1, node);
	}
	
	dlist_delete(&l1);
}

void
test2()
{
	struct gds_dlist l1;
	struct gds_dnode *node, *next;
	int i;
	int *res;

	dlist_create(&l1, sizeof(int));

	i = 0;
	node = dlist_inhead(&l1, &i);
	for (i = 1; i < MAX; i++) {
		node = dlist_after(&l1, node, &i);
	}

	for (node = l1.tail; node != NULL; node = next) {
		next = node->prev;
		res = (int *) DATA_DNODE(node);
		dlist_remove(&l1, node);
	}
	
	dlist_delete(&l1);
}

int
main()
{
	test1();	
	test2();	
	return 0;
}
