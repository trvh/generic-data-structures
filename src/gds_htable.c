#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "gds_htable.h"


#define HTABLE_SIZE 256
#define SHIFT		8

struct gds_hnode {
	struct gds_hnode *next;
};

static void set_arr_null(struct gds_hnode **array, size_t size);

void
htable_create(struct gds_htable *htable,
					size_t (*hash)(void *key),
					int (*cmp)(void *hkey, void *key),
					size_t size)
{
	assert(htable != NULL);
	assert(hash != NULL);
	assert(cmp != NULL);
	assert(size > 0);
	
	/*create array of nodes*/
	array = (struct gds_hnode **) malloc(sizeof(struct gds_hnode *) << SHIFT);
	assert(array != NULL);
	set_arr_null(array, HTABLE_SIZE);

	htable->table.array = array;
	htable->table.size  = HTABLE_SIZE;
	htable->hash  = hash;
	htable->cmp   = cmp;
	htable->size  = size;
}

void
htable_delete(struct gds_htable *htable)
{
	struct gds_hnode **array, **end;
	struct gds_hnode  *node, *next;
	size_t			   size;

	assert(htable != NULL);
	
	array = htable->table.array;
	size  = htable->table.size;
	for (end = array + size; array < end; array++)
		for (node = *array; node != NULL; node = next) {
			next = node->next;
			free(node);
		}
	free(htable->table.array);
}

void
set_arr_null(struct gds_hnode **array, size_t size)
{
	struct gds_hnode **end;

	for (end = array + size; array < end; array++)
		*array = NULL;
}

int
delete_var(char *name)
{
	;
}

void *
htable_search(struct gds_htable *htable, void *key)
{

	struct gds_hnode **array;
	struct gds_hnode  *node, *next;
	size_t (*hash)(void *key);
	int (*cmp)(void *hkey, void *key);

	assert(htable != NULL);
	assert(key != NULL);
	
	hash  = htable->hash;
	cmp   = htable->cmp;
	array = htable->table.array;
	index = hash(key) % size;
	for (node = array[index]; node != NULL; node = node->next)
		if ((*cmp)(node->key, key) == 0)
			break;
	return node;
}

void *
htable_insert(struct gds_htable *htable, void *src, void *key)
{

	struct gds_hnode **array;
	struct gds_hnode  *node, *next;
	size_t (*hash)(void *key);
	int (*cmp)(void *hkey, void *key);

	assert(htable != NULL);
	assert(src != NULL);
	assert(key != NULL);
	
	hash  = htable->hash;
	array = htable->table.array;
	index = hash(key) % size;
	
	return node;
}
