#https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html

CC = g++
CXX = g++
LD = g++

OPTIM_FLAGS = -O0
GTEST_FLAGS = -lgtest -pthread -g
#GCOV_FLAGS = -fprofile-arcs -ftest-coverage
LD_GCOV_FLAGS = -lgcov

CFLAGS 	 = $(OPTIM_FLAGS) $(GTEST_FLAGS) $(GCOV_FLAGS)
CXXFLAGS = $(OPTIM_FLAGS) $(GTEST_FLAGS) $(GCOV_FLAGS)
LDFLAGS  = $(LD_GCOV_FLAGS)

OBJS = main.o BinArray.o
TEST_DIR = tests
SANDBOX_DIR = sandbox

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

test_valgrind: clean test_build
	valgrind ./$(TEST_DIR)/tests.out

# Clean -------------------------

clean:
	$(RM) main.out $(OBJS) *.gcno *.gcda *.info
	$(MAKE) -C $(TEST_DIR) clean
	$(MAKE) -C $(SANDBOX_DIR) clean

.PHONY: clean all tests valgrind