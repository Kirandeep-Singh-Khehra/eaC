#include <stdio.h>

#include "../../prop_ptr.h"

struct p_t {
  int pa;
  float pb;
  char pc[10];
};

struct m_t {
  int ma;
  char pb;
};

int main() {
  printf("Prop size: %ld\n", sizeof(struct p_t));
  printf("Data size: %ld\n", sizeof(struct m_t));

  struct m_t *m_ptr =
      (struct m_t *)prop_ptr_init(sizeof(struct p_t), sizeof(struct m_t));
  prop_ptr_set_prop(m_ptr, &(struct p_t){10, 2.5, "asd"}, sizeof(struct p_t));

  struct p_t *p_ptr = (struct p_t *)prop_ptr_get_prop(m_ptr);
  printf("p_t:{.pa = %d, .bc = %f, .pc = \"%s\"}\n", p_ptr->pa, p_ptr->pb,
         p_ptr->pc);

  printf("Data address %p\n", m_ptr);
  printf("Prop address %p\n", p_ptr);

  printf("Prop size: %ld\n", prop_ptr_get_prop_size(m_ptr));

  prop_ptr_free(m_ptr);
}
