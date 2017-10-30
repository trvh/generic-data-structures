#include "../src/gds_bin_tree.h"

int
main()
{
	struct gds_tree tree;
	
	tree_create(&tree);
	
	tree_insert(&tree, 12);
	tree_insert(&tree, 6);
	tree_insert(&tree, 11);
	tree_insert(&tree, 10);
	tree_insert(&tree, 7);
	tree_insert(&tree, 8);
	tree_insert(&tree, 13);
	
	tree_remove(&tree, 12);

	tree_delete(&tree);
	return 0;
}
