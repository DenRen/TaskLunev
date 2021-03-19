#https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html

CC = g++
LD = g++

OPTIM_FLAGS = -Ofast -flto -ffast-math
DEBUG_FLAGS = #-g
WARNING_FLAGS = #-Wall

CFLAGS   = $(OPTIM_FLAGS) -MD -pthread $(DEBUG_FLAGS) $(WARNING_FLAGS) -D_GNU_SOURCE
CXXFLAGS = $(CFLAGS)
LDFLAGS  = $(CFLAGS) -lm

RM = rm -rf

LIB_HPC_NAME = hpc
LIB_HPC_OBJS = cpu_topology.o debug_func.o hpc.o

# Main --------------------------------

all: $(LIB_HPC_OBJS)

lib_hpc: build_lib_hpc clean_temp

build_lib_hpc: all
	$(AR) rc lib$(LIB_HPC_NAME).a $(LIB_HPC_OBJS)

# Clean -------------------------------

clean: clean_temp
	$(RM) *.a

clean_temp:
	$(RM) *.o *.d *.gch

-include $(LIB_HPC_OBJS:.o=.d)

.PHONY: all lib_hpc clean_temp clean