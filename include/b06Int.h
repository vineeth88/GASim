#ifndef B06_INT_H
#define B06_INT_H

// Std Include
#include "incl.h"

// Verilator Include
#include "verilated.h"
#include "bench/b06/obj_dir/Vtop__Syms.h"
#include "bench/b06/obj_dir/Vtop.h"

// User Defined Datatypes
#include "vType.h"

const int CONST_NUM_INPUT = 2;
const int CONST_NUM_INPUT_BITS = 2;
const int CONST_NUM_BRANCH = 24;
const int CONST_NUM_VARS = 1;
const int CONST_NUM_STATE_BITS = 3;

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
    return (i == 22); // Default branch
}

// Circuit Simulation Functions
inline
void init_reset_clock(Vtop *top) 
{       
    top->reset = 1;     // b circuit
    top->clock = 0;
}

inline
void sim_reset_clock(Vtop *top)
{        
    top->reset = 0;     // b circuit
    top->clock = 0;
}

inline
bool start_sim(Vtop *top)
{
    return (top->clock == 0 && top->reset == 0);
}

inline
void ToggleClk(Vtop *top)
{
    top->clock = !top->clock; // Toggle clock 
}


#endif	// B06_INT_H
