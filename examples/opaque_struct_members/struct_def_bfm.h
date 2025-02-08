#define USE_BIT_MASK_METHOD
#include "../../opaque_struct_members.h"

struct m2_t {
    int a;
    int b;
    PRIVATE(int, c);
    PRIVATE(int, d);
};

struct m2_t new_m2_t(int a, int b);
void print_m2_t(struct m2_t m);
int m2_t_get_c(struct m2_t m);
void m2_t_set_c(struct m2_t* m, int c);
int m2_t_get_d(struct m2_t m);
void m2_t_set_d(struct m2_t* m, int d);

#undef USE_BIT_MASK_METHOD

#ifdef PRIVATE
#undef PRIVATE
#endif

