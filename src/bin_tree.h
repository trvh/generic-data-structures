#ifndef BINARY_TREE_RECURSIVE
#define BINARY_TREE_RECURSIVE

#include <stdlib.h>
#include "common_consts.h"

struct tree_gds {
	struct tnode_gds *root;
};

void tree_create(struct tree_gds *root);
void tree_delete(struct tree_gds *root);

void tree_insert(struct tree_gds *tree, int value);
void tree_remove(struct tree_gds *tree, int value);
int  tree_search(struct tree_gds *tree, int value);

void round_wide(struct tree_gds *tree);
void round_bottom(struct tree_gds *tree);
#endif
