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
    VL_OUT8(icbiu_cyc_o,0,0);
    VL_OUT8(icbiu_stb_o,0,0);
    VL_OUT8(icbiu_we_o,0,0);
    VL_OUT8(icbiu_sel_o,3,0);
    VL_OUT8(icbiu_cab_o,0,0);
    VL_IN8(icbiu_ack_i,0,0);
    VL_IN8(icbiu_err_i,0,0);
    VL_IN8(ic_en,0,0);
    VL_IN8(icqmem_cycstb_i,0,0);
    VL_IN8(icqmem_ci_i,0,0);
    VL_IN8(icqmem_sel_i,3,0);
    VL_IN8(icqmem_tag_i,3,0);
    VL_OUT8(icqmem_ack_o,0,0);
    VL_OUT8(icqmem_rty_o,0,0);
    VL_OUT8(icqmem_err_o,0,0);
    VL_OUT8(icqmem_tag_o,3,0);
    VL_IN8(spr_cs,0,0);
    VL_IN8(spr_write,0,0);
    VL_OUT(icbiu_dat_o,31,0);
    VL_OUT(icbiu_adr_o,31,0);
    VL_IN(icbiu_dat_i,31,0);
    VL_IN(icqmem_adr_i,31,0);
    VL_OUT(icqmem_dat_o,31,0);
    VL_IN(spr_dat_i,31,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    VL_SIG8(v__DOT__icfsm_biu_read,0,0);
    VL_SIG8(v__DOT__tagcomp_miss,0,0);
    VL_SIG8(v__DOT__icfsm_first_miss_ack,0,0);
    VL_SIG8(v__DOT__or1200_ic_fsm__DOT__state,1,0);
    VL_SIG8(v__DOT__or1200_ic_fsm__DOT__cnt,2,0);
    VL_SIG8(v__DOT__or1200_ic_fsm__DOT__hitmiss_eval,0,0);
    VL_SIG8(v__DOT__or1200_ic_fsm__DOT__load,0,0);
    VL_SIG8(v__DOT__or1200_ic_fsm__DOT__cache_inhibit,0,0);
    VL_SIG(v__DOT__ic_addr,31,0);
    VL_SIG(v__DOT__or1200_ic_fsm__DOT__saved_addr_r,31,0);
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    VL_SIG8(__Vclklast__TOP__clk,0,0);
    VL_SIG8(__Vclklast__TOP__rst,0,0);
    //char	__VpadToAlign70[2];
    
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
    static void	_combo__TOP__5(Vtop__Syms* __restrict vlSymsp);
    static void	_combo__TOP__8(Vtop__Syms* __restrict vlSymsp);
    static void	_eval(Vtop__Syms* __restrict vlSymsp);
    static void	_eval_initial(Vtop__Syms* __restrict vlSymsp);
    static void	_eval_settle(Vtop__Syms* __restrict vlSymsp);
    static void	_initial__TOP__2(Vtop__Syms* __restrict vlSymsp);
    static void	_sequent__TOP__3(Vtop__Syms* __restrict vlSymsp);
    static void	_sequent__TOP__6(Vtop__Syms* __restrict vlSymsp);
    static void	_settle__TOP__1(Vtop__Syms* __restrict vlSymsp);
    static void	_settle__TOP__4(Vtop__Syms* __restrict vlSymsp);
    static void	_settle__TOP__7(Vtop__Syms* __restrict vlSymsp);
    static void	_settle__TOP__9(Vtop__Syms* __restrict vlSymsp);
} VL_ATTR_ALIGNED(128);

#endif  /*guard*/
