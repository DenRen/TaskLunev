CXX = g++
CC = g++
LD = g++

OPTIM_FLAGS = -O0
GTEST_FLAGS = -lgtest -pthread -g
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
LD_GCOV_FLAGS = -lgcov

CFLAGS   = $(GTEST_FLAGS) $(OPTIM_FLAGS) $(GCOV_FLAGS)
CXXFLAGS = $(GTEST_FLAGS) $(OPTIM_FLAGS) $(GCOV_FLAGS)
LDFLAGS  = $(GTEST_FLAGS) $(OPTIM_FLAGS) $(LD_GCOV_FLAGS)

OBJS = main.o ../BinArray.o TestFunc.o

all: tests.out

tests.out: $(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS)

../BinArray.o:
	$(MAKE) -C ../ BinArray.o

clean:
	$(RM) tests.out *.o *.gcno *.gcda *.info

.PHONY: clean all ../BinArray.o	# ../BinArray.o because other make