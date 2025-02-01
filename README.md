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

More utils on the way ...

