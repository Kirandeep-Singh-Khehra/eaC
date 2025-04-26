#ifndef _SIZED_ARR_
#define _SIZED_ARR_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

// Size of sized array must always be of integral multiple of SIZED_ARR_BLOCK_SIZE
#ifndef SIZED_ARR_BLOCK_SIZE
#define SIZED_ARR_BLOCK_SIZE 16 // Default block size (can be overridden)
#endif

// --- Forward declarations ---
static void sized_arr_recalculate_and_resize(void **arr); // Made static

// Returns the pointer to the start of the metadata block
static inline void* sized_arr_mem_addr(void *arr) {
    // Metadata is [length, capacity, element_size], data starts after
    return ((unsigned int *)arr) - 3;
}

// Gets the current number of elements
static inline unsigned int sized_arr_len(const void *arr) {
    if (arr == NULL) {
        fprintf(stderr, "ERROR: sized_arr_len called with NULL array\n");
        return 0;
    }
    return ((unsigned int *)arr)[-3];
}

// Sets the current number of elements
static inline void sized_arr_set_len(void *arr, unsigned int len) {
    if (arr == NULL) {
        fprintf(stderr, "ERROR: sized_arr_set_len called with NULL array\n");
        return;
    }
     ((unsigned int *)arr)[-3] = len;
}

// Gets the current allocated capacity (in number of elements)
static inline unsigned int sized_arr_capacity(const void *arr) {
    if (arr == NULL) {
        fprintf(stderr, "ERROR: sized_arr_capacity called with NULL array\n");
        return 0;
    }
    return ((unsigned int *)arr)[-2];
}

// Sets the current allocated capacity
static inline void sized_arr_set_capacity(void *arr, unsigned int capacity) {
     if (arr == NULL) {
        fprintf(stderr, "ERROR: sized_arr_set_capacity called with NULL array\n");
        return;
     }
    ((unsigned int *)arr)[-2] = capacity;
}

// Gets the size of each element in bytes
static inline unsigned int sized_arr_element_size(const void *arr) {
    if (arr == NULL) {
        fprintf(stderr, "ERROR: sized_arr_element_size called with NULL array\n");
        return 0;
    }
    return ((unsigned int *)arr)[-1];
}

// Sets the element size (should only be done at initialization)
static inline void sized_arr_set_element_size(void *arr, unsigned int element_size) {
    if (arr == NULL) {
        fprintf(stderr, "ERROR: sized_arr_set_element_size called with NULL array\n");
        return;
    }
    ((unsigned int *)arr)[-1] = element_size;
}

/**
 * @brief Initializes a new sized array.
 *
 * @param element_size The size (in bytes) of each element. Must be > 0.
 * @param initial_capacity The initial number of elements to allocate space for.
 * The actual allocated capacity will be the next multiple
 * of SIZED_ARR_BLOCK_SIZE >= initial_capacity.
 * @return A pointer to the data area of the sized array, or NULL on failure.
 */
void * sized_arr_init(size_t element_size, unsigned int initial_capacity) {
    if (element_size == 0) {
        fprintf(stderr, "ERROR: sized_arr_init called with element_size=0\n");
        return NULL;
    }

    // Calculate initial capacity based on block size
    unsigned int actual_capacity = 0;
    if (initial_capacity > 0) {
         // Ceiling division trick to find the next multiple
         actual_capacity = ((initial_capacity + SIZED_ARR_BLOCK_SIZE - 1) / SIZED_ARR_BLOCK_SIZE) * SIZED_ARR_BLOCK_SIZE;
    }
     // Even if initial_capacity is 0, we might allocate a minimum block,
     // or just allocate metadata space. Let's allocate only metadata if capacity is 0.
     // If initial_capacity > 0, actual_capacity is guaranteed >= SIZED_ARR_BLOCK_SIZE.

    size_t total_bytes = (sizeof(unsigned int) * 3) + (actual_capacity * element_size);
    void *mem_block = malloc(total_bytes);

    if (mem_block == NULL) {
        perror("ERROR: sized_arr_init malloc failed");
        return NULL;
    }

    // Initialize metadata
    unsigned int *metadata = (unsigned int *)mem_block;
    metadata[0] = 0;                   // Initial length is 0
    metadata[1] = actual_capacity;     // Initial capacity
    metadata[2] = (unsigned int)element_size; // Element size

    // Return pointer to the data area
    return metadata + 3;
}

/**
 * @brief Frees the memory allocated for the sized array.
 *
 * @param arr A pointer to the data area of the sized array. If NULL, does nothing.
 */
void sized_arr_free(void *arr) {
    if (arr != NULL) {
        free(sized_arr_mem_addr(arr));
    }
}

/**
 * @brief Recalculates the required capacity based on the current length
 * and resizes the allocated memory if necessary. The new capacity
 * will be the smallest multiple of SIZED_ARR_BLOCK_SIZE >= length.
 *
 * @param arr A pointer to the variable holding the array pointer (void **).
 * This allows the function to update the pointer if realloc moves memory.
 */
static void sized_arr_recalculate_and_resize(void **arr) {
    if (arr == NULL || *arr == NULL) return; // Nothing to resize

    unsigned int current_len = sized_arr_len(*arr);
    unsigned int current_capacity = sized_arr_capacity(*arr);
    unsigned int element_size = sized_arr_element_size(*arr);

    // Calculate the minimum required capacity (must hold current_len elements)
    // Ensure it's a multiple of SIZED_ARR_BLOCK_SIZE
    unsigned int required_capacity = 0;
     if (current_len > 0) {
        required_capacity = ((current_len + SIZED_ARR_BLOCK_SIZE - 1) / SIZED_ARR_BLOCK_SIZE) * SIZED_ARR_BLOCK_SIZE;
     }
     // Handle case where length is 0 - required capacity can also be 0.

    // Only resize if the required capacity is different from the current capacity
    if (required_capacity != current_capacity) {
        size_t new_total_bytes = (sizeof(unsigned int) * 3) + (required_capacity * element_size);
        void *current_mem_addr = sized_arr_mem_addr(*arr);
        void *new_mem_addr = realloc(current_mem_addr, new_total_bytes);

        if (new_mem_addr == NULL && new_total_bytes > 0) {
            // realloc failed, but only report error if we were actually asking for memory
            perror("ERROR: sized_arr_recalculate_and_resize realloc failed");
            // Keep the old array intact, don't change metadata or pointer
            return;
        }

        // Update the pointer in the calling scope *before* updating metadata
        // because the metadata access relies on the correct pointer.
        *arr = ((unsigned int *)new_mem_addr) + 3;

        // Update the capacity metadata
        sized_arr_set_capacity(*arr, required_capacity);
    }
}

/**
 * @brief Inserts an element at a specific index, shifting existing elements.
 *
 * @param arr Pointer to the array variable (void **). Will be updated if resized.
 * @param index The index at which to insert the value. Must be <= current length.
 * @param value Pointer to the data to be inserted. Must be element_size bytes.
 */
void sized_arr_insert_at(void **arr, unsigned int index, const void *value) {
    if (arr == NULL || *arr == NULL || value == NULL) {
         fprintf(stderr, "ERROR: sized_arr_insert_at called with NULL argument(s)\n");
        return;
    }

    unsigned int current_len = sized_arr_len(*arr);
    unsigned int element_size = sized_arr_element_size(*arr);

    if (index > current_len) {
        fprintf(stderr, "ERROR: sized_arr_insert_at index (%u) out of bounds (%u)\n", index, current_len);
        return;
    }

    // 1. Increase logical length first
    sized_arr_set_len(*arr, current_len + 1);

    // 2. Ensure capacity (recalculate and resize if needed)
    //    This needs to happen *after* increasing length but *before* memory moves.
    sized_arr_recalculate_and_resize(arr);
     // Important: *arr might have changed if realloc occurred.
     // Re-fetch potentially updated pointer and element_size if needed,
     // though element_size won't change here.
     void* current_arr_ptr = *arr; // Use a local variable after potential realloc
     element_size = sized_arr_element_size(current_arr_ptr); // Re-fetch just in case (though unlikely to change)


    // 3. Shift elements to make space (if inserting not at the very end)
    if (index < current_len) {
        char *base_ptr = (char *)current_arr_ptr;
        void *dest = base_ptr + (index + 1) * element_size;
        void *src = base_ptr + index * element_size;
        size_t bytes_to_move = (current_len - index) * element_size;
        memmove(dest, src, bytes_to_move);
    }

    // 4. Copy the new value into the gap
    char *base_ptr = (char *)current_arr_ptr; // Use potentially updated pointer
    void *insert_pos = base_ptr + index * element_size;
    memcpy(insert_pos, value, element_size);
}

/**
 * @brief Appends an element to the end of the array.
 *
 * @param arr Pointer to the array variable (void **). Will be updated if resized.
 * @param value Pointer to the data to be appended. Must be element_size bytes.
 */
void sized_arr_push_back(void **arr, const void *value) {
     if (arr == NULL || *arr == NULL) {
        fprintf(stderr, "ERROR: sized_arr_push_back called with NULL array\n");
        return;
     }
    // Insert at the current length index
    sized_arr_insert_at(arr, sized_arr_len(*arr), value);
}

/**
 * @brief Removes a specified number of elements starting at a given index.
 *
 * @param arr Pointer to the array variable (void **). Will be updated if resized.
 * @param index The starting index of the elements to remove. Must be < current length.
 * @param length The number of elements to remove. Must be > 0.
 */
void sized_arr_remove(void **arr, unsigned int index, unsigned int length) {
     if (arr == NULL || *arr == NULL) {
        fprintf(stderr, "ERROR: sized_arr_remove called with NULL array\n");
        return;
     }
    unsigned int current_len = sized_arr_len(*arr);
    unsigned int element_size = sized_arr_element_size(*arr);

    if (length == 0) return; // Nothing to remove

    if (index >= current_len || (index + length) > current_len) {
        fprintf(stderr, "ERROR: sized_arr_remove index (%u) or length (%u) out of bounds for length (%u)\n", index, length, current_len);
        return;
    }

    // 1. Shift elements to overwrite the removed section (if not removing from the end)
    unsigned int elements_after_removed = current_len - (index + length);
    if (elements_after_removed > 0) {
        char *base_ptr = (char *)(*arr);
        void *dest = base_ptr + index * element_size;
        void *src = base_ptr + (index + length) * element_size;
        size_t bytes_to_move = elements_after_removed * element_size;
        memmove(dest, src, bytes_to_move);
    }

    // 2. Decrease logical length
    sized_arr_set_len(*arr, current_len - length);

    // 3. Recalculate capacity and potentially shrink the array
    sized_arr_recalculate_and_resize(arr);
    // *arr might have changed
}

/**
 * @brief Removes and returns the last element of the array.
 * Allocates memory for the returned element, caller must free it.
 *
 * @param arr Pointer to the array variable (void **). Will be updated if resized.
 * @return Pointer to the removed element (allocated with malloc), or NULL if
 * the array was empty or memory allocation failed.
 */
void *sized_arr_pop_back(void **arr) {
     if (arr == NULL || *arr == NULL) {
        fprintf(stderr, "ERROR: sized_arr_pop_back called with NULL array\n");
        return NULL;
     }
    unsigned int current_len = sized_arr_len(*arr);
    if (current_len == 0) {
        // fprintf(stderr, "Warning: sized_arr_pop_back called on empty array\n");
        return NULL; // Array is empty
    }
    unsigned int element_size = sized_arr_element_size(*arr);

    // Allocate memory for the element to return
    void *popped_value = malloc(element_size);
    if (popped_value == NULL) {
        perror("ERROR: sized_arr_pop_back malloc failed");
        return NULL;
    }

    // Copy the last element's data
    char *base_ptr = (char *)(*arr);
    void *last_element_addr = base_ptr + (current_len - 1) * element_size;
    memcpy(popped_value, last_element_addr, element_size);

    // Remove the last element (length=1)
    // Use sized_arr_remove which handles length update and resizing
    sized_arr_remove(arr, current_len - 1, 1);

    return popped_value;
}

/**
 * @brief Removes and returns the first element of the array.
 * Allocates memory for the returned element, caller must free it.
 *
 * @param arr Pointer to the array variable (void **). Will be updated if resized.
 * @return Pointer to the removed element (allocated with malloc), or NULL if
 * the array was empty or memory allocation failed.
 */
void *sized_arr_pop_front(void **arr) {
    if (arr == NULL || *arr == NULL) {
        fprintf(stderr, "ERROR: sized_arr_pop_front called with NULL array\n");
        return NULL;
     }

    unsigned int current_len = sized_arr_len(*arr);
     if (current_len == 0) {
        // fprintf(stderr, "Warning: sized_arr_pop_front called on empty array\n");
        return NULL; // Array is empty
    }
    unsigned int element_size = sized_arr_element_size(*arr);

    // Allocate memory for the element to return
    void *popped_value = malloc(element_size);
    if (popped_value == NULL) {
        perror("ERROR: sized_arr_pop_front malloc failed");
        return NULL;
    }

    // Copy the first element's data
    memcpy(popped_value, *arr, element_size); // First element is at *arr

    // Remove the first element (index=0, length=1)
    // Use sized_arr_remove which handles length update and resizing
    sized_arr_remove(arr, 0, 1);

    return popped_value;
}

/**
 * @brief Inserts elements from a source memory block into the array at a specific index.
 *
 * @param arr Pointer to the array variable (void **). Will be updated if resized.
 * @param index The index at which to insert the elements. Must be <= current length.
 * @param src_data Pointer to the source data block.
 * @param src_len Number of elements to insert from src_data.
 */
void sized_arr_insert_array(void **arr, unsigned int index, const void *src_data, unsigned int src_len) {
    if (arr == NULL || *arr == NULL || (src_data == NULL && src_len > 0)) {
         fprintf(stderr, "ERROR: sized_arr_insert_array called with NULL argument(s)\n");
        return;
    }
    if (src_len == 0) return; // Nothing to insert

    unsigned int current_len = sized_arr_len(*arr);
    unsigned int element_size = sized_arr_element_size(*arr);

    if (index > current_len) {
        fprintf(stderr, "ERROR: sized_arr_insert_array index (%u) out of bounds (%u)\n", index, current_len);
        return;
    }

    // 1. Increase logical length first
    sized_arr_set_len(*arr, current_len + src_len);

    // 2. Ensure capacity
    sized_arr_recalculate_and_resize(arr);
    // Re-fetch potentially updated pointer
    void* current_arr_ptr = *arr;
    element_size = sized_arr_element_size(current_arr_ptr); // Re-fetch just in case

    // 3. Shift existing elements to make space (if not inserting at the end)
    if (index < current_len) {
        char *base_ptr = (char *)current_arr_ptr;
        void *dest = base_ptr + (index + src_len) * element_size;
        void *src = base_ptr + index * element_size;
        size_t bytes_to_move = (current_len - index) * element_size; // Move original elements after index
        memmove(dest, src, bytes_to_move);
    }

    // 4. Copy the new data into the gap
    char *base_ptr = (char *)current_arr_ptr;
    void *insert_pos = base_ptr + index * element_size;
    memcpy(insert_pos, src_data, src_len * element_size);
}


#endif // _SIZED_ARR_
