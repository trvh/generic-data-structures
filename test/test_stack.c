#include <stdio.h>
#include "../src/gds_stack.h"

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

int
main()
{
	test1();	
	return 0;
}

