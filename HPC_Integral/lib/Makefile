#https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html

CC = g++
LD = g++

OPTIM_FLAGS = -Ofast

CFLAGS = $(OPTIM_FLAGS) -MD -pthread
LDFLAGS = $(CFLAGS) -lm

RM = rm -rf

OBJS = cpu_topology.o hpc.o

# Main --------------------------

all: $(OBJS)

clean:
	$(RM) *.o *.d

-include $(OBJS:.o=.d)

.PHONY: clean all