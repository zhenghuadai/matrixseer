
#include <stdlib.h>
//#include "inttypes.h"

#include "mpeg2.h"

static void * (* malloc_hook) (unsigned size, mpeg2_alloc_t reason) = NULL;
static int (* free_hook) (void * buf) = NULL;

void * mpeg2_malloc (unsigned size, mpeg2_alloc_t reason)
{
    char * buf;

    if (malloc_hook) {
	buf = (char *) malloc_hook (size, reason);
	if (buf)
	    return buf;
    }

    if (size) {
	buf = (char *) malloc (size + 63 + sizeof (void **));
	if (buf) {
	    char * align_buf;

	    align_buf = buf + 63 + sizeof (void **);
	    align_buf -= (long)align_buf & 63;
	    *(((void **)align_buf) - 1) = buf;
	    return align_buf;
	}
    }
    return NULL;
}

void mpeg2_free (void * buf)
{
    if (free_hook && free_hook (buf))
	return;

    if (buf)
	free (*(((void **)buf) - 1));
}

void mpeg2_malloc_hooks (void * malloc (unsigned, mpeg2_alloc_t),
			 int free (void *))
{
    malloc_hook = malloc;
    free_hook = free;
}
