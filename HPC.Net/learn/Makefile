#https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html

CC = g++
LD = g++

RM = rm -rf

OPTIM_FLAGS = #-Ofast -flto -ffast-math
DEBUG_FLAGS = -g
WARNING_FLAGS = -Wall

CFLAGS   = $(OPTIM_FLAGS) -MD -pthread $(DEBUG_FLAGS) $(WARNING_FLAGS) -D_GNU_SOURCE
LDFLAGS  = $(CFLAGS) -lm

LIB_HPC_DIR = ../../HPC_Integral/lib
LIB_HPC_NAME = hpc

LIB_HPC_NET_DIR = lib
LIB_HPC_NET_NAME = hpcnet

SRCS = 1.1.client.c 1.1.server.c

EXEC_FILE = $(SRCS:%.c=%.out)

# Main ----------------------------------------------------

all: $(EXEC_FILE)

%.out: %.c lib_hpc
	$(LD) -o $@ $< $(LDFLAGS) -L$(LIB_HPC_DIR) -l$(LIB_HPC_NAME)

# HPC library ---------------------------------------------

lib_hpc:
	$(MAKE) -C $(LIB_HPC_DIR) build_lib_hpc

# Clean ----------------------------------------------------

clean_lib_hpc:
	$(MAKE) -C $(LIB_HPC_DIR) clean

clean_temp:
	$(RM) *.o *.d *.gch

clean: clean_temp
	$(RM) $(EXEC_FILE)

#-include $(:.o=.d)

.PHONY: all test clean hpc_lib