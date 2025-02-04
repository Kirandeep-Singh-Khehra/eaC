# eaC
Utilities to make programming in C little better.

1. Dynamic arrays with sizes.[[Implementation](sized_arr.h)][[Examples](/examples/sized_arr)]

    Create dynamic arrays and access it's size whenever required. 
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


**More utils on the way ...**

