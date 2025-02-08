#include <stdio.h>

// DEFINE STRUCT //
#define STRUCT_DEF STRUCT(m_t, \
  VAR(int, a) \
  VAR(char, b) \
  VAR(float, c) \
  VAR(double, d) \
)
// Or define STRUCT_DEF_FILE "file_name"

#define STRUCT_TO_STRING // To create print functions
#define STRUCT_GET_PROPS // To create print functions
#include "../../struct_serialize.h"

// DEFINE STRUCT //
#define STRUCT_DEF STRUCT(m2_t, \
  VAR(int, a) \
  VAR(m_t, b) \
)
// Or define STRUCT_DEF_FILE "file_name"

#define STRUCT_TO_STRING // To create print functions
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
  m_t_print(m);
  printf("\n");
  m2_t_print(m2);

  printf("\n-----\n");
  struct_props_t props = m2_t_get_props();
  printf("n_props: %d\n", props.n_props);
  for (int i = 0; i < props.n_props; i++) {
    printf(" - %s: %s\n", props.props[i].name, props.props[i].type);
  }
  printf("\n");
}

