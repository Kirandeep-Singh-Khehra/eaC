#ifndef _SIZED_ARR_
#define _SIZED_ARR_

#include <stdlib.h>
#include <string.h>

// DECLARATION //
void * sized_arr_init(size_t size, int count);
unsigned int sized_arr_len(void *arr);
void sized_arr_free(void *arr);

// IMPLEMENTATION //
void * sized_arr_init(size_t size, int count) {
    void * ptr;
    ptr = malloc(sizeof(unsigned int) + (count * size));

    memcpy(ptr, &count, sizeof(unsigned int));

    return ((unsigned int *)ptr) + 1;
}

unsigned int sized_arr_len(void *arr) {
    return ((unsigned int *)arr)[-1];
}

void sized_arr_free(void *arr) {
    free(((unsigned int *)arr) - 1);
}

#endif
