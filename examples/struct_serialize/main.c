#include <stdio.h>

// DEFINE STRUCT //
#define STRUCT_PRINT // To create print functions
#define STRUCT_GET_PROPS // To create functions to list props
#define STRUCT_GET_PROPS_CONST // To create const prop

#define STRUCT_DEF STRUCT(m_t, \
  VAR(int, a) \
  VAR(char, b) \
  VAR(float, c) \
  VAR(double, d) \
)
// Or define STRUCT_DEF_FILE "file_name"
#include "../../struct_serialize.h"

// DEFINE STRUCT //
#define STRUCT_DEF STRUCT(m2_t, \
  VAR(int, a) \
  VAR(m_t, b) \
)
#include "../../struct_serialize.h"

struct m_t m = {
  .a = 10,
  .b = 'S',
  .c = 10.2,
  .d = 40.5
};

struct m2_t m2 = {
  .a = 15,
};

int main() {
  m2.b = m;
  m_t_print(1, m);
  printf("\n");
  m2_t_print(1, m2);

  printf("\n-----\n");
  long m2_t_prop_count = sizeof(m_t_props)/sizeof(var_props_t);
  printf("n_props: %ld\n", m2_t_prop_count);
  for (int i = 0; i < sizeof(m_t_props)/sizeof(var_props_t); i++) {
    printf(" - %s: %s\n", m_t_props[i].name, m_t_props[i].type);
  }
  printf("\n");
}

