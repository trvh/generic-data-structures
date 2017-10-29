#include <stdio.h>
#include "../src/gds_dlist.h"

void
test1()
{
	struct gds_dlist l1;
	int i;
	int *res;

	dlist_create(&l1, sizeof(int), cmp);

	i = 1;
	dlist_insert(&l1, &i, 0);
	i = 2;
	dlist_append(&l1, &i);
	i = 3;
	dlist_append(&l1, &i);
	i = 777;
	dlist_insert(&l1, &i, 3);

	i = 1;
	res = dlist_search(&l1, &i);
	
	i = 3;
	res = dlist_search(&l1, &i);
	
	res = dlist_index(&l1, 3);

	printf("%d\n", *res);
	
	i = 1;
	dlist_remove2(&l1, &i, ALL_NODES);

	dlist_delete(&l1);
}

void
test2()
{
	struct gds_dlist l1;
	int i;

	dlist_create(&l1, sizeof(int), cmp);

	i = 1;
	dlist_remove2(&l1, &i, ALL_NODES);
	
	i = 1;
	dlist_insert(&l1, &i, 0);
	
	i = 1;
	dlist_remove2(&l1, &i, ALL_NODES);
	
	i = 2;
	dlist_append(&l1, &i);
	i = 3;
	dlist_append(&l1, &i);
	i = 1;
	dlist_insert(&l1, &i, 2);
	i = 1;
	dlist_insert(&l1, &i, 0);

	i = 1;
	dlist_remove2(&l1, &i, ALL_NODES);
	
	dlist_delete(&l1);
}

void
test3()
{
	struct gds_dlist l1;
	int i;
	int data;

	dlist_create(&l1, sizeof(int), cmp);

	i = 1;
	dlist_insert(&l1, &i, 0);
	
	i = 2;
	dlist_append(&l1, &i);
	i = 3;
	dlist_append(&l1, &i);

	dlist_getdata(&l1, &data, 2);
	
	dlist_delete(&l1);
}

void
test4()
{
	struct gds_dlist l1;
	int i;
	int *data;

	dlist_create(&l1, sizeof(int), cmp);

	i = 1;
	dlist_insert(&l1, &i, 0);
	
	i = 2;
	dlist_append(&l1, &i);
	i = 3;
	dlist_append(&l1, &i);
	
	i = 1;
	data = (int *) dlist_search(&l1, &i);
	dlist_remove(&l1, GET_PTR_NODE(data));

	i = 3;
	data = (int *) dlist_search(&l1, &i);
	dlist_remove(&l1, GET_PTR_NODE(data));
	
	dlist_delete(&l1);
}

int
main()
{
	test1();	
	test2();	
	test3();	
	test4();	
	return 0;
}

