#include <stdio.h>
#include "../src/gds_darray.h"

void
test1()
{
	struct gds_darray arr;
	int i, j;
	int *res;

	darray_create(&arr, sizeof(int));

	i = 1;
	darray_insert(&arr, &i, 0);
	i = 2;
	darray_insert(&arr, &i, 0);
	i = 3;
	darray_insert(&arr, &i, 0);
	i = 4;
	darray_insert(&arr, &i, 0);
	i = 999;
	darray_insert(&arr, &i, 2);
	i = 777;
	darray_insert(&arr, &i, 5);

	for (j = 6, res = (int *) arr.buffer; j != 0; res++, j--)
		printf("%d\n", *res);
	printf("-----\n");
	
	darray_remove(&arr, 3);
	
	for (j = 6, res = (int *) arr.buffer; j != 0; res++, j--)
		printf("%d\n", *res);

	darray_delete(&arr);
}

int
main()
{
	test1();	
	return 0;
}

