BENCH = or1200_0

DEFINE_BENCH = -D__${BENCH}
BENCH_DIR = ./bench/${BENCH}/obj_dir
LIBS =  ${BENCH_DIR}/Vtop__ALL.a

CXX = g++
CFLAGS = -c -Wall $(DEFINE_BENCH)

SYSTEMPERL = /home/vineeth/Tools/SystemPerl/src
VERILATOR_INCLUDE = /usr/local/share/verilator/include

INCLUDE = -I$(SYSTEMPERL) -I$(VERILATOR_INCLUDE)

DEBUG = -g
#DEBUG = -g -D_DBG_DEST_CALL_
CFLAGS += ${DEBUG}

all: gen

OBJS_TEST = main.o ${BENCH}Int.o \
verilated.o SpCoverage.o
TARGET_MAIN = ${BENCH}TestSim

OBJS_GEN = ${BENCH}Int.o gaLib.o \
newSim.o \
readGraph.o \
verilated.o SpCoverage.o
TARGET_GEN = ${BENCH}Sim
DEFINE_GEN = -D_FuncMode_

test: $(OBJS_TEST)
	$(CXX) $(OBJS_TEST) $(INCLUDE) $(LIBS) -o $(TARGET_MAIN) -lm -lstdc++ 2>&1 | c++filt

gen: $(OBJS_GEN)
	$(CXX) $(OBJS_GEN) $(INCLUDE) $(LIBS) -o $(TARGET_GEN) -lm -lstdc++ 2>&1 | c++filt

# All object files below

main.o:	vType.h incl.h ${BENCH}Int.h main.cpp
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o main.o main.cpp

newSim.o: newSim.cpp verInclude.h incl.h ${BENCH}Int.h ${BENCH}Int.cpp gaLib.h gaLib.cpp 
	$(CXX) $(CFLAGS) $(DEFINE_GEN) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o newSim.o newSim.cpp

gaLib.o: gaLib.cpp gaLib.h incl.h ${BENCH}Int.h ${BENCH}Int.cpp verInclude.h
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o gaLib.o gaLib.cpp

verilated.o:
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0       -DVL_DEBUG=1  -c -o verilated.o ${VERILATOR_INCLUDE}/verilated.cpp

SpCoverage.o:
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0       -DVL_DEBUG=1  -c -o SpCoverage.o ${SYSTEMPERL}/SpCoverage.cpp

${BENCH}Int.o: incl.h ${BENCH}Int.h ${BENCH}Int.cpp vType.h verInclude.h
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o ${BENCH}Int.o ${BENCH}Int.cpp

readGraph.o: readGraph.h readGraph.cpp verInclude.h incl.h
	$(CXX) $(CFLAGS) $(DEFINE_GEN) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o readGraph.o readGraph.cpp

clean:
	rm -f *.o
	rm -f *.d
