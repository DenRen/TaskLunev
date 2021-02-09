CC = gcc
LD = gcc

CFLAGS = -O0 -g

OBJS = main.o BinArray.o
TEST_DIR = tests

# Main -------------------------

all: main.out

main.out: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Test -------------------------

test: all $(TEST_DIR)/test.out
	echo "Test created. Run: ./$(TEST_DIR)/tests.out"

$(TEST_DIR)/test.out:
	$(MAKE) -C $(TEST_DIR) all

# Clean -------------------------

clean:
	$(RM) main.out $(OBJS)
	$(MAKE) -C $(TEST_DIR) clean

.PHONY: clean all tests
