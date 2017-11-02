#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "gds_htable.h"
#include "gds_common.h"


#define HTABLE_SIZE 256
#define KEY(node) ((void *) (((char *) (node)) + (sizeof(struct gds_hnode))))

struct gds_hnode {
	struct gds_hnode *next;
};

static void set_arr_null(struct gds_hnode **array, size_t size);

void
htable_create(struct gds_htable *htable,
					size_t (*hash)(void *key),
					int (*cmp)(void *hkey, void *key),
					size_t size_elem, size_t size_key)
{
	struct gds_hnode **array;

	assert(htable != NULL);
	assert(hash != NULL);
	assert(cmp != NULL);
	assert(size_elem > 0);
	assert(size_key > 0);
	
	/*create array of nodes*/
	array = (struct gds_hnode **)
				malloc(sizeof(struct gds_hnode *) * HTABLE_SIZE);
	assert(array != NULL);
	set_arr_null(array, HTABLE_SIZE);

	htable->table.array = array;
	htable->table.size  = HTABLE_SIZE;
	htable->hash = hash;
	htable->cmp  = cmp;
	htable->size_elem = size_elem;
	htable->size_key  = size_key;
	htable->count     = 0;
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

void *
htable_search(struct gds_htable *htable, void *key)
{

	struct gds_hnode **array;
	struct gds_hnode  *node;
	size_t (*hash)(void *key);
	int (*cmp)(void *hkey, void *key);
	size_t size, index;

	assert(htable != NULL);
	assert(key != NULL);
	
	hash  = htable->hash;
	cmp   = htable->cmp;
	array = htable->table.array;
	size  = htable->table.size;
	index = (*hash)(key) % size;
	for (node = array[index]; node != NULL; node = node->next)
		if ((*cmp)(KEY(node), key) == 0)
			break;
	return node;
}

void *
htable_insert(struct gds_htable *htable, void *src, void *key)
{
	struct gds_hnode **array;
	struct gds_hnode  *node;
	size_t (*hash)(void *key);
	int (*cmp)(void *hkey, void *key);
	size_t size, index, size_elem, size_key;
	void   *dst;

	assert(htable != NULL);
	assert(src != NULL);
	assert(key != NULL);
	
	hash  = htable->hash;
	cmp   = htable->cmp;
	array = htable->table.array;
	size  = htable->table.size;
	index = (*hash)(key) % size;	
	for (node = array[index]; node != NULL; node = node->next)
		if ((*cmp)(KEY(node), key) == 0)
			break;
	
	size_key  = htable->size_key;
	size_elem = htable->size_elem;
	if (node != NULL) {
		/*element not was find, create new node*/
		size  = sizeof(struct gds_hnode); /*size of node*/
		size += size_key + size_elem; /*general size*/
		node  = (struct gds_hnode *) malloc(size);
		assert(node != NULL);
		
		/*copy value and key in node*/
		dst = TO_NEXT(node, sizeof(struct gds_hnode));
		memcpy(dst, key, size_key);
		dst = TO_NEXT(dst, size_key);
		memcpy(dst, src, size_elem);
		
		/*insert new node in table*/
		node->next   = array[index];
		array[index] = node;
		htable->count++;
	} else {
		/*element was find, update value*/
		dst = KEY(node);
		dst = TO_NEXT(dst, size_key);
		memcpy(dst, src, size_elem);
	}
	return node;
}

void
htable_remove(struct gds_htable *htable, void *key)
{
	struct gds_hnode **array;
	struct gds_hnode  *node, *prev, *head;
	size_t (*hash)(void *key);
	int (*cmp)(void *hkey, void *key);
	size_t size, index;

	assert(htable != NULL);
	assert(key != NULL);
	
	hash  = htable->hash;
	cmp   = htable->cmp;
	array = htable->table.array;
	size  = htable->table.size;
	index = (*hash)(key) % size;
	head  = array[index];
	for (node = head; node != NULL; node = node->next) {
		if ((*cmp)(KEY(node), key) == 0) {
			/*element was find, remove it*/
			if (node == head) /*is it head node of list?*/
				array[index] = node->next;
			else
				prev->next = node->next;
			/*remove node*/
			free(node);
			htable->count--;
			break;
		}
		prev = node; /*remember previous node*/
	}
}
