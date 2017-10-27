#ifndef BINARY_TREE_RECURSIVE
#define BINARY_TREE_RECURSIVE

#include <stdlib.h>
#include "gds_consts.h"

struct gds_tree {
	struct gds_tnode *root;
};

void tree_create(struct gds_tree *tree);
void tree_delete(struct gds_tree *tree);

void tree_insert(struct gds_tree *tree, int value);
void tree_remove(struct gds_tree *tree, int value);
int  tree_search(struct gds_tree *tree, int value);

void round_wide(struct gds_tree *tree);
void round_bottom(struct gds_tree *tree);
#endif