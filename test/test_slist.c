#include <stdio.h>
#include "../src/gds_slist.h"

int
cmp(void *p1, void *p2)
{
	int *i1 = (int *) p1;
	int *i2 = (int *) p2;
	return (*i1 == *i2) ? (0) : (1);
}

void
test1()
{
	struct gds_slist l1;
	int i;
	int *res;

	slist_create(&l1, sizeof(int), cmp);

	i = 1;
	slist_insert(&l1, &i, 0);
	i = 2;
	slist_append(&l1, &i);
	i = 3;
	slist_append(&l1, &i);
	i = 777;
	slist_insert(&l1, &i, 3);

	i = 1;
	res = slist_search(&l1, &i);
	
	i = 3;
	res = slist_search(&l1, &i);
	
	res = slist_index(&l1, 3);
	
	i = 1;
	slist_remove(&l1, &i, ALL_NODES);

	slist_delete(&l1);
}

void
test2()
{
	struct gds_slist l1;
	int i;

	slist_create(&l1, sizeof(int), cmp);

	i = 1;
	slist_remove(&l1, &i, ALL_NODES);
	
	i = 1;
	slist_insert(&l1, &i, 0);
	
	i = 1;
	slist_remove(&l1, &i, ALL_NODES);
	
	i = 2;
	slist_append(&l1, &i);
	i = 3;
	slist_append(&l1, &i);
	i = 1;
	slist_insert(&l1, &i, 2);
	i = 1;
	slist_insert(&l1, &i, 0);

	i = 1;
	slist_remove(&l1, &i, ALL_NODES);
	
	slist_delete(&l1);
}

void
test3()
{
	struct gds_slist l1;
	int i;
	int data;

	slist_create(&l1, sizeof(int), cmp);

	i = 1;
	slist_insert(&l1, &i, 0);
	
	i = 2;
	slist_append(&l1, &i);
	i = 3;
	slist_append(&l1, &i);

	slist_getdata(&l1, &data, 2);
	
	slist_delete(&l1);
}
int
main()
{
	test1();	
	test2();	
	test3();	
	return 0;
}

