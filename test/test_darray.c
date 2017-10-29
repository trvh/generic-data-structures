#include <stdio.h>
#include "../src/gds_darray.h"

#define MAX 100000

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

	for (j = arr.current, res = (int *) arr.buffer; j != 0; res++, j--)
		printf("%d\n", *res);
	printf("-----\n");
	
	darray_remove(&arr, 3);
	
	for (j = arr.current, res = (int *) arr.buffer; j != 0; res++, j--)
		printf("%d\n", *res);

	darray_remove(&arr, 0);
	darray_remove(&arr, 3);
	darray_delete(&arr);
}

void
test2()
{
	struct gds_darray arr;
	int i, j;
	int *res;
	size_t sum;

	darray_create(&arr, sizeof(int));

	for (i = 0; i < MAX; i++)
		darray_insert(&arr, &i, 0);
	
	sum = 0;
	for (j = arr.current, res = (int *) arr.buffer; j != 0; res++, j--)
		sum += *res;
	
	printf("\n------test2-----");
	printf("%ld\n", sum);
	
	darray_delete(&arr);
}

int
main()
{
	test1();
	test2();
	return 0;
}

