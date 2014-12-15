#ifndef OR1200_1_INT_H
#define OR1200_1_INT_H

// Std Include
#include "incl.h"

// Verilator Include
#include "verilated.h"
#include "bench/or1200_1/cpp/Vtop__Syms.h"
#include "bench/or1200_1/cpp/Vtop.h"

// User Defined Datatypes
#include "vType.h"

const int CONST_NUM_INPUT = 14;
const int CONST_NUM_INPUT_BITS = 144;
const int CONST_NUM_BRANCH = 25;
const int CONST_NUM_VARS = 11;		// TODO
const int CONST_NUM_STATE_BITS = 77;
const int CONST_NUM_CTRL_BITS = 77;	// TODO

void set_input(Vtop*, const vecIn_t&);
void RandomVecIn(vecIn_t&);
void ModifyVecIn(vecIn_t&, const rstIn_t&);

// Simulate Multiple cycles - #Cycles or Input Vector
void SimMultiCycle(Vtop*, int numCycles);
void SimMultiCycle(Vtop*, vecIn_t&);
void SimOneCycle(Vtop*, vecIn_t&);

int GetCoverage(const Vtop*, bool printCnt=false);					// Get #branches covered
int GetCoverage(const Vtop*, int);			// Get branch counter value[index]
void GetCoverage(const Vtop*, vector<int>&);	// Get all index[branch with coverage > 0]
int GetBranchCounters(const Vtop*, vector<int>&);
void GetResetBranch(Vtop*, vector<int>&);	// Get index[reset branch]

void ResetCounters(Vtop*);			// Reset counters
void DisplayCircuit(Vtop*);

void setAllXState(Vtop*);
void printCktState(Vtop*);

// Functions

inline
bool IsDefaultBranch(const int i)
{
    return (i == 23); // reset branch
}

// Circuit Simulation Functions
inline
void init_reset_clock(Vtop *top) 
{       
    top->rst = 1;     // b circuit
    top->clk = 0;
}

inline
void sim_reset_clock(Vtop *top)
{        
    top->rst = 0;     // b circuit
    top->clk = 0;
}

inline
bool start_sim(Vtop *top)
{
    return (top->clk == 0 && top->rst == 0);
}

inline
void ToggleClk(Vtop *top)
{
    top->clk = !top->clk; // Toggle clk 
}


#endif	// OR1200_1_INT_H
