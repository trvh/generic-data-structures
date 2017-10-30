#include <stdio.h> 
#include <assert.h> 

#include "gds_bin_tree.h"

enum {
	IS_EMPTY,
	IS_FINDED,
};

struct gds_tnode {
	struct gds_tnode *left;
	struct gds_tnode *right;
	int				  value;
	size_t			  count;
};

struct tnodes {
	struct gds_tnode *prev;
	struct gds_tnode *curr;
};

static void  free_node(struct gds_tnode *node);

static struct gds_tnode *insert_node(struct gds_tnode *node, int value);
static int found_node(struct gds_tnode *node, int value);

static int get_remove_node(struct gds_tree *tree,
							int value,
							struct tnodes *nodes);

static int found_remove_node(struct gds_tnode *prev, 
							struct gds_tnode *curr,
							int value,
							struct tnodes *nodes);

static struct gds_tnode *found_max_left_node(struct gds_tnode *prev,
											struct gds_tnode *curr);

static void tree_print(struct gds_tnode *node);

void
tree_create(struct gds_tree *tree)
{
	assert(tree != NULL);

	tree->root = NULL;
}

void
tree_delete(struct gds_tree *tree)
{
	assert(tree != NULL);

	free_node(tree->root);
	free(tree->root);
}

void
free_node(struct gds_tnode *node)
{
	if (node != NULL) {
		free_node(node->left);
		free(node->left);
		
		free_node(node->right);
		free(node->right);
	}
}

void
tree_insert(struct gds_tree *tree, int value)
{
	assert(tree != NULL);

	struct gds_tnode *root = tree->root;
	if (root == NULL) {
		struct gds_tnode *node =
			(struct gds_tnode *) malloc(sizeof(struct gds_tnode));
		
		node->value = value;
		node->count = 1;
		node->left  = node->right = NULL;
		tree->root  = node;
	} else {
		if (root->value < value)
			root->right = insert_node(root->right, value);
		else if (root->value > value)
			root->left  = insert_node(root->left, value);
		else
			root->count++;
	}
}

struct gds_tnode *
insert_node(struct gds_tnode *node, int value)
{
	if (node == NULL) { 
		node = (struct gds_tnode *) malloc(sizeof(struct gds_tnode)); 
		node->value  = value;
		node->count  = 1;
		node->left   = node->right = NULL;
	} else if (node->value < value) 
		node->right = insert_node(node->right, value);
	else if (node->value > value)
		node->left = insert_node(node->left, value);
	else 
		node->count++;
	
	return node;
}

int
tree_search(struct gds_tree *tree, int value)
{
	assert(tree != NULL);

	return found_node(tree->root, value);
}

int
found_node(struct gds_tnode *node, int value)
{
	if (node == NULL)
		return IS_EMPTY;
	else if (node->value < value)
		return found_node(node->right, value);
	else if (node->value > value)
		return found_node(node->left, value);
	else
		return IS_FINDED;
}

void
tree_remove(struct gds_tree *tree, int value)
{
	assert(tree != NULL);

	if (tree->root != NULL) { /*tree is not empty*/
		struct tnodes  nodes;
		struct gds_tnode  *curr, **prev;

		if (get_remove_node(tree, value, &nodes) == IS_EMPTY)
			/*a remove node is root or tree have not node with value*/
			return ;

		curr = nodes.curr; /*remember a tree_remove node*/
		if (nodes.prev->right == curr) /*left or right node?*/
			prev = &nodes.prev->right;
		else
			prev = &nodes.prev->left;

		if (curr->left == NULL && curr->right == NULL) {
			*prev = NULL;
		} else if (curr->left != NULL && curr->right == NULL) {
			*prev = curr->left;
		} else if (curr->left == NULL && curr->right != NULL) {
			*prev = curr->right;
		} else {
			struct gds_tnode *max_left = found_max_left_node(curr, curr->left);
			max_left->right = curr->right; /*save old references*/
			max_left->left  = curr->left;
			*prev = max_left;/*replace a remove node with found one*/
		}
		free(curr); /*remove node*/
	}
}

struct gds_tnode *
found_max_left_node(struct gds_tnode *prev, struct gds_tnode *curr)
{
	if (curr->right == NULL) {
		if (prev->left == curr) /*left or right node?*/
			prev->left  = curr->left; /*save reference on subtree*/
		else
			prev->right = curr->left;

		return curr;
	} else
		return found_max_left_node(curr, curr->right);
}

int
get_remove_node(struct gds_tree *tree, int value, struct tnodes *nodes)
{
	assert(tree != NULL);

	struct gds_tnode *root = tree->root;
	
	if (root->value == value) { /*a tree_remove node is root*/
		if (root->left == NULL && root->right == NULL) {
			tree->root = NULL; /*it is last node*/
		} else if (root->left != NULL && root->right == NULL) {
			tree->root = root->left;
		} else if (root->left == NULL && root->right != NULL) {
			tree->root = root->right;
		} else {
			struct gds_tnode *max_left = found_max_left_node(root, root->left);
			max_left->right = root->right; /*save old references*/
			max_left->left  = root->left;
			tree->root      = max_left; /*remember node*/
		}
		free(root); /*remove node*/

		return IS_EMPTY; /*node was removed - exit*/
	} else if (root->value < value) /*search node in tree*/
		return found_remove_node(root, root->right, value, nodes);
	else
		return found_remove_node(root, root->left, value, nodes);
}

int
found_remove_node(struct gds_tnode *prev,
				struct gds_tnode *curr,
				int value,
				struct tnodes *nodes)
{
	if (curr == NULL)
		return IS_EMPTY;
	else if (curr->value == value) {
		nodes->prev = prev; /*remember nodes*/
		nodes->curr = curr;
		return IS_FINDED;
	} else if (curr->value < value)
		return found_remove_node(curr, curr->right, value, nodes);
	else
		return found_remove_node(curr, curr->left, value, nodes);
}

void
round_wide(struct gds_tree *tree)
{
	;
}

void
round_bottom(struct gds_tree *tree)
{
	tree_print(tree->root);
}

void
tree_print(struct gds_tnode *node)
{
	if (node != NULL) {
		tree_print(node->left);
	
		printf("%d\n", node->value);

		tree_print(node->right);
	}
}
