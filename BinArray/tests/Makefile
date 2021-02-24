CXX = g++
CC  = g++
LD  = g++

OPTIM_FLAGS = -Os -flto
GTEST_FLAGS = -lgtest -pthread -g
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
LD_GCOV_FLAGS = -lgcov
OTHER_FLAGS = -MD

CFLAGS   = $(GTEST_FLAGS) $(OPTIM_FLAGS) $(GCOV_FLAGS) $(OTHER_FLAGS)
CXXFLAGS = $(GTEST_FLAGS) $(OPTIM_FLAGS) $(GCOV_FLAGS) $(OTHER_FLAGS)
LDFLAGS  = $(GTEST_FLAGS) $(OPTIM_FLAGS) $(LD_GCOV_FLAGS) $(OTHER_FLAGS)

OBJS = main.o ../BinArray.o TestFunc.o

all: tests.out

tests.out: $(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS)
	strip $@

../BinArray.o:
	$(MAKE) -C ../ BinArray.o

clean:
	$(RM) tests.out *.o *.gcno *.gcda *.info *.d

-include $(OBJS:.o=.d)

.PHONY: clean all ../BinArray.o	# ../BinArray.o because other make