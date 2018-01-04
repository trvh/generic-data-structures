#include <string.h> 
#include <assert.h> 

#include "gds_darray.h"
#include "gds_common.h"

#define COUNT 32 /*starting number of elements of array*/

void
darray_create(struct gds_darray *array, size_t size)
{
	void *buffer;

	assert(array != NULL);
	assert(size > 0);

	buffer = malloc(COUNT * size);
	assert(buffer != NULL);
	
	array->buffer  = buffer;
	array->size    = size;
	array->count   = COUNT;
	array->current = 0;
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
	void *buffer;

	assert(array != NULL);
	
	/*free old buffer*/
	free(array->buffer);
	buffer = malloc(COUNT * array->size);
	assert(buffer != NULL);
	
	array->buffer  = buffer;
	array->count   = COUNT;
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
		size_t size = array->size;
		void *buffer = array->buffer;

		dst   = TO_NEXT(buffer, index * size);
		src   = TO_NEXT(buffer, (index + 1) * size);
		bytes = (array->current - index - 1) * size;
		memmove(dst, src, bytes);
	}
	array->current--;
}

void
darray_add(struct gds_darray *array, void *src)
{
	void  *buffer, *dst;
	size_t count, size, current; 
	
	assert(array != NULL);
	assert(src != NULL);

	size    = array->size;
	current = array->current;
	count   = array->count;
	if (current == count) {
		/*increase buffer*/
		count <<= 1;
		buffer = realloc(array->buffer, count * size);
		assert(buffer != NULL);
	
		array->buffer = buffer;
		array->count  = count;	
	}	
	dst = TO_NEXT(array->buffer, current * size);
	memcpy(dst, src, size);
	array->current++;
}

void
darray_join(struct gds_darray *array, void *src, size_t count)
{
	void  *dst;
	size_t rest, size, old_current, old_count, new_current;
	
	assert(array != NULL);
	assert(src != NULL);
	assert(count > 0);
	
	size = array->size;
	old_current = array->current;
	old_count = array->count;
	rest = old_count - old_current;
	if (rest < count) {
		/*increase buffer*/
		void *buffer;
		size_t new_count = old_current + count;

		buffer = realloc(array->buffer, size * new_count);	
		assert(buffer != NULL);
		
		array->buffer = buffer;
		array->count  = new_current = new_count;	
	} else
		new_current = old_current + count;
	
	dst = TO_NEXT(array->buffer, old_current * size);
	memcpy(dst, src, size * count);
	array->current = new_current;
}

void
darray_insert(struct gds_darray *array, void *src, size_t index)
{
	void *dst, *buffer;
	size_t count, current, size;
	
	assert(array != NULL);
	assert(src != NULL);
	assert(array->current >= index);
	
	size    = array->size;
	current = array->current;
	count   = array->count;
	if (current == count) {
		/*increase buffer*/
		count <<= 1;
		buffer = realloc(array->buffer, count * size);
		assert(buffer != NULL);
	
		array->buffer = buffer;
		array->count  = count;	
	}
	
	buffer = array->buffer;
	if (current != index) {
		/*to shift elements in right*/
		void  *src2;
		size_t bytes;
		
		dst   = TO_NEXT(buffer, (index + 1) * size);
		src2  = TO_NEXT(buffer, index * size);
		bytes = (current - index) * size;
		memmove(dst, src2, bytes);
	}
	dst = TO_NEXT(buffer, index * size);
	memcpy(dst, src, size);
	array->current++;
}

void *
darray_getdata(struct gds_darray *array)
{
	void *old_buffer, *new_buffer;
	
	assert(array != NULL);
	
	old_buffer = array->buffer;
	
	new_buffer = malloc(COUNT * array->size);
	assert(new_buffer != NULL);
	
	array->buffer  = new_buffer;
	array->count   = COUNT;
	array->current = 0;
	
	return old_buffer;
}
