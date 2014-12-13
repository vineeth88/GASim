BENCH = b01
DEFINE_BENCH = -D__${BENCH}
BENCH_DIR = ./bench/${BENCH}/obj_dir
LIBS =  ${BENCH_DIR}/Vtop__ALL.a

CXX = g++
CFLAGS = -c -Wall $(DEFINE_BENCH)

SYSTEMPERL = /home/vineeth/Tools/SystemPerl/src
VERILATOR_INCLUDE = /usr/local/share/verilator/include

INCL_DIR = ./include
SRC_DIR = ./src
EXEC_DIR = ./exec
INCLUDE = -I$(SYSTEMPERL) -I$(VERILATOR_INCLUDE) -I$(INCL_DIR)

DEBUG = -g -O0
#DEBUG = -g -D_DBG_DEST_CALL_
CFLAGS += ${DEBUG}

all: gen

OBJS_TEST = testSim.o ${BENCH}Int.o \
verilated.o SpCoverage.o
TARGET_MAIN = ${BENCH}TestSim

OBJS_GEN = ${BENCH}Int.o gaLib.o \
newSim.o \
graphLib.o \
verilated.o SpCoverage.o
TARGET_GEN = ${BENCH}Sim
#DEFINE_GEN = -D_STAGE1_OLD_FN_

test: $(OBJS_TEST)
	$(CXX) $(OBJS_TEST) $(INCLUDE) $(LIBS) -o $(EXEC_DIR)/$(TARGET_MAIN) -lm -lstdc++ 2>&1 | c++filt

gen: $(OBJS_GEN)
	$(CXX) $(OBJS_GEN) $(INCLUDE) $(LIBS) -o $(EXEC_DIR)/$(TARGET_GEN) -pg -lm -lstdc++ 2>&1 | c++filt

# All object files below

testSim.o:	$(INCL_DIR)/vType.h $(INCL_DIR)/incl.h $(INCL_DIR)/${BENCH}Int.h $(SRC_DIR)/testSim.cpp
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o testSim.o $(SRC_DIR)/testSim.cpp

newSim.o: $(INCL_DIR)/verInclude.h $(INCL_DIR)/incl.h $(INCL_DIR)/${BENCH}Int.h $(INCL_DIR)/gaLib.h $(SRC_DIR)/newSim.cpp $(SRC_DIR)/${BENCH}Int.cpp $(SRC_DIR)/gaLib.cpp 
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o newSim.o $(SRC_DIR)/newSim.cpp

gaLib.o: $(SRC_DIR)/gaLib.cpp $(INCL_DIR)/gaLib.h $(INCL_DIR)/incl.h $(INCL_DIR)/${BENCH}Int.h $(SRC_DIR)/${BENCH}Int.cpp $(INCL_DIR)/verInclude.h
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o gaLib.o $(SRC_DIR)/gaLib.cpp

verilated.o:
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o verilated.o ${VERILATOR_INCLUDE}/verilated.cpp

SpCoverage.o:
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o SpCoverage.o ${SYSTEMPERL}/SpCoverage.cpp

${BENCH}Int.o: $(INCL_DIR)/incl.h $(INCL_DIR)/${BENCH}Int.h $(SRC_DIR)/${BENCH}Int.cpp $(INCL_DIR)/vType.h $(INCL_DIR)/verInclude.h
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o ${BENCH}Int.o $(SRC_DIR)/${BENCH}Int.cpp

graphLib.o: $(SRC_DIR)/graphLib.cpp $(INCL_DIR)/graphLib.h $(INCL_DIR)/verInclude.h $(INCL_DIR)/incl.h $(INCL_DIR)/vType.h
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o graphLib.o $(SRC_DIR)/graphLib.cpp

clean:
	rm -f *.o
	rm -f *.d
