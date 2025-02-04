# eaC
Utilities to make programming in C little better.

1. Dynamic arrays with sizes.[[Implementation](sized_arr.h)][[Examples](/examples/sized_arr)]

    Create dynamic arrays and access it's size whenever required.
    > Size of array is stored as `unsigned int` stored at address directly before array pointer.
    ```c
    // Create a dynamic array
    struct data *d_arr = sized_arr_init(sizeof(struct data), LENGTH);
  
    // Get the length of array
    int len = sized_arr_len(d_arr);
  
    // Access elements as usual
    d_arr[INDEX].prop = 10;
    printf("%x", d_arr[INDEX].prop);
    ```

2. Struct with hidden/private/opaque members.[[Implementation](opaque_struct_members.h)][[Examples](/examples/opaque_struct_members)]

    Create structs with opaque properties/members which are visible ony in required c file.
    > For better usage object files are required to be created as explained in given explanation.

    Create a header file with definition.
    ```c
    // File: m_t.h
    #include "opaque_struct_members.h"

    struct m_t {
      int a;
      // Define private members.
      PRIVATE(
        int b;
      )
    };

    struct m_t new_m_t();
    int m_t_get_b(struct m_t);
    void m_t_set_b(struct m_t*, int);

    // Necessary to undefine PRIVATE macro.
    #ifdef PRIVATE
    #undef PRIVATE
    #endif
    ```

    Write implementation of struct.
    ```c
    // File: m_t.c
    #define USE_PRIVATE_SCOPE // Define to make private members visible in this scope.
    #include "struct_def.h"

    struct m_t new_m_t() { return (struct m_t) {.a = 10, .b = 20}; }

    int m_t_get_b(struct m_t m) { return m.b; }
    void m_t_set_b(struct m_t* m, int b) { m->b = b; }
    ```
    Compile this file to get object file.
    ```bash
    gcc -c m_t.c -o m_t.o
    ```
    Now use the header file in your code and play with it.
    ```c
    // File: main.c
    #include "m_t.h"

    int main () {
      // Declare a struct
      struct m_t m = new_m_t(); // or use `{.a = 10}`

      /* Use getter and setter for private/opaque members 
         but direct access will return error. */
      m_t_set_b(&m, 50);
      printf("%d\n", m_t_get_b(m));
      // printf("%d\n", m.b); // Will give error
    }
    ```

3. Pointer with hidden properties.[[Implementation](prop_ptr.h)][[Examples](/examples/prop_ptr)]

    Create pointer to any data structure with hidden property. Property to a pointer can be of any type.
    > Property is stored behind pointer in memory.<br>
    > For reference:<br>
    > `malloc` returns `[memory_block]` but `prop_ptr_init()` creates a memory block to store property, property size and required block and returns address of memory block. <br>
    > Storage structure is somewhat like this `[property][size_t(size of property)][memory_block]`
    ```c
    // Create a pointer with hidden property(here `struct p_t` is used as property).
    struct m_t *m_ptr =
      (struct m_t *)prop_ptr_init(sizeof(struct p_t), sizeof(struct m_t));

    // Set property
    prop_ptr_set_prop(m_ptr, &(struct p_t){10, 2.5, "asd"}, sizeof(struct p_t));

    // Get pointer to property
    struct p_t *p_ptr = (struct p_t *)prop_ptr_get_prop(m_ptr);

    // Finally free the property
    prop_ptr_free(m_ptr);
    ```
**More utils on the way ...**

