#ifndef _PROP_PTR_
#define _PROP_PTR_

#include <stdlib.h>
#include <string.h>

// DECLARATION //
void *prop_ptr_init(size_t prop_size, size_t block_size);
size_t prop_ptr_get_prop_size(void *block);
void *prop_ptr_get_prop(void *block);
void prop_ptr_set_prop(void *block, void *prop, size_t prop_size);

// IMPLEMENTATION //
void *prop_ptr_init(size_t prop_size, size_t block_size) {
  if (prop_size == 0 || block_size == 0) {
    return NULL;
  }
  struct block_t {
    char prop[prop_size];
    size_t prop_size;
    char block[block_size];
  } __attribute__((packed)) * ptr;
  ptr = (struct block_t *)malloc(sizeof(struct block_t));
  ptr->prop_size = prop_size;
  return &ptr->block;
}

size_t prop_ptr_get_prop_size(void *block) {
  if (!block)
    return 0;
  return ((size_t *)block)[-1];
}

void *prop_ptr_get_prop(void *block) {
  if (!block)
    return NULL;

  size_t prop_size = ((size_t *)block)[-1];
  struct block_head_t {
    char prop[prop_size];
    size_t prop_size;
  } __attribute__((packed)) * ptr;

  ptr = (struct block_head_t *)&((char *)block)[-(sizeof(struct block_head_t))];

  return ptr;
}

void prop_ptr_set_prop(void *block, void *prop, size_t prop_size) {
  if (block && prop && prop_size > 0) {
    memcpy(prop_ptr_get_prop(block), prop, prop_size);
  }
}

void prop_ptr_free(void *block) {
  if (block) {
    free(prop_ptr_get_prop(block));
  }
}

#endif
