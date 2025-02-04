#define USE_PRIVATE_SCOPE
#include "struct_def.h"

#include <stdio.h>

struct m_t new_m_t(int a, int b) {
  struct m_t m = {
    .a = a,
    .b = b,
    .c = a + 20,
    .d = b + 20
  };

  return m;
}

void print_m_t(struct m_t m) {
  printf("{.a = %d, .b = %d, .c = %d, .d = %d }\n", m.a, m.b, m.c, m.d);
}

int m_t_get_c(struct m_t m) {
  return m.c;
}

void m_t_set_c(struct m_t* m, int c) {
  m->c = c;
}

int m_t_get_d(struct m_t m) {
  return m.d;
}

void m_t_set_d(struct m_t* m, int d) {
  m->d = d;
}

#undef USE_PRIVATE_SCOPE

