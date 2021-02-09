#https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html

CC = g++
LD = g++

OPTIM_FLAGS = -O0
TEST_FLAGS  = -g

CFLAGS 	 = $(OPTIM_FLAGS) $(TEST_FLAGS)
CXXFLAGS = $(OPTIM_FLAGS) $(TEST_FLAGS)

OBJS = main.o BinArray.o
TEST_DIR = tests

# Main -------------------------

all: main.out

main.out: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Test -------------------------

test_build: all $(TEST_DIR)/test.out 
	echo "Test created. Run: ./$(TEST_DIR)/tests.out"

test: test_build
	./$(TEST_DIR)/tests.out

$(TEST_DIR)/test.out:
	$(MAKE) -C $(TEST_DIR) all

valgrind:
	valgrind ./main.out

test_valgrind: clean test
	valgrind ./$(TEST_DIR)/tests.out

# Clean -------------------------

clean:
	$(RM) main.out $(OBJS)
	$(MAKE) -C $(TEST_DIR) clean

.PHONY: clean all tests valgrind
