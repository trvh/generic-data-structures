#ifndef DYNAMIC_ARRAY_HEADER
#define DYNAMIC_ARRAY_HEADER

#include <stdlib.h> 

struct darray_gds {
	void  *buffer;
	size_t size; /*size of element*/
	size_t count; /*common size of buffer*/
	size_t current; /*count of elements in buffer*/
};

void darray_create(struct darray_gds *array, size_t size);
void darray_delete(struct darray_gds *array);

void darray_clear(struct darray_gds *array);
void darray_add(struct darray_gds *array, void *value);
void darray_join(struct darray_gds *array, void *values, size_t count);
void *darray_get(struct darray_gds *array);
#endif
