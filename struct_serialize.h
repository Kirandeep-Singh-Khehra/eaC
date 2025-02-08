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

typedef char string[32];
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
void dtype##_print(dtype a) { \
  printf(GET_PATTERN_FROM_TYPE(a), a); \
}

GEN_PRINT_FUN(char);
GEN_PRINT_FUN(int);
GEN_PRINT_FUN(long);
GEN_PRINT_FUN(float);
GEN_PRINT_FUN(double);

#endif //!_STRUCT_UTLS_

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
#ifdef STRUCT_TO_STRING

#define VAR(type, var_name) \
printf(" .%s: ", #var_name); \
type##_print(s.var_name); \
printf(" ");

#define STRUCT(struct_name, data) \
void struct_name##_print(struct struct_name s) { \
  printf("(%s){", #struct_name); \
  data \
  printf("}"); \
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
#ifdef STRUCT_GET_PROPS

#define VAR(type, var_name) \
{ #var_name, #type},

#define STRUCT(struct_name, data) \
struct_props_t struct_name##_get_props() { \
    var_props_t var_props[] = { \
      data \
    }; \
    struct_props_t props = (struct_props_t) { \
        .n_props = sizeof(var_props)/sizeof(struct var_props_t), \
    }; \
    props.props = (var_props_t*)malloc(props.n_props * sizeof(var_props_t)); \
    memcpy(props.props, var_props, props.n_props * sizeof(var_props_t));\
    return props; \
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

#ifdef STRUCT_DEF
#undef STRUCT_DEF
#endif

#ifdef STRUCT_DEF_FILE
#undef STRUCT_DEF_FILE
#endif

#endif // STRUCT_GET_PROPS

