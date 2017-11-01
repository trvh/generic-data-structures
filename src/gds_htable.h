#ifndef GDS_HASH_TABLE_HEADER
#define GDS_HASH_TABLE_HEADER

struct gds_table {
	struct gds_hnode **array;
	size_t size; /*size of array*/ 
};

struct gds_htable {
	size_t (*hash)(void *key);
	int (*cmp)(void *hkey, void *key);
	struct gds_table table;
	size_t size; /*size of one element*/
	size_t count; /*current number of elements*/
};

#ifdef __cplusplus
extern "C" {
#endif

void htable_create(struct gds_htable *htable,
					size_t (*hash)(void *key),
					int (*cmp)(void *hkey, void *key),
					size_t size);
void htable_delete(struct gds_htable *htable);

#ifdef __cplusplus
}
#endif

#endif
