#include <string.h> 
#include <assert.h> 
#include "gds_darray.h"

#define COUNT 32
#define GET_CURRENT(buffer, current, size) \
			(((char *) (buffer)) + ((current) * (size)))

void
darray_create(struct gds_darray *array, size_t size)
{
	assert(array != NULL);
	assert(size > 0);

	array->buffer  = NULL;
	array->count   = 0;
	array->current = 0;
	array->size    = size;
}

void
darray_delete(struct gds_darray *array)
{
	assert(array != NULL);
	
	free(array->buffer);
}

void
darray_clear(struct gds_darray *array)
{
	assert(array != NULL);
	
	/*free old buffer*/
	free(array->buffer);
	
	array->buffer  = NULL;
	array->count   = 0;
	array->current = 0;
}

void
darray_remove(struct gds_darray *array, size_t index)
{
	void *dest, *sorc;
	size_t bytes;

	assert(array != NULL);
	assert(array->current > 0);
	assert(array->current - 1 >= index);
	
	if (array->current - 1 == index)
		array->current--;
	else {
		dest  = GET_CURRENT(array->buffer, index, array->size);
		sorc  = GET_CURRENT(array->buffer, index + 1, array->size);
		bytes = (array->current - index - 1) * array->size;
		memmove(dest, sorc, bytes);
		array->current -= index - 1;
	}
}

void
darray_add(struct gds_darray *array, void *value)
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
		   value,
		   array->size);
	array->current++;
}

void
darray_join(struct gds_darray *array, void *values, size_t count)
{
	void  *buffer;
	size_t current, new_count;
	size_t rest = array->count - array->current;
	
	assert(array  != NULL);
	assert(values != NULL);
	assert(count  > 0);
	
	if (rest < count) {
		/*increase buffer*/
		new_count  = (count - rest) + array->count;
		buffer     = realloc(array->buffer, array->size * new_count);
		
		assert(buffer != NULL);
		
		/*remember new buffer*/
		array->buffer = buffer;
		array->count  = current = new_count;	
	} else
		current = array->current + count;
	
	memcpy(GET_CURRENT(array->buffer, array->current, array->size),
		   values,
		   array->size * count);
	array->current = current;
}

void
darray_insert(struct gds_darray *array, void *value, size_t index)
{
	size_t count, bytes;
	void  *buffer, *dest, *sorc;
	
	assert(array != NULL);
	assert(value != NULL);
	assert(array->current >= index);
	
	if (array->current == array->count) {
		/*increase buffer*/
		count  = array->count + COUNT;
		buffer = realloc(array->buffer, count * array->size);
		
		assert(buffer != NULL);
	
		/*remember new buffer*/
		array->buffer = buffer;
		array->count  = count;	
	}
	
	if (array->current != index) {
		dest  = GET_CURRENT(array->buffer, index + 1, array->size);
		sorc  = GET_CURRENT(array->buffer, index, array->size);
		bytes = (array->current - index) * array->size;
		memmove(dest, sorc, bytes);
	}
	dest = GET_CURRENT(array->buffer, index, array->size);
	memcpy(dest, value, array->size);
	array->current++;
}

void *
darray_get(struct gds_darray *array)
{
	void *buffer;
	
	assert(array != NULL);
	
	buffer = array->buffer;
	
	array->buffer  = NULL;
	array->count   = 0;
	array->current = 0;
	
	return buffer;
}
