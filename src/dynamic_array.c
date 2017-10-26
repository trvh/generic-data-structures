#include <string.h> 
#include <assert.h> 
#include "dynamic_array.h"

#define COUNT 32
#define GET_CURRENT(buffer, current, size) \
			(((char *) buffer) + (current * size))

void
darray_create(struct darray_gds *array, size_t size)
{
	assert(array != NULL);
	assert(size > 0);

	array->buffer  = NULL;
	array->count   = 0;
	array->current = 0;
	array->size    = size;
}

void
darray_delete(struct darray_gds *array)
{
	assert(array != NULL);
	
	free(array->buffer);
}

void
darray_clear(struct darray_gds *array)
{
	assert(array != NULL);
	
	/*free old buffer*/
	free(array->buffer);
	array->buffer  = NULL;
	array->count   = 0;
	array->current = 0;
}

void
darray_add(struct darray_gds *array, void *value)
{
	size_t count;
	void  *buffer;
	
	assert(array != NULL);
	assert(value != NULL);
	
	if (array->current == array->count) {
		/*increase buffer*/
		count  = array->count + COUNT;
		buffer = realloc(array->buffer, count * array->size);
		
		assert(buffer != NULL);
	
		/*remember new buffer*/
		array->buffer = buffer;
		array->count  = count;	
	}
	
	memcpy(GET_CURRENT(array->buffer, array->current, array->size),
		value, array->size);
	array->current++;
}

void
darray_join(struct darray_gds *array, void *values, size_t count)
{
	void  *buffer;
	size_t current, new_count;
	size_t rest = array->count - array->current;
	
	assert(array != NULL);
	assert(values != NULL);
	assert(count > 0);
	
	if (rest < count) {
		/*increase buffer*/
		new_count  = (count - rest) + array->count;
		buffer     = realloc(array->buffer, array->size * new_count);
		/*remember new buffer*/
		array->buffer = buffer;
		array->count  = current = new_count;	
	} else
		current = array->current + count;
	
	memcpy(GET_CURRENT(array->buffer, array->current, array->size),
		values, array->size * count);
	array->current = current;
}

void *
darray_get(struct darray_gds *array)
{
	void *buffer;
	
	assert(array != NULL);
	
	buffer = array->buffer;
	array->buffer  = NULL;
	array->count   = 0;
	array->current = 0;
	
	return buffer;
}
