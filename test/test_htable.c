#include <stdio.h>
#include "../src/gds_htable.h"

#define MAX 100000

size_t
hash(void *key)
{
	int *k = (int *) key;
	return (size_t) *k;
}

int
cmp(void *k1, void *k2)
{
	return *(int *) k1 - *(int *) k2;
}

void
test1()
{
	struct gds_htable t;
	int i;
	int *j;
	size_t sum;

	htable_create(&t, hash, cmp, sizeof(int), sizeof(int));
	
	for (i = 0; i < MAX; i++)
		htable_insert(&t, &i, &i);
	
	for (i = sum = 0; i < MAX; i++) {
		j = (int *) htable_search(&t, &i);
		sum += *j;
		htable_remove(&t, &i);
	}
	printf("%ld\n", sum);

	htable_delete(&t);
}

int
main()
{
	test1();	
	return 0;
}

