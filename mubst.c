
#include "mubst.h"
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static uint8_t default_pool_buffer[MUBST_DEFAULT_POOL_SIZE];
static struct mubst_pool_allocator p_alloc = {
    .buffer = default_pool_buffer,
    .size = 0,
    .capacity = sizeof(default_pool_buffer),
};

static struct mubst_allocator mubst_pool_allocator_instance = {
    .ctx = &p_alloc,
    .alloc = mubst_pool_allocator_alloc,
    .realloc = mubst_pool_allocator_realloc,
    .free = mubst_pool_allocator_free,
};

/* ============================================= Public functions definition */

void mubst_init(struct mubst *self, mubst_keycmp keycmp,
                struct mubst_allocator *allocator) {
  memset(self, 0, sizeof(*self));
  self->keycmp = keycmp;
  if (!allocator) {
    allocator = &mubst_pool_allocator_instance;
  }
  self->allocator = allocator;
  return;
}

void mubst_deinit(struct mubst *self) { return; }

struct mubst *mubst_create(mubst_keycmp keycmp,
                           struct mubst_allocator *allocator) {
  if (!allocator) {
    allocator = &mubst_pool_allocator_instance;
  }
  struct mubst *self = allocator->alloc(allocator, sizeof(struct mubst));
  if (!self) {
    return NULL;
  }
  mubst_init(self, keycmp, allocator);
  return self;
}

void mubst_destroy(struct mubst *self) {
  if (!self) {
    return;
  }
  mubst_deinit(self);
  self->allocator->free(self->allocator, self);
  self = NULL;
  return;
}

void *mubst_alloc(struct mubst *self, size_t const size) {
  return self->allocator->alloc(self->allocator, size);
}

int8_t mubst_add(struct mubst *self, struct mubst_node *node) {
  if (self->nodes == NULL) {
    self->nodes = node;
    node->parent = NULL;
    node->right = node->left = NULL;
    return 0;
  }
  if (self->keycmp == NULL) {
    return -1;
  }
  struct mubst_node *leaf = self->nodes;
  while (true) {
    if (self->keycmp(leaf->key, node->key) > 0) {
      if (leaf->left != NULL) {
        leaf = leaf->left;
      } else {
        leaf->left = node;
        node->parent = leaf;
        node->right = node->left = NULL;
        break;
      }
    } else if (self->keycmp(leaf->key, node->key) < 0) {
      if (leaf->right != NULL) {
        leaf = leaf->right;
      } else {
        leaf->right = node;
        node->parent = leaf;
        node->right = node->left = NULL;
        break;
      }
    } else {
      leaf->value = node->value;
      break;
    }
  }
  return 0;
}

struct mubst_node *mubst_find(struct mubst const *self, void const *key) {
  if (self->nodes == NULL || self->keycmp == NULL) {
    return NULL;
  }
  struct mubst_node *leaf = self->nodes;
  while (leaf) {
    if (self->keycmp(leaf->key, key) > 0) {
      leaf = leaf->left;
    } else if (self->keycmp(leaf->key, key) < 0) {
      leaf = leaf->right;
    } else {
      break;
    }
  }
  return leaf;
}

int8_t mubst_remove(struct mubst *self, void const *key) {
  struct mubst_node *n = mubst_find(self, key);
  if (n == NULL) {
    return -1;
  }
  if (n->left == NULL && n->right == NULL) {
    // leaf case
    if (n->parent != NULL) {
      if (n->parent->left == n) {
        n->parent->left = NULL;
      } else {
        MUBST_ASSERT(n->parent->right == n);
        n->parent->right = NULL;
      }
    } else {
      self->nodes = NULL;
    }
  } else if (n->left == NULL || n->right == NULL) {
    // single child case
    struct mubst_node *c = (n->left) ? n->left : n->right;
    c->parent = n->parent;
    if (n->parent != NULL) {
      if (n->parent->right == n) {
        n->parent->right = c;
      } else {
        MUBST_ASSERT(n->parent->left == n);
        n->parent->left = c;
      }
    } else {
      self->nodes = c;
    }
  } else {
    // two children case
    struct mubst_node *successor = n->right;
    while (true) {
      if (successor->left == NULL) {
        n->value = successor->value;
        n->key = successor->key;
        if (successor->right) {
          successor->right->parent = successor->parent;
        }
        if (successor->parent->left == successor) {
          successor->parent->left = successor->right;
        } else {
          MUBST_ASSERT(successor->parent->right == successor);
          successor->parent->right = successor->right;
        }
        break;
      }
      MUBST_ASSERT(self->keycmp(successor->left->key, n->key) > 0);
      successor = successor->left;
    }
  }
  return 0;
}

void mubst_apply(struct mubst const *self,
                 void (*callback)(struct mubst_node *node, void *arg),
                 void *arg) {
  if (self->nodes == NULL) {
    return;
  }
  struct mubst_node *current = self->nodes;
  struct mubst_node *last_visited = NULL;
  while (current) {
    if (last_visited == current->parent) {
      last_visited = current;
      if (current->left) {
        current = current->left;
      } else {
        callback(current, arg);
        if (current->right) {
          current = current->right;
        } else {
          current = current->parent;
        }
      }
    } else if (last_visited == current->left) {
      last_visited = current;
      callback(current, arg);
      if (current->right) {
        current = current->right;
      } else {
        current = current->parent;
      }
    } else if (last_visited == current->right) {
      last_visited = current;
      current = current->parent;
    }
  }
  return;
}

void *mubst_pool_allocator_alloc(void *ctx, size_t const size) {
  struct mubst_pool_allocator *self = ctx;
  if (self->size + size >= self->capacity) {
    return NULL;
  }
  void *ptr = (void *)((uintptr_t)self->buffer + (uintptr_t)size);
  self->size += size;
  return ptr;
}

void *mubst_pool_allocator_realloc(void *ctx, void *ptr, size_t const size) {
  return NULL;
}

void mubst_pool_allocator_free(void *ctx, void *ptr) {
  (void)ctx;
  ptr = NULL;
  return;
}
