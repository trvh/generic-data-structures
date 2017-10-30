#ifndef GDS_COMMON_MACROS_HEADER
#define GDS_COMMON_MACROS_HEADER

#define TO_NEXT(start, bytes) ((void *) (((char *) (start)) + (bytes)))
#define TO_PREV(start, bytes) ((void *) (((char *) (start)) - (bytes)))

#endif
