
#pragma once

#include <stddef.h>
#include <stdint.h>

#ifndef MUBST_ASSERT
#include <assert.h>
#define MUBST_ASSERT(expr) assert(expr)
#endif

#ifndef MUBST_DEFAULT_POOL_SIZE
#define MUBST_DEFAULT_POOL_SIZE 1024 * 4
#endif

typedef int8_t (*mubst_keycmp)(void const *key1, void const *key2);

struct mubst_allocator {
  void *ctx;
  void *(*alloc)(void *ctx, size_t const size);
  void *(*realloc)(void *ctx, void *ptr, size_t const size);
  void (*free)(void *ctx, void *ptr);
};

struct mubst {
  mubst_keycmp keycmp;
  struct mubst_node *nodes;
  struct mubst_allocator *allocator;
};

struct mubst_node {
  void *key;
  void *value;
  void *container;
  struct mubst_node *parent;
  struct mubst_node *right;
  struct mubst_node *left;
};

struct mubst_pool_allocator {
  void *buffer;
  size_t size;
  size_t capacity;
};

/* ============================================ Public functions declaration */

void mubst_init(struct mubst *self, mubst_keycmp keycmp,
                struct mubst_allocator *allocator);

void mubst_deinit(struct mubst *self);

struct mubst *mubst_create(mubst_keycmp keycmp,
                           struct mubst_allocator *allocator);

void mubst_destroy(struct mubst *self);

void *mubst_alloc(struct mubst *self, size_t const size);

void mubst_free(struct mubst *self, struct mubst_node *node);

int8_t mubst_add(struct mubst *self, struct mubst_node *node);

int8_t mubst_remove(struct mubst *self, void const *key);

struct mubst_node *mubst_find(struct mubst const *self, void const *key);

void mubst_apply(struct mubst const *self,
                 void (*callback)(struct mubst_node *node, void *arg),
                 void *arg);

void *mubst_pool_allocator_alloc(void *ctx, size_t const size);

void *mubst_pool_allocator_realloc(void *ctx, void *ptr, size_t const size);

void mubst_pool_allocator_free(void *ctx, void *ptr);
