CC  = g++
CXX = g++
LD  = g++

OPTIM_FLAGS = -O0

CGLAGS 	 = $(OPTIM_FLAGS)
CXXFLAGS = $(OPTIM_FLAGS)
LDFLAGS  = $(OPTIM_FLAGS)

OBJS = main.o binlib.o Tutorials.o ../BinArray.o

all: main.out

main.out: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

clean:
	$(RM) *.o main.out

valgrind: main.out
	valgring main.out

.PHONY: all clean valgrind
