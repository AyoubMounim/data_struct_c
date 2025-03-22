
#pragma once

#include <assert.h>
#include <stdlib.h>

#ifndef MUVECTOR_ALLOC
#include <stdlib.h>
#define MUVECTOR_ALLOC(size) malloc(size)
#endif

#ifndef MUVECTOR_REALLOC
#include <stdlib.h>
#define MUVECTOR_REALLOC(ptr, size) realloc(ptr, size)
#endif

#ifndef MUVECTOR_FREE
#include <stdlib.h>
#define MUVECTOR_FREE(ptr) free(ptr)
#endif

#ifndef MUVECTOR_ASSERT
#include <assert.h>
#define MUVECTOR_ASSERT(expr) assert(expr)
#endif

#ifndef MUVECTOR_DEFAULT_CAPACITY
#define MUVECTOR_DEFAULT_CAPACITY 128
#endif

#define MUVECTOR_RESET(v)                                                      \
  do {                                                                         \
    (v)->length = 0;                                                           \
  } while (0)

#define MUVECTOR_DESTROY(v)                                                    \
  do {                                                                         \
    MUVECTOR_FREE((v)->items);                                                 \
    (v)->length = 0;                                                           \
    (v)->capacity = 0;                                                         \
  } while (0)

#define MUVECTOR_APPEND(v, item)                                               \
  do {                                                                         \
    if ((v)->length == (v)->capacity) {                                        \
      if ((v)->capacity == 0)                                                  \
        (v)->capacity = MUVECTOR_DEFAULT_CAPACITY;                             \
      else                                                                     \
        (v)->capacity *= 2;                                                    \
      (v)->items =                                                             \
          MUVECTOR_REALLOC((v)->items, (v)->capacity * sizeof(*((v)->items))); \
      assert((v)->items && "muvector realloc failed");                         \
    }                                                                          \
    (v)->items[(v)->length] = item;                                            \
    (v)->length++;                                                             \
  } while (0)
