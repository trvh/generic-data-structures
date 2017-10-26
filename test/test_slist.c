#include <stdio.h>
#include "slist.h"

int
main()
{
	struct slist_gds l1;
	int i;
	int *res;

	slist_create(&l1, sizeof(int));

	i = 1;
	slist_insert(&l1, &i);
	i = 2;
	slist_insert(&l1, &i);
	i = 3;
	slist_insert(&l1, &i);
	i = 1;
	slist_insert(&l1, &i);

	i = 1;
	res = slist_search(&l1, &i);
	printf("%d\n", *res);
	
	i = 3;
	res = slist_search(&l1, &i);
	printf("%d\n", *res);
	
	res = slist_index(&l1, 4);
	
	i = 1;
	slist_remove(&l1, &i);

	slist_delete(&l1);
	
	return 0;
}
