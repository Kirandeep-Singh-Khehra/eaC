#include "../../opaque_struct_members.h"

struct m_t {
    int a;
    int b;
    PRIVATE(
      int c;
      int d;
    )
};

struct m_t new_m_t(int a, int b);
void print_m_t(struct m_t m);
int m_t_get_c(struct m_t m);
void m_t_set_c(struct m_t* m, int c);
int m_t_get_d(struct m_t m);
void m_t_set_d(struct m_t* m, int d);

#ifdef PRIVATE
#undef PRIVATE
#endif
