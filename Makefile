
CC=gcc
CFLAGS = -ggdb
BUILD_DIR=build
TEST_BUILD_DIR=$(BUILD_DIR)/test
TEST_SRC_DIR=test

.PHONY: test all

all: $(TEST_BUILD_DIR)/vector $(TEST_BUILD_DIR)/bst


$(TEST_BUILD_DIR)/vector: $(TEST_BUILD_DIR)/vector.o
	$(CC) $(CFLAGS) -o $@ $< 

$(TEST_BUILD_DIR)/vector.o: $(TEST_SRC_DIR)/vector.c muvector.h test_build_dir
	$(CC) $(CFLAGS) -c -o $@ $<

$(TEST_BUILD_DIR)/bst: $(TEST_BUILD_DIR)/bst.o $(TEST_BUILD_DIR)/mubst.o
	$(CC) $(CFLAGS) -o $@ $^ 

$(TEST_BUILD_DIR)/bst.o: $(TEST_SRC_DIR)/bst.c mubst.h test_build_dir
	$(CC) $(CFLAGS) -c -o $@ $<

$(TEST_BUILD_DIR)/mubst.o: mubst.c mubst.h
	$(CC) $(CFLAGS) -c -o $@ $<

test_build_dir:
	mkdir -p $(TEST_BUILD_DIR)

test: $(TEST_BUILD_DIR)/vector $(TEST_BUILD_DIR)/bst
	@$(TEST_BUILD_DIR)/vector
	@$(TEST_BUILD_DIR)/bst
