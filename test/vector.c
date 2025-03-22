#include "../muvector.h"
#include <complex.h>
#include <stdio.h>

struct int_vector {
  int *items;
  size_t length;
  size_t capacity;
};

int test_01(void) {
  int res = EXIT_SUCCESS;
  struct int_vector v = {0};
  for (int i = 0; i < 100; i++) {
    MUVECTOR_APPEND(&v, i);
  }
  for (int i = 0; i < v.length; i++) {
    if (v.items[i] != i) {
      res = EXIT_FAILURE;
      goto exit;
    }
  }

exit:
  return res;
}

int main(int argc, char *argv[]) {
  printf("\n======================= vector tests ========================\n");
  printf(">>> test_01 ... ");
  if (test_01() == EXIT_SUCCESS) {
    printf("ok\n");
  } else {
    printf("fail\n");
  }
}
