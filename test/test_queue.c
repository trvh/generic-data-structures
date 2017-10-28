
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
		queue_enqueue(&queue, &i);

	sum = 0;
	while (queue.count != 0) {
		queue_dequeue(&queue, &i);
		sum += i;
	}
	printf("%ld\n", sum);

	queue_delete(&queue);
}

void
test2()
{
	struct gds_queue queue;
	int i;
	size_t sum;

	queue_create(&queue, sizeof(int));
	
	
	for (i = 0; i < MAX; i++)
		queue_enqueue(&queue, &i);

	sum = 0;
	while (--queue.count != 0) {
		queue_peek(&queue, &i);
		sum += i;
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
	return 0;
}

