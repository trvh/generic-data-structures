
#include <stdio.h>
#include "../src/gds_queue.h"

#define MAX 1000000

void
test1()
{
	struct gds_queue queue;
	int i;
	size_t sum;

	queue_create(&queue, sizeof(int));
	
	
	for (i = 0; i < MAX; i++)
		queue_add(&queue, &i);

	sum = 0;
	while (queue.count != 0) {
		queue_pop(&queue, &i);
		sum += i;
	}
	printf("%ld\n", sum);

	queue_delete(&queue);
}

void
test2()
{
	struct gds_queue queue;
	int i, j;
	size_t sum;

	queue_create(&queue, sizeof(int));
	
	
	for (j = 1, i = 0; i < MAX; i++)
		queue_add(&queue, &j);

	sum = 0;
	while (queue.count != 0) {
		queue_front(&queue, &i);
		sum += i;
		queue.count--;
	}
	printf("%ld\n", sum);
	

	queue_clear(&queue);
	queue_delete(&queue);
}

void
test3()
{
	struct gds_queue queue;
	int i;
	size_t sum;

	queue_create(&queue, sizeof(int));
	
	
	for (i = 0; i < MAX; i++)
		queue_add(&queue, &i);

	sum = 0;
	while (queue.count != 0) {
		queue_back(&queue, &i);
		sum += i;
		queue_pop(&queue, &i);
	}
	printf("%ld\n", sum);
	

	queue_clear(&queue);
	queue_delete(&queue);
}

int
main()
{
	test1();	
	test2();	
	test3();	
	return 0;
}

