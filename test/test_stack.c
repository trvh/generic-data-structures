#include <stdio.h>
#include "../src/gds_stack.h"

#define MAX 1000000

void
test1()
{
	struct gds_stack stack;
	int i;

	stack_create(&stack, sizeof(int));
	i = 1;
	stack_push(&stack, &i);
	i = 2;
	stack_push(&stack, &i);
	i = 3;
	stack_push(&stack, &i);

	i = 0;
	while (stack.count != 0) {
		stack_pop(&stack, &i);
		printf("%d\n", i);
	}

	stack_delete(&stack);
}

void
test2()
{
	struct gds_stack stack;
	size_t sum;
	int i, j;

	stack_create(&stack, sizeof(int));
	
	for (i = 0, j = 1; i < MAX; i++)
		stack_push(&stack, &j);
	
	sum = j = 0;
	while (stack.count) {
		stack_pop(&stack, &j);
		sum += j;
	}
	printf("%ld\n", sum);

	stack_delete(&stack);
}

void
test4()
{
	struct gds_stack stack;
	int i, j;

	stack_create(&stack, sizeof(int));
	
	for (i = 0, j = 1; i < MAX; i++)
		stack_push(&stack, &j);
	
	while (--stack.count) {
		stack_peek(&stack, &j);
	}

	stack_delete(&stack);
}

void
test5()
{
	struct gds_stack stack;
	int i, j;

	stack_create(&stack, sizeof(int));
	
	for (i = 0, j = 1; i < MAX; i++)
		stack_push(&stack, &j);
	
	stack_clear(&stack);
	stack_delete(&stack);
}

int
main()
{
	test1();
	test2();	
	test4();	
	test5();
	return 0;
}

