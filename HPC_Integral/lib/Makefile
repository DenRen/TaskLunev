#https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html

CC = g++
LD = g++

OPTIM_FLAGS = -O0

DEBUG_FLAGS = -g
CFLAGS = $(OPTIM_FLAGS) -MD -pthread $(DEBUG_FLAGS)
LDFLAGS = $(CFLAGS) -lm

RM = rm -rf

OBJS = cpu_topology.o hpc.o debug_func.o

# Main --------------------------

all: $(OBJS)

clean:
	$(RM) *.o *.d

-include $(OBJS:.o=.d)

.PHONY: clean all