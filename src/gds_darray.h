#ifndef GDS_DYNAMIC_ARRAY_HEADER
#define GDS_DYNAMIC_ARRAY_HEADER

#include <stdlib.h> 

struct gds_darray {
	void  *buffer;
	size_t size; /*size of element*/
	size_t count; /*common size of buffer*/
	size_t current; /*count of elements in buffer*/
};

void darray_create(struct gds_darray *array, size_t size);
void darray_delete(struct gds_darray *array);

void darray_clear(struct gds_darray *array);
void darray_remove(struct gds_darray *array, size_t index);

void darray_add(struct gds_darray *array, void *src);
void darray_join(struct gds_darray *array, void *src, size_t count);
void darray_insert(struct gds_darray *array, void *src, size_t index);

void *darray_getdata(struct gds_darray *array);
#endif
