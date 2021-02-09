CC = gcc
LD = gcc

CFLAGS = -O0 -g

OBJS = main.o BinArray.o

all: main.out

main.out: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f main.out $(OBJS)

.PHONY: clean all
