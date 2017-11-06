#ifndef GDS_HASH_TABLE_HEADER
#define GDS_HASH_TABLE_HEADER

struct gds_table {
	struct gds_hnode **buckets;
	size_t size; /*size of buckets*/ 
};

struct gds_htable {
	size_t (*hash)(void *key);
	int (*cmp)(void *hkey, void *key);
	struct gds_table table;
	size_t size_elem; /*size of one element*/
	size_t size_key;
	size_t count; /*current number of elements*/
};

#ifdef __cplusplus
extern "C" {
#endif

void htable_create(struct gds_htable *htable,
					size_t (*hash)(void *key),
					int (*cmp)(void *hkey, void *key),
					size_t size_elem, size_t size_key);
void htable_delete(struct gds_htable *htable);

void *htable_search(struct gds_htable *htable, void *key);
void *htable_insert(struct gds_htable *htable, void *src, void *key);
void htable_remove(struct gds_htable *htable, void *key);
void htable_resize(struct gds_htable *htable, size_t size_new);

#ifdef __cplusplus
}
#endif

#endif
