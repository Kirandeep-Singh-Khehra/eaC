#define USE_PRIVATE_SCOPE
#include "struct_def_bfm.h"

#include <stdio.h>

struct m2_t new_m2_t(int a, int b) {
  struct m2_t m = {
    .a = a,
    .b = b,
    .c = a + 20,
    .d = b + 20
  };

  return m;
}

void print_m2_t(struct m2_t m) {
  printf("{.a = %d, .b = %d, .c = %d, .d = %d }\n", m.a, m.b, m.c, m.d);
}

int m2_t_get_c(struct m2_t m) {
  return m.c;
}

void m2_t_set_c(struct m2_t* m, int c) {
  m->c = c;
}

int m2_t_get_d(struct m2_t m) {
  return m.d;
}

void m2_t_set_d(struct m2_t* m, int d) {
  m->d = d;
}

#undef USE_PRIVATE_SCOPE

