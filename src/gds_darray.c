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
	void *dst, *src;
	size_t bytes;

	assert(array != NULL);
	assert(array->current > 0);
	assert(array->current - 1 >= index);
	
	if (array->current - 1 != index) {
		/*to shift elements in left*/
		dst   = GET_CURRENT(array->buffer, index, array->size);
		src   = GET_CURRENT(array->buffer, index + 1, array->size);
		bytes = (array->current - index - 1) * array->size;
		memmove(dst, src, bytes);
	}
	array->current--;
}

void
darray_add(struct gds_darray *array, void *src)
{
	size_t count;
	void  *buffer, *dst;
	
	assert(array != NULL);
	assert(src != NULL);
	
	if (array->current == array->count) {
		/*increase buffer*/
		count  = array->count + COUNT;
		buffer = realloc(array->buffer, count * array->size);
		
		assert(buffer != NULL);
	
		/*remember new buffer*/
		array->buffer = buffer;
		array->count  = count;	
	}	
	dst = GET_CURRENT(array->buffer, array->current, array->size);
	memcpy(dst, src, array->size);
	array->current++;
}

void
darray_join(struct gds_darray *array, void *src, size_t count)
{
	void  *buffer, *dst;
	size_t current, new_count;
	size_t rest;
	
	assert(array  != NULL);
	assert(src != NULL);
	assert(count  > 0);
	
	rest = array->count - array->current;
	if (rest < count) {
		/*increase buffer*/
		new_count = (count - rest) + array->count;
		buffer    = realloc(array->buffer, array->size * new_count);
		
		assert(buffer != NULL);
		
		/*remember new buffer*/
		array->buffer = buffer;
		array->count  = current = new_count;	
	} else
		current = array->current + count;
	
	dst = GET_CURRENT(array->buffer, array->current, array->size);
	memcpy(dst, src, array->size * count);
	array->current = current;
}

void
darray_insert(struct gds_darray *array, void *src, size_t index)
{
	void *dst;
	
	assert(array != NULL);
	assert(src != NULL);
	assert(array->current >= index);
	
	if (array->current == array->count) {
		/*increase buffer*/
		void  *buffer;
		size_t count;

		count  = array->count + COUNT;
		buffer = realloc(array->buffer, count * array->size);
		
		assert(buffer != NULL);
	
		/*remember new buffer*/
		array->buffer = buffer;
		array->count  = count;	
	}
	
	if (array->current != index) {
		/*to shift elements in right*/
		void  *src2;
		size_t bytes;
		
		dst   = GET_CURRENT(array->buffer, index + 1, array->size);
		src2  = GET_CURRENT(array->buffer, index, array->size);
		bytes = (array->current - index) * array->size;
		memmove(dst, src2, bytes);
	}
	dst = GET_CURRENT(array->buffer, index, array->size);
	memcpy(dst, src, array->size);
	array->current++;
}

void *
darray_getdata(struct gds_darray *array)
{
	void *buffer;
	
	assert(array != NULL);
	
	buffer = array->buffer;
	
	array->buffer  = NULL;
	array->count   = 0;
	array->current = 0;
	
	return buffer;
}
