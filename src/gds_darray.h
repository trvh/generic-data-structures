#ifndef DYNAMIC_ARRAY_HEADER
#define DYNAMIC_ARRAY_HEADER

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
void darray_add(struct gds_darray *array, void *value);
void darray_join(struct gds_darray *array, void *values, size_t count);
void *darray_get(struct gds_darray *array);
#endif
