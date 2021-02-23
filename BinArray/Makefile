#https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html

CC  = g++
CXX = g++
LD  = g++

RM = rm -rf

OPTIM_FLAGS = -O3 -flto
GTEST_FLAGS = -lgtest -pthread -g
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
LD_GCOV_FLAGS = -lgcov

BROWSER = yandex-browser
LCOV_OUT_FOLDER = out

FLAGS  = OPTIM_FLAGS=$(OPTIM_FLAGS)
#FLAGS += GTEST_FLAGS=$(GTEST_FLAGS)
#FLAGS += GCOV_FLAGS=$(GCOV_FLAGS)
#FLAGS += LD_GCOV_FLAGS=$(LD_GCOV_FLAGS)

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
	strip $@

# Test -------------------------

test_build: all $(TEST_DIR)/test.out 
	echo "Test created. Run: ./$(TEST_DIR)/tests.out"

test: test_build
	./$(TEST_DIR)/tests.out

$(TEST_DIR)/test.out:
	$(MAKE) -C $(TEST_DIR)

valgrind:
	valgrind ./main.out

test_valgrind: clean test_build
	valgrind ./$(TEST_DIR)/tests.out

# Coverage ----------------------

view_coverage:
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info -o $(LCOV_OUT_FOLDER)
	$(BROWSER)
	$(BROWSER) $(LCOV_OUT_FOLDER)/index.html

# Clean -------------------------

clean:
	$(RM) main.out $(OBJS) *.gcno *.gcda *.info $(LCOV_OUT_FOLDER)
	$(MAKE) -C $(TEST_DIR) clean
	$(MAKE) -C $(SANDBOX_DIR) clean

.PHONY: clean all tests valgrind
