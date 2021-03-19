#https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html

CC = g++
LD = g++

OPTIM_FLAGS = #-Ofast -flto -ffast-math
DEBUG_FLAGS = #-g
WARNING_FLAGS = -Wall -Wcomment

CFLAGS   = $(OPTIM_FLAGS) -MD -pthread $(DEBUG_FLAGS) $(WARNING_FLAGS) -D_GNU_SOURCE
CXXFLAGS = $(CFLAGS)
LDFLAGS  = $(CFLAGS) -lm

RM = rm -rf

LIB_HPC_NET_NAME = hpcnet
LIB_HPC_NET_OBJS = hpc_net.o

# Main --------------------------

all: $(LIB_HPC_NET_OBJS)

lib_hpc_net: build_lib_hpc_net clean_temp

build_lib_hpc_net: all
	$(AR) rc lib$(LIB_HPC_NET_NAME).a $(LIB_HPC_NET_OBJS)

clean: clean_temp
	$(RM) lib$(LIB_HPC_NET_NAME).a

clean_temp:
	$(RM) *.o *.d *.gch

-include $(LIB_HPC_NET_OBJS:.o=.d)

.PHONY: lib_hpc_net build_lib_hpc_net all clean_temp clean