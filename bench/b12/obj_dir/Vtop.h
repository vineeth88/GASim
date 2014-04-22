// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _Vtop_H_
#define _Vtop_H_

#include "verilated.h"
#include "SpCoverage.h"
class Vtop__Syms;

//----------

VL_MODULE(Vtop) {
  public:
    // CELLS
    // Public to allow access to /*verilator_public*/ items;
    // otherwise the application code can consider these internals.
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clock,0,0);
    VL_IN8(reset,0,0);
    VL_IN8(start,0,0);
    VL_IN8(k,3,0);
    VL_OUT8(nloss,0,0);
    VL_OUT8(nl,3,0);
    VL_OUT8(speaker,0,0);
    //char	__VpadToAlign7[1];
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    VL_SIG8(v__DOT__wr,0,0);
    VL_SIG8(v__DOT__address,4,0);
    VL_SIG8(v__DOT__data_in,1,0);
    VL_SIG8(v__DOT__data_out,1,0);
    VL_SIG8(v__DOT__num,1,0);
    VL_SIG8(v__DOT__sound,2,0);
    VL_SIG8(v__DOT__play,0,0);
    VL_SIG8(v__DOT__s,0,0);
    VL_SIG8(v__DOT__counter,2,0);
    VL_SIG8(v__DOT__count,1,0);
    VL_SIG8(v__DOT__gamma,4,0);
    VL_SIG8(v__DOT__ind,1,0);
    VL_SIG8(v__DOT__scan,4,0);
    VL_SIG8(v__DOT__max,4,0);
    VL_SIG8(v__DOT__timebase,5,0);
    VL_SIG8(v__DOT__count2,5,0);
    //char	__VpadToAlign28[4];
    VL_SIG8(v__DOT__memory[32],1,0);
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    VL_SIG8(__Vclklast__TOP__clock,0,0);
    //char	__VpadToAlign69[3];
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    //char	__VpadToAlign76[4];
    Vtop__Syms*	__VlSymsp;		// Symbol table
  private:
    // Coverage
    void __vlCoverInsert(uint32_t* countp, bool enable, const char* filenamep, int lineno, int column,
	const char* hierp, const char* pagep, const char* commentp);
    
    // PARAMETERS
    // Parameters marked /*verilator public*/ for use by application code
  public:
    
    // CONSTRUCTORS
  private:
    Vtop& operator= (const Vtop&);	///< Copying not allowed
    Vtop(const Vtop&);	///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible WRT DPI scope names.
    Vtop(const char* name="TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vtop();
    
    // USER METHODS
    
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval();
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    
    // INTERNAL METHODS
  private:
    static void _eval_initial_loop(Vtop__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vtop__Syms* symsp, bool first);
  private:
    static IData	_change_request(Vtop__Syms* __restrict vlSymsp);
  public:
    static void	_eval(Vtop__Syms* __restrict vlSymsp);
    static void	_eval_initial(Vtop__Syms* __restrict vlSymsp);
    static void	_eval_settle(Vtop__Syms* __restrict vlSymsp);
    static void	_sequent__TOP__1(Vtop__Syms* __restrict vlSymsp);
} VL_ATTR_ALIGNED(128);

#endif  /*guard*/
