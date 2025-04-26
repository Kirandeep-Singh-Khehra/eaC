#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

// #define SIZED_ARR_BLOCK_SIZE 16 // To override block size
#include "../../sized_arr.h"

// Define a sample data structure
struct data {
    int a;
    float b;
    char c[32];
};

// Helper function to print the array contents
void print_array(const char *label, struct data *arr) {
    printf("--- %s ---\n", label);
    if (arr == NULL) {
        printf("Array is NULL\n");
        return;
    }
    unsigned int len = sized_arr_len(arr);
    // Accessing internal details for demonstration only
    unsigned int cap = ((unsigned int *)arr)[-2];
    unsigned int elem_size = ((unsigned int *)arr)[-1];

    printf("Length: %u, Capacity: %u, Element Size: %u\n", len, cap, elem_size);
    if (len == 0) {
        printf("Array is empty.\n");
    } else {
        for (unsigned int i = 0; i < len; i++) {
            printf("  [%u] { .a = %d, .b = %.2f, .c = \"%s\" }\n", i, arr[i].a, arr[i].b, arr[i].c);
        }
    }
    printf("---------------\n");
}

int main() {
    struct data *my_array = NULL;
    struct data temp_item;

    printf("=== Basic Sized Array Usage ===\n");

    // Initialize an array to hold 'struct data' with an initial capacity of 3
    my_array = sized_arr_init(sizeof(struct data), 3);
    assert(my_array != NULL);
    print_array("Initial Array", my_array);

    // Add elements to the end of the array
    temp_item.a = 10; temp_item.b = 1.1f; strcpy(temp_item.c, "One");
    sized_arr_push_back((void **)&my_array, &temp_item);
    temp_item.a = 20; temp_item.b = 2.2f; strcpy(temp_item.c, "Two");
    sized_arr_push_back((void **)&my_array, &temp_item);
    temp_item.a = 30; temp_item.b = 3.3f; strcpy(temp_item.c, "Three");
    sized_arr_push_back((void **)&my_array, &temp_item);
    print_array("After Pushing Three Items", my_array);
    assert(sized_arr_len(my_array) == 3);

    // Access elements using array indexing
    printf("Element at index 1: { .a = %d, .b = %.2f, .c = \"%s\" }\n", my_array[1].a, my_array[1].b, my_array[1].c);
    assert(my_array[1].a == 20);

    // Insert an element at the beginning
    temp_item.a = 5; temp_item.b = 0.5f; strcpy(temp_item.c, "First");
    sized_arr_insert_at((void **)&my_array, 0, &temp_item);
    print_array("After Inserting at Start", my_array);
    assert(sized_arr_len(my_array) == 4);
    assert(my_array[0].a == 5);

    // Remove the last element
    struct data *popped_item = (struct data *)sized_arr_pop_back((void **)&my_array);
    if (popped_item != NULL) {
        printf("Popped item: { .a = %d, .b = %.2f, .c = \"%s\" }\n", popped_item->a, popped_item->b, popped_item->c);
        free(popped_item); // Remember to free the returned pointer
    }
    print_array("After Pop Back", my_array);
    assert(sized_arr_len(my_array) == 3);

    // Remove the first element
    popped_item = (struct data *)sized_arr_pop_front((void **)&my_array);
    if (popped_item != NULL) {
        printf("Popped front item: { .a = %d, .b = %.2f, .c = \"%s\" }\n", popped_item->a, popped_item->b, popped_item->c);
        free(popped_item);
    }
    print_array("After Pop Front", my_array);
    assert(sized_arr_len(my_array) == 2);
    assert(my_array[0].a == 10); // Element shifted

    // Free the dynamically allocated array
    sized_arr_free(my_array);
    my_array = NULL; // Good practice to set the pointer to NULL after freeing

    printf("\nBasic Usage Completed.\n");

    return 0;
}
