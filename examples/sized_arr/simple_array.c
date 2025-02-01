#include "../../sized_arr.h"
#include <stdio.h>

#define LENGTH 10

struct data {
  int a;
  float b;
  char c[32];
};

int main() {
  struct data *d_arr = sized_arr_init(sizeof(struct data), LENGTH);

  printf("Length: %d\n", sized_arr_len(d_arr));

  for (int i = 0; i < LENGTH; i++) {
    d_arr[i].a = i;
    d_arr[i].b = i * 0.1f;
    d_arr[i].c[0] = 'a' + i;
    d_arr[i].c[1] = '\0';
  }

  for (int i = 0; i < LENGTH; i++) {
    printf("%d, { .a = %d, .b = %f, .c = \"%s\"}\n", i, d_arr[i].a, d_arr[i].b, d_arr[i].c);
  }
  
  return 0;
}

