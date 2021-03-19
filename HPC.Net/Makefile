#https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html

CC = g++
LD = g++

RM = rm -rf

OPTIM_FLAGS = #-Ofast -flto -ffast-math
DEBUG_FLAGS = -g
WARNING_FLAGS = -Wall

CFLAGS   = $(OPTIM_FLAGS) -MD -pthread $(DEBUG_FLAGS) $(WARNING_FLAGS) -D_GNU_SOURCE
CXXFLAGS = $(CFLAGS)
LDFLAGS  = $(CFLAGS) -lm

LIB_HPC_DIR = ../HPC_Integral/lib
LIB_HPC_NAME = hpc
LIB_HPC_OBJS = cpu_topology.o debug_func.o hpc.o
LIB_HPC_OBJS_PATH = $(LIB_HPC_OBJS:%.o=$(LIB_HPC_DIR)/%.o)

LIB_HPC_NET_DIR = lib
LIB_HPC_NET_NAME = hpcnet
LIB_HPC_NET_OBJS = hpc_net.o #cpu_topology.o debug_func.o 
LIB_HPC_NET_OBJS_PATH = $(LIB_HPC_NET_OBJS:%.o=$(LIB_HPC_NET_DIR)/%.o)

LIB_HPC_NET_TEST_DIR = test

OBJS = main.o
EXEC_FILE = main.out

# Main ----------------------------------------------------

# Required to build the executable file
ALL_EXEC_OBJS = $(OBJS) $(LIB_HPC_NET_OBJS_PATH)

all: $(EXEC_FILE)

$(EXEC_FILE): lib_hpc lib_hpc_net $(OBJS)
	$(LD) -o $@ $(ALL_EXEC_OBJS) $(LDFLAGS)			\
		-L$(LIB_HPC_DIR)	 -l$(LIB_HPC_NAME)		\
		-L$(LIB_HPC_NET_DIR) -l$(LIB_HPC_NET_NAME)
	#strip $@

# HPC libraries -------------------------------------------

lib_hpc_net:
	$(MAKE) -C $(LIB_HPC_NET_DIR) build_lib_hpc_net

lib_hpc:
	$(MAKE) -C $(LIB_HPC_DIR) build_lib_hpc

# HPC test library ----------------------------------------

test:
	$(MAKE) -C $(LIB_HPC_NET_TEST_DIR) start_test

# Clean ----------------------------------------------------

clean_lib_hpc:
	$(MAKE) -C $(LIB_HPC_DIR) clean

clean_temp:
	$(RM) *.o *.d *.gch
	$(MAKE) -C $(LIB_HPC_NET_DIR) clean

clean: clean_temp
	$(RM) $(EXEC_FILE)

-include $(ALL_EXEC_OBJS:.o=.d)

.PHONY: all test clean #hpc_net_lib test_hpc_net_lib