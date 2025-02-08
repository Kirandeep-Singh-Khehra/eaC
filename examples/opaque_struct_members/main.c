#include "struct_def.h"
#include "struct_def_bfm.h"
#include <stdio.h>

void test_non_bfm() {
  printf("-------- TESTING BIT-FIELD METHOD --------\n");
  struct m_t m = new_m_t(10, 20);

  print_m_t(m);

  m_t_set_d(&m, 35);

  printf("m.d: %d\n", m_t_get_d(m));
  // printf("m.d: %d\n", m.d); // Gives error because it is hidden

  printf("m._: %x\n", m._); // m._ is visible for non-bit-field method

  print_m_t(m);
}

void test_bfm() {
  printf("-------- TESTING NON-BIT-FIELD METHOD --------\n");

  struct m2_t m = new_m2_t(10, 20);

  print_m2_t(m);

  m2_t_set_d(&m, 35);

  printf("m.d: %d\n", m2_t_get_d(m));
  // printf("m.d: %d\n", m.d); // Gives error because it is hidden

  // printf("m._: %x\n", m._); // m._ is not visible for bit-field method

  print_m2_t(m);
}

int main() {
  test_non_bfm();
  test_bfm();
}
