#include <stdio.h>
#define STR_BLOCK_SIZE 4
#include "../../dstring.h" // Include the header file

int main() {
    // Test case 1: Initialization and basic operations
    printf("\n--- Test Case 1: Initialization and Basic Operations ---\n");
    string str1 = string_init();
    printf("Initial string: \"%s\", length: %u, block size: %u\n", str1, string_length(str1), string_block_size(str1));

    str1 = string_append(str1, "Hello, ");
    str1 = string_append(str1, "World!");
    printf("After appending: \"%s\", length: %u, block size: %u\n", str1, string_length(str1), string_block_size(str1));

    // Test case 2: Copying a string
    printf("\n--- Test Case 2: Copying a String ---\n");
    string str2 = string_copy("Goodbye");
    printf("Copied string: \"%s\", length: %u, block size: %u\n", str2, string_length(str2), string_block_size(str2));

    // Test case 3: Appending characters
    printf("\n--- Test Case 3: Appending Characters ---\n");
    string str3 = string_init();
    str3 = string_append_char(str3, 'C');
    str3 = string_append_char(str3, 'h');
    str3 = string_append_char(str3, 'a');
    str3 = string_append_char(str3, 'r');
    printf("After appending chars: \"%s\", length: %u, block size: %u\n", str3, string_length(str3), string_block_size(str3));

    // Test case 4: Insertion
    printf("\n--- Test Case 4: Insertion ---\n");
    string str4 = string_copy("InsertTest");
    str4 = string_insert_at(str4, "AAA", 6);
    printf("After inserting \"AAA\" at index 6: \"%s\", length: %u, block size: %u\n", str4, string_length(str4), string_block_size(str4));
    str4 = string_insert_at(str4, "BBB", 0);
    printf("After inserting \"BBB\" at index 0: \"%s\", length: %u, block size: %u\n", str4, string_length(str4), string_block_size(str4));
    str4 = string_insert_at(str4, "CCC", string_length(str4));
    printf("After inserting \"CCC\" at the end: \"%s\", length: %u, block size: %u\n", str4, string_length(str4), string_block_size(str4));

   // Test case 5: Removal
    printf("\n--- Test Case 5: Removal ---\n");
    string str5 = string_copy("RemoveThisSubstring");
    str5 = string_remove_substring(str5, 6, 4);
    printf("After removing substring at index 6, length 4: \"%s\", length: %u, block size: %u\n", str5, string_length(str5), string_block_size(str5));

    str5 = string_remove_substring(str5, 0, 6);
    printf("After removing substring at index 0, length 6: \"%s\", length: %u, block size: %u\n", str5, string_length(str5), string_block_size(str5));

    str5 = string_remove_substring(str5, string_length(str5) - 3, 3);
    printf("After removing last 3 chars: \"%s\", length: %u, block size: %u\n", str5, string_length(str5), string_block_size(str5));
    
    string str6 = string_copy("short");
    str6 = string_remove_substring(str6, 0, 100); //test removal beyond length
    printf("After removing beyond length: \"%s\", length: %u, block size: %u\n", str6, string_length(str6), string_block_size(str6));

    // Test case 6: string_reset_size
    printf("\n--- Test Case 6: string_reset_size ---\n");
    string str7 = string_copy("This is a test string");
    printf("Original string: \"%s\", length: %u, block size: %u\n", str7, string_length(str7), string_block_size(str7));
    str7 = string_reset_size(str7);
    printf("After reset_size: \"%s\", length: %u, block size: %u\n", str7, string_length(str7), string_block_size(str7));
    string str8 = string_copy("a"); //length 1
    str8 = string_reset_size(str8);
    printf("After reset_size: \"%s\", length: %u, block size: %u\n", str8, string_length(str8), string_block_size(str8));

    // Test case 7: Check boundary conditions
    printf("\n--- Test Case 7: Boundary Conditions ---\n");
    string str9 = string_init();
    str9 = string_insert_at(str9, "XYZ", 0); // Insert at empty string
    printf("Inserted into empty string: \"%s\", length: %u, block size: %u\n", str9, string_length(str9), string_block_size(str9));

    string str10 = string_copy("Test");
    str10 = string_insert_at(str10, "123", 4); // Insert at end
    printf("Inserted at end: \"%s\", length: %u, block size: %u\n", str10, string_length(str10), string_block_size(str10));

    string str11 = string_copy("ABCDE");
    str11 = string_remove_substring(str11, 0, 0); // Remove 0 length
    printf("Removed 0 length: \"%s\", length: %u, block size: %u\n", str11, string_length(str11), string_block_size(str11));

    // Test case 8: Check for memory leaks.  Use valgrind on Linux/WSL to confirm.
    printf("\n--- Test Case 8: Memory Leak Check (Run with Valgrind) ---\n");
    string leak_test = string_init();
    leak_test = string_append(leak_test, "LeakTestString");
    string leak_copy = string_copy("LeakCopy");
    string_free(leak_test);
    string_free(leak_copy);
    // If there are no memory leaks, valgrind should report no errors.

    // Free all allocated strings
    string_free(str1);
    string_free(str2);
    string_free(str3);
    string_free(str4);
    string_free(str5);
    string_free(str6);
    string_free(str7);
    string_free(str8);
    string_free(str9);
    string_free(str10);
    string_free(str11);

    printf("\n--- End of Tests ---\n");
    return 0;
}


