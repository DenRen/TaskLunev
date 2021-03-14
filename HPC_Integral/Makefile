#https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html

CC = g++
LD = g++

RM = rm -rf

OPTIM_FLAGS = -Ofast -flto -ffast-math
DEBUG_FLAGS = #-g
WARNING_FLAGS = -Wall

CFLAGS   = $(OPTIM_FLAGS) -MD -pthread $(DEBUG_FLAGS) $(WARNING_FLAGS) -D_GNU_SOURCE
CXXFLAGS = $(CFLAGS)
LDFLAGS  = $(CFLAGS) -lm

LIB_HPC_DIR = lib
LIB_HPC_OBJS = cpu_topology.o debug_func.o hpc.o
LIB_HPC_OBJS_PATH = $(LIB_HPC_OBJS:%.o=$(LIB_HPC_DIR)/%.o)

LIB_HPC_TEST_DIR = test

OBJS = main.o
EXEC_FILE = main.out

# Main ----------------------------------------------------

# Required to build the executable file
ALL_EXEC_OBJS = $(OBJS) $(LIB_HPC_OBJS_PATH)

all: $(EXEC_FILE)

$(EXEC_FILE): hpc_lib $(OBJS)
	$(LD) -o $@ $(ALL_EXEC_OBJS) $(LDFLAGS)
	strip $@

# HPC library ---------------------------------------------

hpc_lib:
	$(MAKE) -C $(LIB_HPC_DIR) all

# HPC test library ----------------------------------------

test:
	$(MAKE) -C $(LIB_HPC_TEST_DIR) start_test

# Clean ----------------------------------------------------

clean_temp:
	$(RM) *.o *.d *.gch
	$(MAKE) -C $(LIB_HPC_DIR) 	   clean
	$(MAKE) -C $(LIB_HPC_TEST_DIR) clean

clean: clean_temp
	$(RM) $(EXEC_FILE)

-include $(ALL_EXEC_OBJS:.o=.d)

.PHONY: all hpc_lib test_hpc_lib test clean