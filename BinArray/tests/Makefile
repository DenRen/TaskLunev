CXX = g++
LD = g++

OPTIM_FLAGS = -O0
GTEST_FLAGS = -lgtest -pthread -g

LDFLAGS  = $(GTEST_FLAGS) $(OPTIM_FLAGS)
CXXFLAGS = $(GTEST_FLAGS) $(OPTIM_FLAGS)

OBJS = main.o ../BinArray.o TestFunc.o

all: tests.out

tests.out: $(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS)

../BinArray.o:
	$(MAKE) -C ../ BinArray.o

clean:
	$(RM) tests.out *.o

.PHONY: clean all ../BinArray.o	# ../BinArray.o because other make
