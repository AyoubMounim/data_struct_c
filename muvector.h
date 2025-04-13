
#pragma once

#include <stddef.h>

#define MUVECTOR_INLINE static inline

#ifndef MUVECTOR_ASSERT
#include <assert.h>
#define MUVECTOR_ASSERT(expr) assert(expr)
#endif

#ifndef MUVECTOR_DEFAULT_CAPACITY
#define MUVECTOR_DEFAULT_CAPACITY 128
#endif

struct muvector_allocator {
  void *ctx;
  void *(*alloc)(void *ctx, size_t const size);
  void *(*realloc)(void *ctx, void *ptr, size_t const size);
  void (*free)(void *ctx, void **ptr);
};

#define MUVECTOR_NOT_IMPLEMENTED(func)                                         \
  MUVECTOR_ASSERT(func && "function not implemented")

#define muvector_allocator_alloc(allocator, size)                              \
  (MUVECTOR_NOT_IMPLEMENTED((allocator)->alloc),                               \
   (allocator)->alloc((allocator)->ctx, size))

#define muvector_allocator_realloc(allocator, ptr, size)                       \
  (MUVECTOR_NOT_IMPLEMENTED((allocator)->realloc),                             \
   (allocator)->realloc((allocator)->ctx, ptr, size))

#define muvector_allocator_free(allocator, ptr)                                \
  (MUVECTOR_NOT_IMPLEMENTED((allocator)->free),                                \
   (allocator)->free((allocator)->ctx, ptr))

#ifdef MUVECTOR_NO_STDLIB
// TODO: implement some sort of std allocator in this case.
static struct muvector_allocator muvector_std_allocator = {0};
#else
#include <stdlib.h>

MUVECTOR_INLINE void *muvector_std_alloc(void *ctx, size_t const size) {
  return malloc(size);
}

MUVECTOR_INLINE void *muvector_std_realloc(void *ctx, void *ptr,
                                           size_t const size) {
  (void)ctx;
  return realloc(ptr, size);
}

MUVECTOR_INLINE void muvector_std_free(void *ctx, void **ptr) {
  (void)ctx;
  free(*ptr);
  *ptr = NULL;
  return;
}

static struct muvector_allocator muvector_std_allocator = {
    .ctx = NULL,
    .alloc = muvector_std_alloc,
    .realloc = muvector_std_realloc,
    .free = muvector_std_free,
};

#endif // MUVECTOR_NO_STDLIB

#define muvector_reset(v)                                                      \
  do {                                                                         \
    (v)->length = 0;                                                           \
  } while (0)

#define muvector_destroy(v)                                                    \
  do {                                                                         \
    if ((v)->allocator) {                                                      \
      (v)->allocator->free((v)->allocator->ctx, (void **)&((v)->items));       \
    }                                                                          \
  } while (0)

#define muvector_append(v, item)                                               \
  do {                                                                         \
    if ((v)->length == (v)->capacity) {                                        \
      if ((v)->allocator == NULL) {                                            \
        MUVECTOR_ASSERT(0 && "muvector full and no allocator available");      \
      }                                                                        \
      if ((v)->capacity == 0)                                                  \
        (v)->capacity = MUVECTOR_DEFAULT_CAPACITY;                             \
      else                                                                     \
        (v)->capacity *= 2;                                                    \
      (v)->items = muvector_allocator_realloc(                                 \
          (v)->allocator, (v)->items, (v)->capacity * sizeof(*((v)->items)));  \
      MUVECTOR_ASSERT((v)->items && "muvector allocator realloc failed");      \
    }                                                                          \
    (v)->items[(v)->length] = item;                                            \
    (v)->length++;                                                             \
  } while (0)

#define MUVECTOR_DEFINE(type)                                                  \
  struct muvector_##type {                                                     \
    type *items;                                                               \
    size_t length;                                                             \
    size_t capacity;                                                           \
    struct muvector_allocator *allocator;                                      \
  }

#define muvector_init_static(v, type, init_capacity)                           \
  struct muvector_##type v = {0};                                              \
  type __darray_buffer_##v[init_capacity];                                     \
  v.items = __darray_buffer_##v;                                               \
  v.capacity = init_capacity;                                                  \
  v.allocator = NULL

#define muvector_init_std(v, type)                                             \
  struct muvector_##type v = {0};                                              \
  v.allocator = &muvector_std_allocator

#undef MUVECTOR_INLINE
