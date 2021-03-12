#https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html

CC = g++
LD = g++

OPTIM_FLAGS = -Os -flto
DEBUG_FLAGS = #-g

CFLAGS = $(OPTIM_FLAGS) -MD -pthread $(DEBUG_FLAGS) -D_GNU_SOURCE
CXXFLAGS = $(CFLAGS)
LDFLAGS = $(CFLAGS) -lm

RM = rm -rf

OBJS = cpu_topology.o debug_func.o hpc.o

# Main --------------------------

all: $(OBJS)

clean:
	$(RM) *.o *.d

-include $(OBJS:.o=.d)

.PHONY: clean all