#include "struct_def.h"
#include <stdio.h>

int main() {
  struct m_t m = new_m_t(10, 20);

  print_m_t(m);

  m_t_set_d(&m, 35);
  printf("m.d: %d\n", m_t_get_d(m));
  // printf("m.d: %d\n", m.d); // Gives error because it is hidden

  print_m_t(m);
}
