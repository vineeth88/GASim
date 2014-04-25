BENCH = b06

DEFINE_BENCH = -D__${BENCH}
BENCH_DIR = ./bench/${BENCH}/obj_dir
LIBS =  ${BENCH_DIR}/Vtop__ALL.a

CXX = g++
CFLAGS = -c -Wall $(DEFINE_BENCH)
# LM = -lm -ldl -rdynamic -lreadline -ltermcap

SYSTEMPERL = /home/vineeth/Tools/SystemPerl/src
VERILATOR_INCLUDE = /usr/local/share/verilator/include

INCLUDE = -I$(SYSTEMPERL) -I$(VERILATOR_INCLUDE)

DEBUG = -g 

CFLAGS += ${DEBUG}

all: gen

OBJS_MAIN = main.o ${BENCH}Int.o \
verilated.o SpCoverage.o
TARGET_MAIN = ${BENCH}Main

OBJS_TMP = ${BENCH}Int.o tmpSim.o \
readGraph.o \
verilated.o SpCoverage.o
TARGET_TMP = tmpSim
DEFINE_TMP = -D_ResetMode_

OBJS_GEN = ${BENCH}Int.o gaLib.o \
newSim.o \
verilated.o SpCoverage.o
TARGET_GEN = ${BENCH}Sim
DEFINE_GEN = -D_FuncMode_

OBJS_TEST = ${BENCH}Int.o testSim.o \
verilated.o SpCoverage.o
TARGET_TEST = ${BENCH}TestSim
DEFINE_TEST = -D_TestMode_

OBJS_RG = ${BENCH}Int.o readGraph.o \
verilated.o SpCoverage.o
TARGET_RG = ${BENCH}Graph
DEFINE_RG = -D_RdGraph_

OBJS_RC = ${BENCH}Int.o readConstr.o \
verilated.o SpCoverage.o
TARGET_RC = ${BENCH}Const
DEFINE_RC = -D_RdConstr_

main: $(OBJS_MAIN)
	$(CXX) $(OBJS_MAIN) $(INCLUDE) $(LIBS) -o $(TARGET_MAIN) -lm -lstdc++ 2>&1 | c++filt

main.o:	vType.h incl.h ${BENCH}Int.h main.cpp
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o main.o main.cpp

gen: $(OBJS_GEN)
	$(CXX) $(OBJS_GEN) $(INCLUDE) $(LIBS) -o $(TARGET_GEN) -lm -lstdc++ 2>&1 | c++filt

verilated.o:
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0       -DVL_DEBUG=1  -c -o verilated.o ${VERILATOR_INCLUDE}/verilated.cpp

SpCoverage.o:
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0       -DVL_DEBUG=1  -c -o SpCoverage.o ${SYSTEMPERL}/SpCoverage.cpp

${BENCH}Int.o: incl.h ${BENCH}Int.h ${BENCH}Int.cpp vType.h verInclude.h
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o ${BENCH}Int.o ${BENCH}Int.cpp

newSim.o: newSim.cpp verInclude.h incl.h ${BENCH}Int.h ${BENCH}Int.cpp gaLib.h gaLib.cpp 
#newSim.o: T0330.cpp verInclude.h incl.h ${BENCH}Int.h ${BENCH}Int.cpp gaLib.h gaLib.cpp 
	$(CXX) $(CFLAGS) $(DEFINE_GEN) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o newSim.o newSim.cpp

testSim.o: newGASim.cpp verInclude.h incl.h ${BENCH}Int.h ${BENCH}Int.cpp 
	$(CXX) $(CFLAGS) $(DEFINE_TEST) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o testSim.o newSim.cpp

gaLib.o: gaLib.cpp gaLib.h incl.h ${BENCH}Int.h ${BENCH}Int.cpp verInclude.h
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o gaLib.o gaLib.cpp

clean:
	rm -f *.o
	rm -f *.d

test: $(OBJS_TEST)
	$(CXX) $(OBJS_TEST) $(INCLUDE) $(LIBS) -o $(TARGET_TEST) -lm -lstdc++ 2>&1 | c++filt

constr: $(OBJS_RC)
	CFLAGS += $(DEFINE_RC)
	$(CXX) $(OBJS_RC) $(INCLUDE) $(LIBS) -o $(TARGET_RC) -lm -lstdc++ 2>&1 | c++filt	

readConstr.o: readConstr.cpp incl.h ${BENCH}Int.h ${BENCH}Int.cpp
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o readConstr.o readConstr.cpp
 
tmp: $(OBJS_TMP)
	$(CXX) $(OBJS_TMP) $(INCLUDE) $(LIBS) -o $(TARGET_TMP) -lm -lstdc++ 2>&1 | c++filt

tmpSim.o: verInclude.h incl.h ${BENCH}Int.h ${BENCH}Int.cpp tmpSim.cpp
	$(CXX) $(CFLAGS) $(DEFINE_TMP) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o tmpSim.o tmpSim.cpp

graph: $(OBJS_RG)
#	CFLAGS += $(DEFINE_RG)
	$(CXX) $(OBJS_RG) $(INCLUDE) $(LIBS) -o $(TARGET_RG) -lm -lstdc++ 2>&1 | c++filt

