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
    VL_IN8(clk,0,0);
    VL_IN8(rst,0,0);
    VL_IN8(wb_clk_i,0,0);
    VL_IN8(wb_rst_i,0,0);
    VL_IN8(clmode,1,0);
    VL_IN8(wb_ack_i,0,0);
    VL_IN8(wb_err_i,0,0);
    VL_IN8(wb_rty_i,0,0);
    VL_OUT8(wb_cyc_o,0,0);
    VL_OUT8(wb_stb_o,0,0);
    VL_OUT8(wb_we_o,0,0);
    VL_OUT8(wb_sel_o,3,0);
    VL_OUT8(wb_cab_o,0,0);
    VL_IN8(biu_cyc_i,0,0);
    VL_IN8(biu_stb_i,0,0);
    VL_IN8(biu_we_i,0,0);
    VL_IN8(biu_cab_i,0,0);
    VL_IN8(biu_sel_i,3,0);
    VL_OUT8(biu_ack_o,0,0);
    VL_OUT8(biu_err_o,0,0);
    VL_IN(wb_dat_i,31,0);
    VL_OUT(wb_adr_o,31,0);
    VL_OUT(wb_dat_o,31,0);
    VL_IN(biu_dat_i,31,0);
    VL_IN(biu_adr_i,31,0);
    VL_OUT(biu_dat_o,31,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    VL_SIG8(v__DOT__valid_div,1,0);
    VL_SIG8(v__DOT__aborted,0,0);
    VL_SIG8(v__DOT__aborted_r,0,0);
    //char	__VpadToAlign51[1];
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    VL_SIG8(__Vclklast__TOP__wb_clk_i,0,0);
    VL_SIG8(__Vclklast__TOP__wb_rst_i,0,0);
    VL_SIG8(__Vclklast__TOP__clk,0,0);
    VL_SIG8(__Vclklast__TOP__rst,0,0);
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
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
    static void	_combo__TOP__1(Vtop__Syms* __restrict vlSymsp);
    static void	_combo__TOP__5(Vtop__Syms* __restrict vlSymsp);
    static void	_eval(Vtop__Syms* __restrict vlSymsp);
    static void	_eval_initial(Vtop__Syms* __restrict vlSymsp);
    static void	_eval_settle(Vtop__Syms* __restrict vlSymsp);
    static void	_sequent__TOP__3(Vtop__Syms* __restrict vlSymsp);
    static void	_sequent__TOP__4(Vtop__Syms* __restrict vlSymsp);
} VL_ATTR_ALIGNED(128);

#endif  /*guard*/
