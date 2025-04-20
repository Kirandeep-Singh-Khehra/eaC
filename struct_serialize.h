#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* PLANS //

STRUCT (name,
  VAR(var_type, var_name)
  VAR(var_type, var_name)
)

Stuff to add
 -[x] Declare struct
 -[x] Create struct to string(currently printing). Try adding print to fd
 -[x] Generate struct props(currently there is a function but try to declare it in var)
 -[ ] 
*/

#ifndef _STRUCT_UTLS_
#define _STRUCT_UTLS_

typedef char *string;
typedef struct var_props_t {
  string name; // 0: name, 1: type
  string type; // 0: name, 1: type
} var_props_t;

typedef struct struct_props_t {
  int n_props;
  struct var_props_t* props;
} struct_props_t;

typedef char* char_ptr;
typedef int* int_ptr;
typedef long* long_ptr;
typedef float* float_ptr;
typedef double* double_ptr;

#define GET_PATTERN_FROM_TYPE(x) _Generic(x, \
  char: "%c", \
  int: "%d", \
  long: "%ld", \
  float: "%f", \
  double: "%lf", \
  default: "%x")
  // string: "%s",

#define GEN_PRINT_FUN(dtype) \
void dtype##_print(int fd, dtype a) { \
  dprintf(fd, GET_PATTERN_FROM_TYPE(a), a); \
}

GEN_PRINT_FUN(char);
GEN_PRINT_FUN(int);
GEN_PRINT_FUN(long);
GEN_PRINT_FUN(float);
GEN_PRINT_FUN(double);

#endif //!_STRUCT_UTLS_


#ifdef CLEAR_STRUCT_MACROS

#undef CLEAR_STRUCT_MACROS
#endif

/*************************************** DECLARE *********/
#define VAR(type, var_name) \
type var_name;

#define STRUCT(struct_name, data) \
typedef struct struct_name { \
  data \
} struct_name;
/*********************************************************/

#ifdef STRUCT_DEF_FILE
  #include STRUCT_DEF_FILE
#else // STRUCT_DEF_FILE
  #ifdef STRUCT_DEF
    STRUCT_DEF
  #endif // STRUCT_DEF
#endif // STRUCT_DEF_FILE

#undef STRUCT
#undef VAR

/*************************************** PRINT ***********/
#ifdef STRUCT_PRINT

#define VAR(type, var_name) \
dprintf(fd, " .%s: ", #var_name); \
type##_print(fd, s.var_name); \
dprintf(fd, " ");

#define STRUCT(struct_name, data) \
void struct_name##_print(int fd, struct struct_name s) { \
  dprintf(fd, "(%s){", #struct_name); \
  data \
  dprintf(fd, "}"); \
};
/*********************************************************/

#ifdef STRUCT_DEF_FILE
  #include STRUCT_DEF_FILE
#else // STRUCT_DEF_FILE
  #ifdef STRUCT_DEF
    STRUCT_DEF
  #endif // STRUCT_DEF
#endif // STRUCT_DEF_FILE

#undef STRUCT
#undef VAR

#endif // STRUCT_TO_STRING

/*************************************** PROPS ***********/
#ifdef STRUCT_GET_PROPS_CONST

#define VAR(type, var_name) \
{ #var_name, #type},

#define STRUCT(struct_name, data) \
var_props_t struct_name##_props[] = { \
      data \
};
/*********************************************************/

#ifdef STRUCT_DEF_FILE
  #include STRUCT_DEF_FILE
#else // STRUCT_DEF_FILE
  #ifdef STRUCT_DEF
    STRUCT_DEF
  #endif // STRUCT_DEF
#endif // STRUCT_DEF_FILE

#undef STRUCT
#undef VAR

#endif // STRUCT_GET_PROPS

#ifdef STRUCT_DEF
#undef STRUCT_DEF
#endif

#ifdef STRUCT_DEF_FILE
#undef STRUCT_DEF_FILE
#endif


