#include "xmemtools.h"

#include <assert.h>

void *xmalloc(size_t bytes) {

	void *temp = malloc(bytes);
	assert(temp != NULL);
	return (temp);
}

void *xcalloc(size_t nelem, size_t bytes) {

	void *temp = calloc(nelem, bytes);
	assert(temp != NULL);
	return (temp);
}

void *xrealloc(void *pntr, size_t bytes) {

	void *temp = realloc(pntr, bytes);
	assert(temp != NULL);
	return (temp);
}

void xfree(void *pntr) {

	free(pntr);
}

void *xmemdup(void *pntr, size_t bytes) {
    
    void *temp = xmalloc(bytes);
    assert(temp != NULL);
    return memmove(temp, pntr, bytes);

}
