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
    VL_IN8(sig_ibuserr,0,0);
    VL_IN8(sig_dbuserr,0,0);
    VL_IN8(sig_illegal,0,0);
    VL_IN8(sig_align,0,0);
    VL_IN8(sig_range,0,0);
    VL_IN8(sig_dtlbmiss,0,0);
    VL_IN8(sig_dmmufault,0,0);
    VL_IN8(sig_int,0,0);
    VL_IN8(sig_syscall,0,0);
    VL_IN8(sig_trap,0,0);
    VL_IN8(sig_itlbmiss,0,0);
    VL_IN8(sig_immufault,0,0);
    VL_IN8(sig_tick,0,0);
    VL_IN8(branch_taken,0,0);
    VL_IN8(genpc_freeze,0,0);
    VL_IN8(id_freeze,0,0);
    VL_IN8(ex_freeze,0,0);
    VL_IN8(wb_freeze,0,0);
    VL_IN8(if_stall,0,0);
    VL_IN8(epcr_we,0,0);
    VL_IN8(eear_we,0,0);
    VL_IN8(esr_we,0,0);
    VL_IN8(pc_we,0,0);
    VL_IN8(sr_we,0,0);
    VL_OUT8(flushpipe,0,0);
    VL_OUT8(extend_flush,0,0);
    VL_OUT8(except_type,3,0);
    VL_OUT8(except_start,0,0);
    VL_OUT8(except_started,0,0);
    VL_IN8(ex_void,0,0);
    VL_OUT8(abort_ex,0,0);
    VL_IN8(icpu_ack_i,0,0);
    VL_IN8(icpu_err_i,0,0);
    VL_IN8(dcpu_ack_i,0,0);
    VL_IN8(dcpu_err_i,0,0);
    //char	__VpadToAlign37[1];
    VL_IN16(du_dsr,13,0);
    VL_OUT16(esr,15,0);
    VL_IN16(to_sr,15,0);
    VL_IN16(sr,15,0);
    VL_OUT16(except_stop,12,0);
    VL_IN(if_pc,31,0);
    VL_OUT(id_pc,31,0);
    VL_OUT(lr_sav,31,2);
    VL_IN(datain,31,0);
    VL_OUT(epcr,31,0);
    VL_OUT(eear,31,0);
    VL_IN(lsu_addr,31,0);
    VL_OUT(spr_dat_ppc,31,0);
    VL_OUT(spr_dat_npc,31,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    VL_SIG8(v__DOT__id_exceptflags,2,0);
    VL_SIG8(v__DOT__ex_exceptflags,2,0);
    VL_SIG8(v__DOT__state,2,0);
    VL_SIG8(v__DOT__ex_dslot,0,0);
    VL_SIG8(v__DOT__except_flushpipe,0,0);
    VL_SIG8(v__DOT__delayed_iee,2,0);
    VL_SIG8(v__DOT__delayed_tee,2,0);
    VL_SIG8(v__DOT__int_pending,0,0);
    VL_SIG8(v__DOT__tick_pending,0,0);
    //char	__VpadToAlign97[1];
    VL_SIG16(v__DOT__except_trig,12,0);
    VL_SIG(v__DOT__ex_pc,31,0);
    VL_SIG(v__DOT__wb_pc,31,0);
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    VL_SIG8(__Vclklast__TOP__clk,0,0);
    VL_SIG8(__Vclklast__TOP__rst,0,0);
    //char	__VpadToAlign114[2];
    
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
    static void	_combo__TOP__8(Vtop__Syms* __restrict vlSymsp);
    static void	_eval(Vtop__Syms* __restrict vlSymsp);
    static void	_eval_initial(Vtop__Syms* __restrict vlSymsp);
    static void	_eval_settle(Vtop__Syms* __restrict vlSymsp);
    static void	_sequent__TOP__3(Vtop__Syms* __restrict vlSymsp);
    static void	_sequent__TOP__6(Vtop__Syms* __restrict vlSymsp);
    static void	_settle__TOP__4(Vtop__Syms* __restrict vlSymsp);
    static void	_settle__TOP__7(Vtop__Syms* __restrict vlSymsp);
    static void	_settle__TOP__9(Vtop__Syms* __restrict vlSymsp);
} VL_ATTR_ALIGNED(128);

#endif  /*guard*/
