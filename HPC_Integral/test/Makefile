CXX = g++
CC  = g++
LD  = g++

RM = rm -rf

OPTIM_FLAGS = -O3 -flto
GTEST_FLAGS = -lgtest -pthread -g
#GCOV_FLAGS = -fprofile-arcs -ftest-coverage
#LD_GCOV_FLAGS = -lgcov
OTHER_FLAGS = -MD -lm

CFLAGS   = $(GTEST_FLAGS) $(OPTIM_FLAGS) $(GCOV_FLAGS) $(OTHER_FLAGS)
CXXFLAGS = $(GTEST_FLAGS) $(OPTIM_FLAGS) $(GCOV_FLAGS) $(OTHER_FLAGS)
LDFLAGS  = $(GTEST_FLAGS) $(OPTIM_FLAGS) $(LD_GCOV_FLAGS) $(OTHER_FLAGS)

LIB_HPC_DIR = ../lib
LIB_HPC_OBJS = hpc.o cpu_topology.o
LIB_HPC_OBJS_PATH = $(LIB_HPC_OBJS:%.o=$(LIB_HPC_DIR)/%.o)

OBJS = main.o test.o
EXEC_FILE = test.out

ALL_EXEC_OBJS = $(OBJS) $(LIB_HPC_OBJS_PATH)

all: $(EXEC_FILE)

$(EXEC_FILE): $(OBJS) hpc_lib

	$(LD) -o $@ $(ALL_EXEC_OBJS) $(LDFLAGS)
	#strip $@
hpc_lib:
	$(MAKE) -C $(LIB_HPC_DIR)

start_test: $(EXEC_FILE)
	./$(EXEC_FILE)

clean:
	$(RM) $(EXEC_FILE) $(OBJS) $(OBJS:.o=.d)

-include $(OBJS:.o=.d)

.PHONY: all hpc_lib start_test clean