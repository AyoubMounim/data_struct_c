# Data Structures For c

Collection of simple implementations of various data structures in c.

## Vector

Just add the `muvector.h` header file to your project.

A vector containing elements of a given type must be first declared via the
`MUVECTOR_DECLARE(type)` macro. Afterwards you can create vectors via the
`muvector_init_*` macros.

## Binary Search Tree

Add the `mubst.h` and `mubst.c` files to your project.

## Test

Run `make test` in the repo's root directory to run some basics tests.
