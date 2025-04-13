#include "../muvector.h"
#include <assert.h>
#include <complex.h>
#include <stdio.h>
#include <stdlib.h>

MUVECTOR_DEFINE(int);

int test_01(void) {
  int res = EXIT_SUCCESS;
#define VECTOR_LENGTH 512
  muvector_init_static(v, int, VECTOR_LENGTH);
  for (int i = 0; i < VECTOR_LENGTH; i++) {
    muvector_append(&v, i);
  }
  assert(v.length == VECTOR_LENGTH);
  for (int i = 0; i < v.length; i++) {
    if (v.items[i] != i) {
      res = EXIT_FAILURE;
      goto exit;
    }
  }
exit:
  return res;
}

int test_02(void) {
  int res = EXIT_SUCCESS;
  muvector_init_std(v, int);
#define VECTOR_LENGTH 512
  for (int i = 0; i < VECTOR_LENGTH; i++) {
    muvector_append(&v, i);
  }
  assert(v.length == VECTOR_LENGTH);
  for (int i = 0; i < v.length; i++) {
    if (v.items[i] != i) {
      res = EXIT_FAILURE;
      goto exit;
    }
  }
exit:
  muvector_destroy(&v);
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
  printf(">>> test_02 ... ");
  if (test_02() == EXIT_SUCCESS) {
    printf("ok\n");
  } else {
    printf("fail\n");
  }
}
