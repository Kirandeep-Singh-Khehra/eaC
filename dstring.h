#ifndef _D_STRING_H_
#define _D_STRING_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Assuming sized_arr.h is in the same directory, otherwise adjust the path.
#include "sized_arr.h"

#ifndef STR_BLOCK_SIZE
#define STR_BLOCK_SIZE 64
#endif

typedef char* string;

// Function Declarations
unsigned int string_block_size(string s);
char* string_mem_addr(string s);
string string_init();
unsigned int string_length(string s);
string string_append_char(string s, char c);
string string_append(string s, const char* str);
string string_copy(const char* str);
void string_free(string s);
char* string_c_str(string s);
string string_insert_at(string s, const char* str, unsigned int index);
string string_remove_substring(string s, unsigned int index, unsigned int length);
string string_reset_size(string s);

// Function Implementations

// Helper function to get the allocated size of the string (in terms of blocks)
unsigned int string_block_size(string s) {
    return sized_arr_len(s);
}

// Helper function to get the actual memory address of the string
char* string_mem_addr(string s) {
  return (char*)sized_arr_mem_addr(s);
}

// Function to initialize a string
string string_init() {
    string s = (string)sized_arr_init(sizeof(char), STR_BLOCK_SIZE);
    if (s == NULL) {
        perror("string_init failed");
        return NULL;
    }
    s[0] = '\0'; // Ensure it's a valid empty string
    return s;
}

// Function to get the length of a string (excluding the null terminator)
unsigned int string_length(string s) {
    if (s == NULL) return 0;
    return strlen(s);
}

// Function to append a character to a string
string string_append_char(string s, char c) {
    if (s == NULL) return NULL;

    unsigned int current_length = string_length(s);
    unsigned int current_block_size = string_block_size(s);

    if (current_length + 1 >= current_block_size) {
        // Reallocate memory in blocks of STR_BLOCK_SIZE
        string new_s = (string)sized_arr_init(sizeof(char), current_block_size + STR_BLOCK_SIZE);
        if (new_s == NULL) {
            perror("string_append_char failed to reallocate");
            return s; // Return the original string, even if allocation failed.  Important.
        }
        strcpy(new_s, s); // Copy the existing string
        sized_arr_free(s);  // Free the old string
        s = new_s; //update the pointer
    }
    s[current_length] = c;
    s[current_length + 1] = '\0';
    return s;
}

// Function to append a C-style string to a string
string string_append(string s, const char* str) {
    if (s == NULL || str == NULL) return NULL;

    unsigned int current_length = string_length(s);
    unsigned int append_length = strlen(str);
    unsigned int current_block_size = string_block_size(s);

    if (current_length + append_length >= current_block_size) {
        // Calculate the number of blocks needed, increase in STR_BLOCK_SIZE increments
        unsigned int needed_blocks = ((current_length + append_length + 1 + STR_BLOCK_SIZE - 1) / STR_BLOCK_SIZE) * STR_BLOCK_SIZE; // Ensure null term
        string new_s = (string)sized_arr_init(sizeof(char), needed_blocks);

        if (new_s == NULL) {
            perror("string_append failed to reallocate");
            return s; // Return original
        }
        strcpy(new_s, s);
        sized_arr_free(s);
        s = new_s;
    }
    strcat(s, str);
    return s;
}

// Function to copy a C-style string into a string
string string_copy(const char* str) {
    if (str == NULL) return NULL;

    unsigned int str_length = strlen(str);
    unsigned int needed_blocks = ((str_length + 1 + STR_BLOCK_SIZE - 1) / STR_BLOCK_SIZE) * STR_BLOCK_SIZE; // Include null terminator
    string new_s = (string)sized_arr_init(sizeof(char), needed_blocks);
    if (new_s == NULL)
    {
      return NULL;
    }
    strcpy(new_s, str);
    return new_s;
}

// Function to free the memory allocated for a string
void string_free(string s) {
    sized_arr_free(s);
}

// Function to get the underlying char* of the string
char* string_c_str(string s) {
    return s;
}

// Function to insert a C-style string at a given index
string string_insert_at(string s, const char* str, unsigned int index) {
    if (s == NULL || str == NULL) return NULL;

    unsigned int current_length = string_length(s);
    unsigned int insert_length = strlen(str);
    unsigned int current_block_size = string_block_size(s);

    if (index > current_length) {
        fprintf(stderr, "Error: Index out of bounds in string_insert_at\n");
        return s; // Or handle the error as you see fit (e.g., return NULL, exit)
    }

    if (current_length + insert_length >= current_block_size) {
        // Reallocate memory
        unsigned int needed_blocks = ((current_length + insert_length + 1 + STR_BLOCK_SIZE - 1) / STR_BLOCK_SIZE) * STR_BLOCK_SIZE;
        string new_s = (string)sized_arr_init(sizeof(char), needed_blocks);
        if (new_s == NULL) {
            perror("string_insert_at failed to reallocate");
            return s;
        }
        // Copy the part of the string before the insertion point
        strncpy(new_s, s, index);
        // Copy the string to be inserted
        strcpy(new_s + index, str);
        // Copy the rest of the original string
        strcpy(new_s + index + insert_length, s + index);
        sized_arr_free(s);
        s = new_s;
    } else {
        // Make space for the inserted string by moving the existing characters
        memmove(s + index + insert_length, s + index, current_length - index + 1); // +1 for null terminator
        // Copy the inserted string into the gap
        memcpy(s + index, str, insert_length);
    }
    return s;
}

// Function to remove a substring from a string
string string_remove_substring(string s, unsigned int index, unsigned int length) {
    if (s == NULL) return NULL;

    unsigned int current_length = string_length(s);

    if (index >= current_length) {
        fprintf(stderr, "Error: Index out of bounds in string_remove_substring\n");
        return s; // Or handle the error as you see fit.
    }

    if (length == 0) return s; // Nothing to remove.

    if (index + length > current_length) {
        length = current_length - index; // Adjust length if it goes beyond the string.
    }

    // Shift the remaining part of the string to overwrite the substring to be removed
    memmove(s + index, s + index + length, current_length - index - length + 1); // +1 for null terminator

    s = string_reset_size(s);
    return s;
}

string string_reset_size(string s) {
    if (s == NULL) return NULL;
    unsigned int current_length = string_length(s);
    unsigned int current_block_size = string_block_size(s);

    // Calculate the number of blocks needed
    unsigned int needed_blocks = ((current_length + 1 + STR_BLOCK_SIZE - 1) / STR_BLOCK_SIZE); // Ensure null terminator is included.

    if (needed_blocks < current_block_size) {
        // Reallocate memory to the smaller size.
        string new_s = (string)sized_arr_init(sizeof(char), needed_blocks * STR_BLOCK_SIZE);
        if (new_s == NULL) {
            perror("string_reset_size failed to reallocate");
            return s; // Return the original string, even if allocation failed
        }
        strcpy(new_s, s);
        sized_arr_free(s);
        s = new_s;
    }
    return s;
}

#endif // _D_STRING_H_

