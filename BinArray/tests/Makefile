CXX = g++
LD = g++

TEST_FLAGS = -lgtest -pthread

LDFLAGS = $(TEST_FLAGS)
CXXFLAGS = $(TEST_FLAGS)

OBJS = main.o BinArray_Tests.o

all: tests.out

tests.out: $(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS)
	
clean:
	$(RM) tests.out *.o

.PHONY: clean
