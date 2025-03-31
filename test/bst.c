
#include "../mubst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void *std_allocator_alloc(void *ctx, size_t const size) {
  return malloc(size);
}
static void *std_allocator_realloc(void *ctx, void *ptr, size_t const size) {
  return realloc(ptr, size);
}
static void std_allocator_free(void *ctx, void *ptr) { return free(ptr); }

static struct mubst_allocator std_allocator = {
    .ctx = NULL,
    .alloc = std_allocator_alloc,
    .realloc = std_allocator_realloc,
    .free = std_allocator_free,
};

#define TEST_TABLE                                                             \
  D("add test", test_01)                                                       \
  D("remove leaf test", test_rm_leaf)                                          \
  D("remove single branch test", test_rm_single)                               \
  D("remove double branch test", test_rm_double)                               \
  D("traverse test", test_traverse)

#define TEST_BST_INIT()                                                        \
  struct mubst bst;                                                            \
  mubst_init(&bst, person_keycmp, NULL);                                       \
  struct person *p1 = mubst_alloc(&bst, sizeof(struct person));                \
  MUBST_ASSERT(p1);                                                            \
  p1->name = "Kenobi";                                                         \
  p1->age = 42;                                                                \
  p1->node.key = p1->name;                                                     \
  p1->node.value = &p1->age;                                                   \
  p1->node.container = p1;                                                     \
  struct person *p2 = mubst_alloc(&bst, sizeof(struct person));                \
  MUBST_ASSERT(p2);                                                            \
  p2->name = "Grevious";                                                       \
  p2->age = 69;                                                                \
  p2->node.key = p2->name;                                                     \
  p2->node.value = &p2->age;                                                   \
  p2->node.container = p2;                                                     \
  struct person *p3 = mubst_alloc(&bst, sizeof(struct person));                \
  MUBST_ASSERT(p3);                                                            \
  p3->name = "Sogeking";                                                       \
  p3->age = 22;                                                                \
  p3->node.key = p3->name;                                                     \
  p3->node.value = &p3->age;                                                   \
  p3->node.container = p3;                                                     \
  struct person *p4 = mubst_alloc(&bst, sizeof(struct person));                \
  MUBST_ASSERT(p4);                                                            \
  p4->name = "Rufy";                                                           \
  p4->age = 23;                                                                \
  p4->node.key = p4->name;                                                     \
  p4->node.value = &p4->age;                                                   \
  p4->node.container = p4;                                                     \
  struct person *p5 = mubst_alloc(&bst, sizeof(struct person));                \
  MUBST_ASSERT(p5);                                                            \
  p5->name = "Goku";                                                           \
  p5->age = 25;                                                                \
  p5->node.key = p5->name;                                                     \
  p5->node.value = &p5->age;                                                   \
  p5->node.container = p5;                                                     \
  struct person *p6 = mubst_alloc(&bst, sizeof(struct person));                \
  MUBST_ASSERT(p6);                                                            \
  p6->name = "Kenshin";                                                        \
  p6->age = 32;                                                                \
  p6->node.key = p6->name;                                                     \
  p6->node.value = &p6->age;                                                   \
  p6->node.container = p6;                                                     \
  struct person *p7 = mubst_alloc(&bst, sizeof(struct person));                \
  MUBST_ASSERT(p7);                                                            \
  p7->name = "Spike";                                                          \
  p7->age = 28;                                                                \
  p7->node.key = p7->name;                                                     \
  p7->node.value = &p7->age;                                                   \
  p7->node.container = p7;                                                     \
  mubst_add(&bst, &p2->node);                                                  \
  mubst_add(&bst, &p1->node);                                                  \
  mubst_add(&bst, &p3->node);                                                  \
  mubst_add(&bst, &p4->node);                                                  \
  mubst_add(&bst, &p5->node);                                                  \
  mubst_add(&bst, &p6->node);                                                  \
  mubst_add(&bst, &p7->node);

static int8_t person_keycmp(void const *key1, void const *key2) {
  return strcmp((char *)key1, (char *)key2);
}

struct person {
  char *name;
  uint8_t age;
  struct mubst_node node;
};

struct test {
  char *name;
  int (*test_function)(void);
};

#define D(fname, func) static int func(void);
TEST_TABLE
#undef D

#define D(fname, func) {.name = fname, .test_function = func},
static struct test tests[] = {TEST_TABLE{0}};
#undef D

int main(int argc, char *argv[]) {
  printf("\n======================= bst tests ========================\n");
  size_t i = 0;
  while (tests[i].name != NULL) {
    printf(">>> %s ... ", tests[i].name);
    if (tests[i].test_function() == EXIT_SUCCESS) {
      printf("ok\n");
    } else {
      printf("fail\n");
    }
    i++;
  }
  return 0;
}

static int test_01(void) {
  TEST_BST_INIT();
  int res = EXIT_SUCCESS;
  struct mubst_node *n1 = mubst_find(&bst, "Kenobi");
  if (n1 == NULL) {
    fprintf(stderr, "failed to find %s\n", "Kenobi");
    res = EXIT_FAILURE;
  }
  n1 = mubst_find(&bst, "Grevious");
  if (n1 == NULL) {
    fprintf(stderr, "failed to find %s\n", "Grevious");
    res = EXIT_FAILURE;
  }
  n1 = mubst_find(&bst, "Sogeking");
  if (n1 == NULL) {
    fprintf(stderr, "failed to find %s\n", "Sogeking");
    res = EXIT_FAILURE;
  }
  n1 = mubst_find(&bst, "Rufy");
  if (n1 == NULL) {
    fprintf(stderr, "failed to find %s\n", "Rufy");
    res = EXIT_FAILURE;
  }
  n1 = mubst_find(&bst, "Goku");
  if (n1 == NULL) {
    fprintf(stderr, "failed to find %s\n", "Goku");
    res = EXIT_FAILURE;
  }
  n1 = mubst_find(&bst, "Kenshin");
  if (n1 == NULL) {
    fprintf(stderr, "failed to find %s\n", "Kenshin");
    res = EXIT_FAILURE;
  }
  n1 = mubst_find(&bst, "Spike");
  if (n1 == NULL) {
    fprintf(stderr, "failed to find %s\n", "Spike");
    res = EXIT_FAILURE;
  }
  return res;
}

static int test_rm_leaf(void) {
  TEST_BST_INIT();
  int res = EXIT_SUCCESS;
  mubst_remove(&bst, "Goku");
  struct mubst_node *n1 = mubst_find(&bst, "Kenobi");
  if (n1 == NULL) {
    fprintf(stderr, "failed to find %s\n", "Kenobi");
    res = EXIT_FAILURE;
  }
  n1 = mubst_find(&bst, "Grevious");
  if (n1 == NULL) {
    fprintf(stderr, "failed to find %s\n", "Grevious");
    res = EXIT_FAILURE;
  }
  n1 = mubst_find(&bst, "Sogeking");
  if (n1 == NULL) {
    fprintf(stderr, "failed to find %s\n", "Sogeking");
    res = EXIT_FAILURE;
  }
  n1 = mubst_find(&bst, "Rufy");
  if (n1 == NULL) {
    fprintf(stderr, "failed to find %s\n", "Rufy");
    res = EXIT_FAILURE;
  }
  n1 = mubst_find(&bst, "Goku");
  if (n1 != NULL) {
    fprintf(stderr, "failed not to find %s\n", "Goku");
    res = EXIT_FAILURE;
  }
  n1 = mubst_find(&bst, "Kenshin");
  if (n1 == NULL) {
    fprintf(stderr, "failed to find %s\n", "Kenshin");
    res = EXIT_FAILURE;
  }
  n1 = mubst_find(&bst, "Spike");
  if (n1 == NULL) {
    fprintf(stderr, "failed to find %s\n", "Spike");
    res = EXIT_FAILURE;
  }
  return res;
}

static int test_rm_single(void) {
  TEST_BST_INIT();
  int res = EXIT_SUCCESS;
  mubst_remove(&bst, "Sogeking");
  struct mubst_node *n1 = mubst_find(&bst, "Kenobi");
  if (n1 == NULL) {
    fprintf(stderr, "failed to find %s\n", "Kenobi");
    res = EXIT_FAILURE;
  }
  n1 = mubst_find(&bst, "Grevious");
  if (n1 == NULL) {
    fprintf(stderr, "failed to find %s\n", "Grevious");
    res = EXIT_FAILURE;
  }
  n1 = mubst_find(&bst, "Sogeking");
  if (n1 != NULL) {
    fprintf(stderr, "failed not to find %s\n", "Sogeking");
    res = EXIT_FAILURE;
  }
  n1 = mubst_find(&bst, "Rufy");
  if (n1 == NULL) {
    fprintf(stderr, "failed to find %s\n", "Rufy");
    res = EXIT_FAILURE;
  }
  n1 = mubst_find(&bst, "Goku");
  if (n1 == NULL) {
    fprintf(stderr, "failed to find %s\n", "Goku");
    res = EXIT_FAILURE;
  }
  n1 = mubst_find(&bst, "Kenshin");
  if (n1 == NULL) {
    fprintf(stderr, "failed to find %s\n", "Kenshin");
    res = EXIT_FAILURE;
  }
  n1 = mubst_find(&bst, "Spike");
  if (n1 == NULL) {
    fprintf(stderr, "failed to find %s\n", "Spike");
    res = EXIT_FAILURE;
  }
  return res;
}

static int test_rm_double(void) {
  TEST_BST_INIT();
  int res = EXIT_SUCCESS;
  mubst_remove(&bst, "Grevious");
  struct mubst_node *n1 = mubst_find(&bst, "Kenobi");
  if (n1 == NULL) {
    fprintf(stderr, "failed to find %s\n", "Kenobi");
    res = EXIT_FAILURE;
  }
  n1 = mubst_find(&bst, "Grevious");
  if (n1 != NULL) {
    fprintf(stderr, "failed not to find %s\n", "Grevious");
    res = EXIT_FAILURE;
  }
  n1 = mubst_find(&bst, "Sogeking");
  if (n1 == NULL) {
    fprintf(stderr, "failed to find %s\n", "Sogeking");
    res = EXIT_FAILURE;
  }
  n1 = mubst_find(&bst, "Rufy");
  if (n1 == NULL) {
    fprintf(stderr, "failed to find %s\n", "Rufy");
    res = EXIT_FAILURE;
  }
  n1 = mubst_find(&bst, "Goku");
  if (n1 == NULL) {
    fprintf(stderr, "failed to find %s\n", "Goku");
    res = EXIT_FAILURE;
  }
  n1 = mubst_find(&bst, "Kenshin");
  if (n1 == NULL) {
    fprintf(stderr, "failed to find %s\n", "Kenshin");
    res = EXIT_FAILURE;
  }
  n1 = mubst_find(&bst, "Spike");
  if (n1 == NULL) {
    fprintf(stderr, "failed to find %s\n", "Spike");
    res = EXIT_FAILURE;
  }
  return res;
}

#define VAL_VECTOR_CAPACITY 16
struct val_vector {
  uint8_t vals[VAL_VECTOR_CAPACITY];
  size_t length;
};

static void collect_nodes_val(struct mubst_node *node, void *arg) {
  struct val_vector *v = arg;
  if (v->length == VAL_VECTOR_CAPACITY) {
    return;
  }
  v->vals[v->length++] = *(uint8_t *)node->value;
  return;
}

static int test_traverse(void) {
  TEST_BST_INIT();
  struct val_vector v = {0};
  mubst_apply(&bst, collect_nodes_val, &v);
  uint8_t expected[] = {25, 69, 42, 32, 23, 22, 28};
  int res = EXIT_SUCCESS;
  if (memcmp(expected, v.vals, sizeof(expected) / sizeof(*expected)) != 0) {
    res = EXIT_FAILURE;
  }
  return res;
}
