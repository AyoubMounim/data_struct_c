
#pragma once

#include <stdint.h>

typedef int8_t (*mubst_keycmp)(void const *key1, void const *key2);

struct mubst {
  mubst_keycmp keycmp;
  struct mubst_node *nodes;
};

struct mubst_node {
  void *key;
  void *value;
  struct mubst_node *parent;
  struct mubst_node *right;
  struct mubst_node *left;
};

/* ============================================ Public functions declaration */

void mubst_init(struct mubst *self, mubst_keycmp keycmp);

void mubst_deinit(struct mubst *self);

int8_t mubst_add(struct mubst *self, struct mubst_node *node);

int8_t mubst_remove(struct mubst *self, void const *key);

struct mubst_node *mubst_find(struct mubst const *self, void const *key);

void mubst_apply(struct mubst const *self,
                 void (*callback)(struct mubst_node *node, void *arg),
                 void *arg);
