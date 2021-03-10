#https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html

CC  = g++
CXX = g++
LD  = g++

RM = rm -rf

OPTIM_FLAGS = -O3
GTEST_FLAGS = -lgtest -pthread -g 
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
LD_GCOV_FLAGS = -flto -lgcov

CFLAGS 	 = $(OPTIM_FLAGS) $(GTEST_FLAGS) $(GCOV_FLAGS)
CXXFLAGS = $(OPTIM_FLAGS) $(GTEST_FLAGS) $(GCOV_FLAGS)
LDFLAGS  = $(LD_GCOV_FLAGS)

OBJS = main.o BinArray.o

TEST_DIR = tests
SANDBOX_DIR = sandbox
WRAPPER_DIR = wrapper

# Main -------------------------

all: main.out
	echo $(FLAGS)

main.out: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Clean -------------------------

clean:
	$(RM) main.out $(OBJS) *.gcno *.gcda *.info

.PHONY: clean all
