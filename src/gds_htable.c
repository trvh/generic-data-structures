#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "gds_htable.h"
#include "gds_common.h"


#define HTABLE_SIZE 256

#define KEY(node) ((void *) (((char *) (node)) + (sizeof(struct gds_hnode))))

#define VALUE(node, size_key) ((void *) (((char *) KEY(node)) + (size_key)))

struct gds_hnode {
	struct gds_hnode *next;
};

static void set_arr_null(struct gds_hnode **buckets, size_t size);

void
htable_create(struct gds_htable *htable,
					size_t (*hash)(void *key),
					int (*cmp)(void *hkey, void *key),
					size_t size_elem, size_t size_key)
{
	struct gds_hnode **buckets;

	assert(htable != NULL);
	assert(hash != NULL);
	assert(cmp != NULL);
	assert(size_elem > 0);
	assert(size_key > 0);
	
	/*create buckets of nodes*/
	buckets = (struct gds_hnode **)
				malloc(sizeof(struct gds_hnode *) * HTABLE_SIZE);
	assert(buckets != NULL);
	set_arr_null(buckets, HTABLE_SIZE);

	htable->table.buckets = buckets;
	htable->table.size    = HTABLE_SIZE;
	htable->hash		  = hash;
	htable->cmp			  = cmp;
	htable->size_elem	  = size_elem;
	htable->size_key	  = size_key;
	htable->count		  = 0;
}

void
htable_delete(struct gds_htable *htable)
{
	struct gds_hnode **buckets, **end;
	struct gds_hnode  *node, *next;
	size_t			   size;

	assert(htable != NULL);
	
	buckets = htable->table.buckets;
	size    = htable->table.size;
	for (end = buckets + size; buckets < end; buckets++)
		for (node = *buckets; node != NULL; node = next) {
			next = node->next;
			free(node);
		}
	free(htable->table.buckets);
}

void
set_arr_null(struct gds_hnode **buckets, size_t size)
{
	struct gds_hnode **end;

	for (end = buckets + size; buckets < end; buckets++)
		*buckets = NULL;
}

void *
htable_search(struct gds_htable *htable, void *key)
{

	struct gds_hnode **buckets;
	struct gds_hnode  *node;
	size_t (*hash)(void *key);
	int (*cmp)(void *hkey, void *key);
	size_t size, index;

	assert(htable != NULL);
	assert(key != NULL);
	
	hash    = htable->hash;
	cmp     = htable->cmp;
	buckets = htable->table.buckets;
	size    = htable->table.size;
	index   = (*hash)(key) % size;
	for (node = buckets[index]; node != NULL; node = node->next)
		if ((*cmp)(KEY(node), key) == 0)
			return VALUE(node, htable->size_key);
	return NULL; /*element not was find*/
}

void *
htable_insert(struct gds_htable *htable, void *src, void *key)
{
	struct gds_hnode **buckets;
	struct gds_hnode  *node;
	size_t (*hash)(void *key);
	int (*cmp)(void *hkey, void *key);
	size_t size, index, size_elem, size_key;
	void   *dst;

	assert(htable != NULL);
	assert(src != NULL);
	assert(key != NULL);
	
	hash    = htable->hash;
	cmp     = htable->cmp;
	buckets = htable->table.buckets;
	size    = htable->table.size;
	index   = (*hash)(key) % size;	
	for (node = buckets[index]; node != NULL; node = node->next)
		if ((*cmp)(KEY(node), key) == 0)
			break;
	
	size_key  = htable->size_key;
	size_elem = htable->size_elem;
	if (node == NULL) {
		/*element not was find, create new node*/
		size  = sizeof(struct gds_hnode) + size_key + size_elem;
		node  = (struct gds_hnode *) malloc(size);
		assert(node != NULL);
		
		/*copy value and key in node*/
		dst = KEY(node);
		memcpy(dst, key, size_key);
		dst = VALUE(node, size_key);
		memcpy(dst, src, size_elem);
		
		/*insert new node in table*/
		node->next     = buckets[index];
		buckets[index] = node;
		htable->count++;
		
		/*to double of size*/
		if (htable->count == htable->table.size)
			htable_resize(htable, htable->table.size * 2);
	} else {
		/*element was find, update value*/
		dst = VALUE(node, size_key);
		memcpy(dst, src, size_elem);
	}
	return VALUE(node, size_key);
}

void
htable_remove(struct gds_htable *htable, void *key)
{
	struct gds_hnode **buckets;
	struct gds_hnode  *node, *prev, *head;
	size_t (*hash)(void *key);
	int (*cmp)(void *hkey, void *key);
	size_t size, index;

	assert(htable != NULL);
	assert(key != NULL);
	
	hash    = htable->hash;
	cmp     = htable->cmp;
	buckets = htable->table.buckets;
	size    = htable->table.size;
	index   = (*hash)(key) % size;
	head    = buckets[index];
	for (node = head; node != NULL; node = node->next) {
		if ((*cmp)(KEY(node), key) == 0) {
			/*element was find, remove it*/
			if (node == head) /*is it head node of list?*/
				buckets[index] = node->next;
			else
				prev->next = node->next;
			free(node);
			htable->count--;
			break;
		}
		prev = node; /*remember previous node*/
	}
}

void
htable_resize(struct gds_htable *htable, size_t size_new)
{
	struct gds_hnode **buckets, **buckets_new, **end;
	struct gds_hnode  *node, *next;
	size_t (*hash)(void *key);
	size_t size;

	assert(htable != NULL);
	assert(size_new > 0);
		
	buckets_new = 
		(struct gds_hnode **) malloc(sizeof(struct gds_hnode *) * size_new);
	assert(buckets_new != NULL);
	set_arr_null(buckets_new, size_new);
	
	hash    = htable->hash;
	buckets = htable->table.buckets;
	size    = htable->table.size;
	/*copy all nodes in new buckets*/
	for (end = buckets + size; buckets < end; buckets++)
		for (node = *buckets; node != NULL; node = next) {
			size_t index = (*hash)(KEY(node)) % size_new;
			next = node->next; /*remember next node*/	
			node->next = buckets_new[index]; /*remove old reference*/
			buckets_new[index] = node;
		}
	free(htable->table.buckets);
	htable->table.buckets = buckets_new;
	htable->table.size    = size_new;
}
