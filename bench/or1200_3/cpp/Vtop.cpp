// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "Vtop.h"              // For This
#include "Vtop__Syms.h"

//--------------------
// STATIC VARIABLES


//--------------------

VL_CTOR_IMP(Vtop) {
    Vtop__Syms* __restrict vlSymsp = __VlSymsp = new Vtop__Syms(this, name());
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    clk = VL_RAND_RESET_I(1);
    rst = VL_RAND_RESET_I(1);
    sig_ibuserr = VL_RAND_RESET_I(1);
    sig_dbuserr = VL_RAND_RESET_I(1);
    sig_illegal = VL_RAND_RESET_I(1);
    sig_align = VL_RAND_RESET_I(1);
    sig_range = VL_RAND_RESET_I(1);
    sig_dtlbmiss = VL_RAND_RESET_I(1);
    sig_dmmufault = VL_RAND_RESET_I(1);
    sig_int = VL_RAND_RESET_I(1);
    sig_syscall = VL_RAND_RESET_I(1);
    sig_trap = VL_RAND_RESET_I(1);
    sig_itlbmiss = VL_RAND_RESET_I(1);
    sig_immufault = VL_RAND_RESET_I(1);
    sig_tick = VL_RAND_RESET_I(1);
    branch_taken = VL_RAND_RESET_I(1);
    genpc_freeze = VL_RAND_RESET_I(1);
    id_freeze = VL_RAND_RESET_I(1);
    ex_freeze = VL_RAND_RESET_I(1);
    wb_freeze = VL_RAND_RESET_I(1);
    if_stall = VL_RAND_RESET_I(1);
    if_pc = VL_RAND_RESET_I(32);
    id_pc = VL_RAND_RESET_I(32);
    lr_sav = VL_RAND_RESET_I(30);
    datain = VL_RAND_RESET_I(32);
    du_dsr = VL_RAND_RESET_I(14);
    epcr_we = VL_RAND_RESET_I(1);
    eear_we = VL_RAND_RESET_I(1);
    esr_we = VL_RAND_RESET_I(1);
    pc_we = VL_RAND_RESET_I(1);
    epcr = VL_RAND_RESET_I(32);
    eear = VL_RAND_RESET_I(32);
    esr = VL_RAND_RESET_I(16);
    to_sr = VL_RAND_RESET_I(16);
    sr_we = VL_RAND_RESET_I(1);
    sr = VL_RAND_RESET_I(16);
    lsu_addr = VL_RAND_RESET_I(32);
    flushpipe = VL_RAND_RESET_I(1);
    extend_flush = VL_RAND_RESET_I(1);
    except_type = VL_RAND_RESET_I(4);
    except_start = VL_RAND_RESET_I(1);
    except_started = VL_RAND_RESET_I(1);
    except_stop = VL_RAND_RESET_I(13);
    ex_void = VL_RAND_RESET_I(1);
    spr_dat_ppc = VL_RAND_RESET_I(32);
    spr_dat_npc = VL_RAND_RESET_I(32);
    abort_ex = VL_RAND_RESET_I(1);
    icpu_ack_i = VL_RAND_RESET_I(1);
    icpu_err_i = VL_RAND_RESET_I(1);
    dcpu_ack_i = VL_RAND_RESET_I(1);
    dcpu_err_i = VL_RAND_RESET_I(1);
    v__DOT__ex_pc = VL_RAND_RESET_I(32);
    v__DOT__wb_pc = VL_RAND_RESET_I(32);
    v__DOT__id_exceptflags = VL_RAND_RESET_I(3);
    v__DOT__ex_exceptflags = VL_RAND_RESET_I(3);
    v__DOT__state = VL_RAND_RESET_I(3);
    v__DOT__ex_dslot = VL_RAND_RESET_I(1);
    v__DOT__except_trig = VL_RAND_RESET_I(13);
    v__DOT__except_flushpipe = VL_RAND_RESET_I(1);
    v__DOT__delayed_iee = VL_RAND_RESET_I(3);
    v__DOT__delayed_tee = VL_RAND_RESET_I(3);
    v__DOT__int_pending = VL_RAND_RESET_I(1);
    v__DOT__tick_pending = VL_RAND_RESET_I(1);
    __Vclklast__TOP__clk = VL_RAND_RESET_I(1);
    __Vclklast__TOP__rst = VL_RAND_RESET_I(1);
}

void Vtop::__Vconfigure(Vtop__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    // Coverage Declarations
    __vlCoverInsert(&(vlSymsp->__Vcoverage[0]), first, "or1200_except.v", 298, 0, ".v", "v_line/or1200_except", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[1]), first, "or1200_except.v", 302, 0, ".v", "v_line/or1200_except", "elsif");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[2]), first, "or1200_except.v", 306, 0, ".v", "v_line/or1200_except", "elsif");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[3]), first, "or1200_except.v", 321, 0, ".v", "v_line/or1200_except", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[4]), first, "or1200_except.v", 323, 0, ".v", "v_line/or1200_except", "elsif");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[5]), first, "or1200_except.v", 326, 0, ".v", "v_line/or1200_except", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[6]), first, "or1200_except.v", 337, 0, ".v", "v_line/or1200_except", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[7]), first, "or1200_except.v", 339, 0, ".v", "v_line/or1200_except", "elsif");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[8]), first, "or1200_except.v", 342, 0, ".v", "v_line/or1200_except", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[9]), first, "or1200_except.v", 348, 0, ".v", "v_line/or1200_except", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[10]), first, "or1200_except.v", 355, 0, ".v", "v_line/or1200_except", "elsif");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[11]), first, "or1200_except.v", 362, 0, ".v", "v_line/or1200_except", "elsif");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[12]), first, "or1200_except.v", 369, 0, ".v", "v_line/or1200_except", "elsif");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[13]), first, "or1200_except.v", 382, 0, ".v", "v_line/or1200_except", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[14]), first, "or1200_except.v", 385, 0, ".v", "v_line/or1200_except", "elsif");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[15]), first, "or1200_except.v", 409, 0, ".v", "v_line/or1200_except", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[16]), first, "or1200_except.v", 431, 0, ".v", "v_line/or1200_except", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[17]), first, "or1200_except.v", 437, 0, ".v", "v_line/or1200_except", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[18]), first, "or1200_except.v", 443, 0, ".v", "v_line/or1200_except", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[19]), first, "or1200_except.v", 457, 0, ".v", "v_line/or1200_except", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[20]), first, "or1200_except.v", 467, 0, ".v", "v_line/or1200_except", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[21]), first, "or1200_except.v", 474, 0, ".v", "v_line/or1200_except", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[22]), first, "or1200_except.v", 481, 0, ".v", "v_line/or1200_except", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[23]), first, "or1200_except.v", 488, 0, ".v", "v_line/or1200_except", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[24]), first, "or1200_except.v", 495, 0, ".v", "v_line/or1200_except", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[25]), first, "or1200_except.v", 502, 0, ".v", "v_line/or1200_except", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[26]), first, "or1200_except.v", 509, 0, ".v", "v_line/or1200_except", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[27]), first, "or1200_except.v", 514, 0, ".v", "v_line/or1200_except", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[28]), first, "or1200_except.v", 520, 0, ".v", "v_line/or1200_except", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[29]), first, "or1200_except.v", 525, 0, ".v", "v_line/or1200_except", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[30]), first, "or1200_except.v", 425, 0, ".v", "v_line/or1200_except", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[31]), first, "or1200_except.v", 529, 0, ".v", "v_line/or1200_except", "elsif");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[32]), first, "or1200_except.v", 534, 0, ".v", "v_line/or1200_except", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[33]), first, "or1200_except.v", 536, 0, ".v", "v_line/or1200_except", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[34]), first, "or1200_except.v", 538, 0, ".v", "v_line/or1200_except", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[35]), first, "or1200_except.v", 533, 0, ".v", "v_line/or1200_except", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[36]), first, "or1200_except.v", 424, 0, ".v", "v_line/or1200_except", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[37]), first, "or1200_except.v", 542, 0, ".v", "v_line/or1200_except", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[38]), first, "or1200_except.v", 541, 0, ".v", "v_line/or1200_except", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[39]), first, "or1200_except.v", 546, 0, ".v", "v_line/or1200_except", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[40]), first, "or1200_except.v", 554, 0, ".v", "v_line/or1200_except", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[41]), first, "or1200_except.v", 544, 0, ".v", "v_line/or1200_except", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[42]), first, "or1200_except.v", 555, 0, ".v", "v_line/or1200_except", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[43]), first, "or1200_except.v", 559, 0, ".v", "v_line/or1200_except", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[44]), first, "or1200_except.v", 569, 0, ".v", "v_line/or1200_except", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[45]), first, "or1200_except.v", 565, 0, ".v", "v_line/or1200_except", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[46]), first, "or1200_except.v", 418, 0, ".v", "v_line/or1200_except", "else");
}

Vtop::~Vtop() {
    delete __VlSymsp; __VlSymsp=NULL;
}

// Coverage
void Vtop::__vlCoverInsert(uint32_t* countp, bool enable, const char* filenamep, int lineno, int column,
    const char* hierp, const char* pagep, const char* commentp) {
    static uint32_t fake_zero_count = 0;
    if (!enable) countp = &fake_zero_count;
    *countp = 0;
    SP_COVER_INSERT(countp,  "filename",filenamep,  "lineno",lineno,  "column",column,
	"hier",string(name())+hierp,  "page",pagep,  "comment",commentp);
}

//--------------------


void Vtop::eval() {
    Vtop__Syms* __restrict vlSymsp = this->__VlSymsp; // Setup global symbol table
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Initialize
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) _eval_initial_loop(vlSymsp);
    // Evaluate till stable
    VL_DEBUG_IF(VL_PRINTF("\n----TOP Evaluate Vtop::eval\n"); );
    int __VclockLoop = 0;
    IData __Vchange=1;
    while (VL_LIKELY(__Vchange)) {
	VL_DEBUG_IF(VL_PRINTF(" Clock loop\n"););
	vlSymsp->__Vm_activity = true;
	_eval(vlSymsp);
	__Vchange = _change_request(vlSymsp);
	if (++__VclockLoop > 100) vl_fatal(__FILE__,__LINE__,__FILE__,"Verilated model didn't converge");
    }
}

void Vtop::_eval_initial_loop(Vtop__Syms* __restrict vlSymsp) {
    vlSymsp->__Vm_didInit = true;
    _eval_initial(vlSymsp);
    vlSymsp->__Vm_activity = true;
    int __VclockLoop = 0;
    IData __Vchange=1;
    while (VL_LIKELY(__Vchange)) {
	_eval_settle(vlSymsp);
	_eval(vlSymsp);
	__Vchange = _change_request(vlSymsp);
	if (++__VclockLoop > 100) vl_fatal(__FILE__,__LINE__,__FILE__,"Verilated model didn't DC converge");
    }
}

//--------------------
// Internal Methods

void Vtop::_combo__TOP__1(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_combo__TOP__1\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->abort_ex = (((((IData)(vlTOPp->sig_dbuserr) 
			   | (IData)(vlTOPp->sig_dmmufault)) 
			  | (IData)(vlTOPp->sig_dtlbmiss)) 
			 | (IData)(vlTOPp->sig_align)) 
			| (IData)(vlTOPp->sig_illegal));
}

void Vtop::_sequent__TOP__3(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_sequent__TOP__3\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    VL_SIG8(__Vdly__v__DOT__delayed_iee,2,0);
    VL_SIG8(__Vdly__v__DOT__delayed_tee,2,0);
    VL_SIG8(__Vdly__v__DOT__ex_dslot,0,0);
    VL_SIG8(__Vdly__v__DOT__state,2,0);
    VL_SIG8(__Vdly__except_type,3,0);
    //char	__VpadToAlign29[3];
    // Body
    __Vdly__v__DOT__delayed_tee = vlTOPp->v__DOT__delayed_tee;
    __Vdly__v__DOT__delayed_iee = vlTOPp->v__DOT__delayed_iee;
    __Vdly__v__DOT__ex_dslot = vlTOPp->v__DOT__ex_dslot;
    __Vdly__except_type = vlTOPp->except_type;
    __Vdly__v__DOT__state = vlTOPp->v__DOT__state;
    // ALWAYS at or1200_except.v:336
    if (vlTOPp->rst) {
	++(vlSymsp->__Vcoverage[6]);
	__Vdly__v__DOT__delayed_tee = 0;
    } else {
	if ((2 & (IData)(vlTOPp->sr))) {
	    __Vdly__v__DOT__delayed_tee = (1 | (6 & 
						((IData)(vlTOPp->v__DOT__delayed_tee) 
						 << 1)));
	    ++(vlSymsp->__Vcoverage[8]);
	} else {
	    ++(vlSymsp->__Vcoverage[7]);
	    __Vdly__v__DOT__delayed_tee = 0;
	}
    }
    // ALWAYS at or1200_except.v:320
    if (vlTOPp->rst) {
	++(vlSymsp->__Vcoverage[3]);
	__Vdly__v__DOT__delayed_iee = 0;
    } else {
	if ((4 & (IData)(vlTOPp->sr))) {
	    __Vdly__v__DOT__delayed_iee = (1 | (6 & 
						((IData)(vlTOPp->v__DOT__delayed_iee) 
						 << 1)));
	    ++(vlSymsp->__Vcoverage[5]);
	} else {
	    ++(vlSymsp->__Vcoverage[4]);
	    __Vdly__v__DOT__delayed_iee = 0;
	}
    }
    // ALWAYS at or1200_except.v:408
    if (vlTOPp->rst) {
	++(vlSymsp->__Vcoverage[15]);
	__Vdly__v__DOT__state = 0;
	__Vdly__except_type = 0;
	vlTOPp->extend_flush = 0;
	vlTOPp->epcr = 0;
	vlTOPp->eear = 0;
	vlTOPp->esr = 0x8001;
    } else {
	++(vlSymsp->__Vcoverage[46]);
	if ((4 & (IData)(vlTOPp->v__DOT__state))) {
	    if ((2 & (IData)(vlTOPp->v__DOT__state))) {
		++(vlSymsp->__Vcoverage[45]);
		if ((1 & ((~ (IData)(vlTOPp->if_stall)) 
			  & (~ (IData)(vlTOPp->id_freeze))))) {
		    ++(vlSymsp->__Vcoverage[44]);
		    __Vdly__v__DOT__state = 0;
		    __Vdly__except_type = 0;
		}
	    } else {
		if ((1 & (IData)(vlTOPp->v__DOT__state))) {
		    ++(vlSymsp->__Vcoverage[45]);
		    if ((1 & ((~ (IData)(vlTOPp->if_stall)) 
			      & (~ (IData)(vlTOPp->id_freeze))))) {
			++(vlSymsp->__Vcoverage[44]);
			__Vdly__v__DOT__state = 0;
			__Vdly__except_type = 0;
		    }
		} else {
		    ++(vlSymsp->__Vcoverage[43]);
		    __Vdly__v__DOT__state = 5;
		    vlTOPp->extend_flush = 0;
		}
	    }
	} else {
	    if ((2 & (IData)(vlTOPp->v__DOT__state))) {
		if ((1 & (IData)(vlTOPp->v__DOT__state))) {
		    ++(vlSymsp->__Vcoverage[42]);
		    __Vdly__v__DOT__state = 4;
		} else {
		    ++(vlSymsp->__Vcoverage[41]);
		    if ((0xe == (IData)(vlTOPp->except_type))) {
			++(vlSymsp->__Vcoverage[39]);
			__Vdly__v__DOT__state = 0;
			vlTOPp->extend_flush = 0;
			__Vdly__except_type = 0;
		    } else {
			++(vlSymsp->__Vcoverage[40]);
			__Vdly__v__DOT__state = 3;
		    }
		}
	    } else {
		if ((1 & (IData)(vlTOPp->v__DOT__state))) {
		    ++(vlSymsp->__Vcoverage[38]);
		    if ((((IData)(vlTOPp->icpu_ack_i) 
			  | (IData)(vlTOPp->icpu_err_i)) 
			 | (IData)(vlTOPp->genpc_freeze))) {
			++(vlSymsp->__Vcoverage[37]);
			__Vdly__v__DOT__state = 2;
		    }
		} else {
		    ++(vlSymsp->__Vcoverage[36]);
		    if (vlTOPp->v__DOT__except_flushpipe) {
			++(vlSymsp->__Vcoverage[30]);
			__Vdly__v__DOT__state = 1;
			vlTOPp->extend_flush = 1;
			vlTOPp->esr = ((IData)(vlTOPp->sr_we)
				        ? (IData)(vlTOPp->to_sr)
				        : (IData)(vlTOPp->sr));
			if (((((((((0x1000 == (0x1000 
					       & (IData)(vlTOPp->v__DOT__except_trig))) 
				   | (0x800 == (0x1800 
						& (IData)(vlTOPp->v__DOT__except_trig)))) 
				  | (0x400 == (0x1c00 
					       & (IData)(vlTOPp->v__DOT__except_trig)))) 
				 | (0x200 == (0x1e00 
					      & (IData)(vlTOPp->v__DOT__except_trig)))) 
				| (0x100 == (0x1f00 
					     & (IData)(vlTOPp->v__DOT__except_trig)))) 
			       | (0x80 == (0x1f80 & (IData)(vlTOPp->v__DOT__except_trig)))) 
			      | (0x40 == (0x1fc0 & (IData)(vlTOPp->v__DOT__except_trig)))) 
			     | (0x20 == (0x1fe0 & (IData)(vlTOPp->v__DOT__except_trig))))) {
			    if ((0x1000 == (0x1000 
					    & (IData)(vlTOPp->v__DOT__except_trig)))) {
				++(vlSymsp->__Vcoverage[16]);
				__Vdly__except_type = 5;
				vlTOPp->epcr = ((IData)(vlTOPp->v__DOT__ex_dslot)
						 ? vlTOPp->v__DOT__wb_pc
						 : vlTOPp->id_pc);
			    } else {
				if ((0x800 == (0x1800 
					       & (IData)(vlTOPp->v__DOT__except_trig)))) {
				    ++(vlSymsp->__Vcoverage[17]);
				    __Vdly__except_type = 8;
				    vlTOPp->epcr = 
					((IData)(vlTOPp->v__DOT__ex_dslot)
					  ? vlTOPp->v__DOT__wb_pc
					  : vlTOPp->id_pc);
				} else {
				    if ((0x400 == (0x1c00 
						   & (IData)(vlTOPp->v__DOT__except_trig)))) {
					++(vlSymsp->__Vcoverage[18]);
					__Vdly__except_type = 0xa;
					vlTOPp->eear 
					    = vlTOPp->v__DOT__ex_pc;
					vlTOPp->epcr 
					    = ((IData)(vlTOPp->v__DOT__ex_dslot)
					        ? vlTOPp->v__DOT__wb_pc
					        : vlTOPp->v__DOT__ex_pc);
				    } else {
					if ((0x200 
					     == (0x1e00 
						 & (IData)(vlTOPp->v__DOT__except_trig)))) {
					    ++(vlSymsp->__Vcoverage[19]);
					    __Vdly__except_type = 4;
					    vlTOPp->eear 
						= ((IData)(vlTOPp->v__DOT__ex_dslot)
						    ? vlTOPp->v__DOT__ex_pc
						    : vlTOPp->id_pc);
					    vlTOPp->epcr 
						= ((IData)(vlTOPp->v__DOT__ex_dslot)
						    ? vlTOPp->v__DOT__wb_pc
						    : vlTOPp->id_pc);
					} else {
					    if ((0x100 
						 == 
						 (0x1f00 
						  & (IData)(vlTOPp->v__DOT__except_trig)))) {
						++(vlSymsp->__Vcoverage[20]);
						__Vdly__except_type = 2;
						vlTOPp->eear 
						    = 
						    ((IData)(vlTOPp->v__DOT__ex_dslot)
						      ? vlTOPp->v__DOT__wb_pc
						      : vlTOPp->v__DOT__ex_pc);
						vlTOPp->epcr 
						    = 
						    ((IData)(vlTOPp->v__DOT__ex_dslot)
						      ? vlTOPp->v__DOT__wb_pc
						      : vlTOPp->v__DOT__ex_pc);
					    } else {
						if (
						    (0x80 
						     == 
						     (0x1f80 
						      & (IData)(vlTOPp->v__DOT__except_trig)))) {
						    ++(vlSymsp->__Vcoverage[21]);
						    __Vdly__except_type = 7;
						    vlTOPp->eear 
							= vlTOPp->v__DOT__ex_pc;
						    vlTOPp->epcr 
							= 
							((IData)(vlTOPp->v__DOT__ex_dslot)
							  ? vlTOPp->v__DOT__wb_pc
							  : vlTOPp->v__DOT__ex_pc);
						} else {
						    if (
							(0x40 
							 == 
							 (0x1fc0 
							  & (IData)(vlTOPp->v__DOT__except_trig)))) {
							++(vlSymsp->__Vcoverage[22]);
							__Vdly__except_type = 6;
							vlTOPp->eear 
							    = vlTOPp->lsu_addr;
							vlTOPp->epcr 
							    = 
							    ((IData)(vlTOPp->v__DOT__ex_dslot)
							      ? vlTOPp->v__DOT__wb_pc
							      : vlTOPp->v__DOT__ex_pc);
						    } else {
							++(vlSymsp->__Vcoverage[23]);
							__Vdly__except_type = 9;
							vlTOPp->eear 
							    = vlTOPp->lsu_addr;
							vlTOPp->epcr 
							    = 
							    ((IData)(vlTOPp->v__DOT__ex_dslot)
							      ? vlTOPp->v__DOT__wb_pc
							      : vlTOPp->v__DOT__ex_pc);
						    }
						}
					    }
					}
				    }
				}
			    }
			} else {
			    if ((0x10 == (0x1ff0 & (IData)(vlTOPp->v__DOT__except_trig)))) {
				++(vlSymsp->__Vcoverage[24]);
				__Vdly__except_type = 3;
				vlTOPp->eear = vlTOPp->lsu_addr;
				vlTOPp->epcr = ((IData)(vlTOPp->v__DOT__ex_dslot)
						 ? vlTOPp->v__DOT__wb_pc
						 : vlTOPp->v__DOT__ex_pc);
			    } else {
				if ((8 == (0x1ff8 & (IData)(vlTOPp->v__DOT__except_trig)))) {
				    ++(vlSymsp->__Vcoverage[25]);
				    __Vdly__except_type = 2;
				    vlTOPp->eear = vlTOPp->lsu_addr;
				    vlTOPp->epcr = 
					((IData)(vlTOPp->v__DOT__ex_dslot)
					  ? vlTOPp->v__DOT__wb_pc
					  : vlTOPp->v__DOT__ex_pc);
				} else {
				    if ((4 == (0x1ffc 
					       & (IData)(vlTOPp->v__DOT__except_trig)))) {
					++(vlSymsp->__Vcoverage[26]);
					__Vdly__except_type = 0xb;
					vlTOPp->epcr 
					    = ((IData)(vlTOPp->v__DOT__ex_dslot)
					        ? vlTOPp->v__DOT__wb_pc
					        : vlTOPp->id_pc);
				    } else {
					if ((2 == (0x1ffe 
						   & (IData)(vlTOPp->v__DOT__except_trig)))) {
					    ++(vlSymsp->__Vcoverage[27]);
					    __Vdly__except_type = 0xe;
					    vlTOPp->epcr 
						= ((IData)(vlTOPp->v__DOT__ex_dslot)
						    ? vlTOPp->v__DOT__wb_pc
						    : vlTOPp->v__DOT__ex_pc);
					} else {
					    if ((1 
						 == (IData)(vlTOPp->v__DOT__except_trig))) {
						++(vlSymsp->__Vcoverage[28]);
						__Vdly__except_type = 0xc;
						vlTOPp->epcr 
						    = 
						    ((IData)(vlTOPp->v__DOT__ex_dslot)
						      ? vlTOPp->v__DOT__wb_pc
						      : vlTOPp->id_pc);
					    } else {
						++(vlSymsp->__Vcoverage[29]);
						__Vdly__except_type = 0;
					    }
					}
				    }
				}
			    }
			}
		    } else {
			if (vlTOPp->pc_we) {
			    ++(vlSymsp->__Vcoverage[31]);
			    __Vdly__v__DOT__state = 1;
			    vlTOPp->extend_flush = 1;
			} else {
			    ++(vlSymsp->__Vcoverage[35]);
			    if (vlTOPp->epcr_we) {
				++(vlSymsp->__Vcoverage[32]);
				vlTOPp->epcr = vlTOPp->datain;
			    }
			    if (vlTOPp->eear_we) {
				++(vlSymsp->__Vcoverage[33]);
				vlTOPp->eear = vlTOPp->datain;
			    }
			    if (vlTOPp->esr_we) {
				++(vlSymsp->__Vcoverage[34]);
				vlTOPp->esr = (0x8000 
					       | (0x7fff 
						  & vlTOPp->datain));
			    }
			}
		    }
		}
	    }
	}
    }
    vlTOPp->v__DOT__delayed_tee = __Vdly__v__DOT__delayed_tee;
    vlTOPp->v__DOT__delayed_iee = __Vdly__v__DOT__delayed_iee;
    vlTOPp->except_type = __Vdly__except_type;
    vlTOPp->v__DOT__state = __Vdly__v__DOT__state;
    vlTOPp->except_start = ((0 != (IData)(vlTOPp->except_type)) 
			    & (IData)(vlTOPp->extend_flush));
    // ALWAYS at or1200_except.v:381
    if (vlTOPp->rst) {
	++(vlSymsp->__Vcoverage[13]);
	vlTOPp->v__DOT__wb_pc = 0;
    } else {
	if ((1 & (~ (IData)(vlTOPp->wb_freeze)))) {
	    ++(vlSymsp->__Vcoverage[14]);
	    vlTOPp->v__DOT__wb_pc = vlTOPp->v__DOT__ex_pc;
	}
    }
    vlTOPp->spr_dat_ppc = vlTOPp->v__DOT__wb_pc;
    // ALWAYS at or1200_except.v:347
    if (vlTOPp->rst) {
	++(vlSymsp->__Vcoverage[9]);
	__Vdly__v__DOT__ex_dslot = 0;
	vlTOPp->v__DOT__ex_pc = 0;
	vlTOPp->v__DOT__ex_exceptflags = 0;
    } else {
	if (vlTOPp->flushpipe) {
	    ++(vlSymsp->__Vcoverage[10]);
	    __Vdly__v__DOT__ex_dslot = 0;
	    vlTOPp->v__DOT__ex_pc = 0;
	    vlTOPp->v__DOT__ex_exceptflags = 0;
	} else {
	    if (((~ (IData)(vlTOPp->ex_freeze)) & (IData)(vlTOPp->id_freeze))) {
		++(vlSymsp->__Vcoverage[11]);
		vlTOPp->v__DOT__ex_pc = vlTOPp->id_pc;
		vlTOPp->v__DOT__ex_exceptflags = 0;
		__Vdly__v__DOT__ex_dslot = 0;
	    } else {
		if ((1 & (~ (IData)(vlTOPp->ex_freeze)))) {
		    ++(vlSymsp->__Vcoverage[12]);
		    vlTOPp->v__DOT__ex_exceptflags 
			= vlTOPp->v__DOT__id_exceptflags;
		    __Vdly__v__DOT__ex_dslot = vlTOPp->branch_taken;
		    vlTOPp->v__DOT__ex_pc = vlTOPp->id_pc;
		}
	    }
	}
    }
    vlTOPp->v__DOT__ex_dslot = __Vdly__v__DOT__ex_dslot;
    vlTOPp->lr_sav = (0x3fffffff & (vlTOPp->v__DOT__ex_pc 
				    >> 2));
    // ALWAYS at or1200_except.v:297
    if (vlTOPp->rst) {
	++(vlSymsp->__Vcoverage[0]);
	vlTOPp->id_pc = 0;
	vlTOPp->v__DOT__id_exceptflags = 0;
    } else {
	if (vlTOPp->flushpipe) {
	    ++(vlSymsp->__Vcoverage[1]);
	    vlTOPp->id_pc = 0;
	    vlTOPp->v__DOT__id_exceptflags = 0;
	} else {
	    if ((1 & (~ (IData)(vlTOPp->id_freeze)))) {
		++(vlSymsp->__Vcoverage[2]);
		vlTOPp->id_pc = vlTOPp->if_pc;
		vlTOPp->v__DOT__id_exceptflags = (((IData)(vlTOPp->sig_ibuserr) 
						   << 2) 
						  | (((IData)(vlTOPp->sig_itlbmiss) 
						      << 1) 
						     | (IData)(vlTOPp->sig_immufault)));
	    }
	}
    }
}

void Vtop::_settle__TOP__4(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_settle__TOP__4\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->except_start = ((0 != (IData)(vlTOPp->except_type)) 
			    & (IData)(vlTOPp->extend_flush));
    vlTOPp->spr_dat_ppc = vlTOPp->v__DOT__wb_pc;
    vlTOPp->lr_sav = (0x3fffffff & (vlTOPp->v__DOT__ex_pc 
				    >> 2));
    vlTOPp->v__DOT__tick_pending = ((((((IData)(vlTOPp->sig_tick) 
					& ((IData)(vlTOPp->sr) 
					   >> 1)) & 
				       (~ (IData)(vlTOPp->ex_freeze))) 
				      & (~ (IData)(vlTOPp->branch_taken))) 
				     & (~ (IData)(vlTOPp->v__DOT__ex_dslot))) 
				    & (~ (IData)(vlTOPp->sr_we)));
    vlTOPp->v__DOT__int_pending = (((((((IData)(vlTOPp->sig_int) 
					& ((IData)(vlTOPp->sr) 
					   >> 2)) & 
				       ((IData)(vlTOPp->v__DOT__delayed_iee) 
					>> 2)) & (~ (IData)(vlTOPp->ex_freeze))) 
				     & (~ (IData)(vlTOPp->branch_taken))) 
				    & (~ (IData)(vlTOPp->v__DOT__ex_dslot))) 
				   & (~ (IData)(vlTOPp->sr_we)));
    vlTOPp->spr_dat_npc = ((IData)(vlTOPp->ex_void)
			    ? vlTOPp->id_pc : vlTOPp->v__DOT__ex_pc);
    vlTOPp->except_started = ((IData)(vlTOPp->extend_flush) 
			      & (IData)(vlTOPp->except_start));
}

void Vtop::_combo__TOP__5(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_combo__TOP__5\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->v__DOT__tick_pending = ((((((IData)(vlTOPp->sig_tick) 
					& ((IData)(vlTOPp->sr) 
					   >> 1)) & 
				       (~ (IData)(vlTOPp->ex_freeze))) 
				      & (~ (IData)(vlTOPp->branch_taken))) 
				     & (~ (IData)(vlTOPp->v__DOT__ex_dslot))) 
				    & (~ (IData)(vlTOPp->sr_we)));
    vlTOPp->v__DOT__int_pending = (((((((IData)(vlTOPp->sig_int) 
					& ((IData)(vlTOPp->sr) 
					   >> 2)) & 
				       ((IData)(vlTOPp->v__DOT__delayed_iee) 
					>> 2)) & (~ (IData)(vlTOPp->ex_freeze))) 
				     & (~ (IData)(vlTOPp->branch_taken))) 
				    & (~ (IData)(vlTOPp->v__DOT__ex_dslot))) 
				   & (~ (IData)(vlTOPp->sr_we)));
    vlTOPp->spr_dat_npc = ((IData)(vlTOPp->ex_void)
			    ? vlTOPp->id_pc : vlTOPp->v__DOT__ex_pc);
    vlTOPp->except_stop = ((0xfffff000 & (((IData)(vlTOPp->v__DOT__tick_pending) 
					   << 0xc) 
					  & ((IData)(vlTOPp->du_dsr) 
					     << 8))) 
			   | ((0xfffff800 & (((IData)(vlTOPp->v__DOT__int_pending) 
					      << 0xb) 
					     & ((IData)(vlTOPp->du_dsr) 
						<< 4))) 
			      | ((0x400 & (((IData)(vlTOPp->v__DOT__ex_exceptflags) 
					    << 9) & 
					   ((IData)(vlTOPp->du_dsr) 
					    << 1))) 
				 | ((0x200 & (((IData)(vlTOPp->v__DOT__ex_exceptflags) 
					       << 9) 
					      & ((IData)(vlTOPp->du_dsr) 
						 << 6))) 
				    | ((0x100 & (((IData)(vlTOPp->v__DOT__ex_exceptflags) 
						  << 6) 
						 & ((IData)(vlTOPp->du_dsr) 
						    << 7))) 
				       | ((0xffffff80 
					   & (((IData)(vlTOPp->sig_illegal) 
					       << 7) 
					      & ((IData)(vlTOPp->du_dsr) 
						 << 1))) 
					  | ((0xffffffc0 
					      & (((IData)(vlTOPp->sig_align) 
						  << 6) 
						 & ((IData)(vlTOPp->du_dsr) 
						    << 1))) 
					     | ((0x1fffffe0 
						 & (((IData)(vlTOPp->sig_dtlbmiss) 
						     << 5) 
						    & ((IData)(vlTOPp->du_dsr) 
						       >> 3))) 
						| ((0xfffffff0 
						    & (((IData)(vlTOPp->sig_dmmufault) 
							<< 4) 
						       & ((IData)(vlTOPp->du_dsr) 
							  << 2))) 
						   | ((0xfffffff8 
						       & (((IData)(vlTOPp->sig_dbuserr) 
							   << 3) 
							  & ((IData)(vlTOPp->du_dsr) 
							     << 2))) 
						      | ((0xfffffc 
							  & (((IData)(vlTOPp->sig_range) 
							      << 2) 
							     & ((IData)(vlTOPp->du_dsr) 
								>> 8))) 
							 | ((0xffffe 
							     & ((((IData)(vlTOPp->sig_trap) 
								  << 1) 
								 & ((IData)(vlTOPp->du_dsr) 
								    >> 0xc)) 
								& ((~ (IData)(vlTOPp->ex_freeze)) 
								   << 1))) 
							    | (((IData)(vlTOPp->sig_syscall) 
								& ((IData)(vlTOPp->du_dsr) 
								   >> 0xb)) 
							       & (~ (IData)(vlTOPp->ex_freeze)))))))))))))));
    vlTOPp->v__DOT__except_trig = ((((IData)(vlTOPp->v__DOT__tick_pending) 
				     & (~ ((IData)(vlTOPp->du_dsr) 
					   >> 4))) 
				    << 0xc) | ((((IData)(vlTOPp->v__DOT__int_pending) 
						 & (~ 
						    ((IData)(vlTOPp->du_dsr) 
						     >> 7))) 
						<< 0xb) 
					       | ((0x400 
						   & (((IData)(vlTOPp->v__DOT__ex_exceptflags) 
						       << 9) 
						      & ((~ 
							  ((IData)(vlTOPp->du_dsr) 
							   >> 9)) 
							 << 0xa))) 
						  | ((0x200 
						      & (((IData)(vlTOPp->v__DOT__ex_exceptflags) 
							  & (~ 
							     ((IData)(vlTOPp->du_dsr) 
							      >> 3))) 
							 << 9)) 
						     | ((0x100 
							 & (((IData)(vlTOPp->v__DOT__ex_exceptflags) 
							     << 6) 
							    & ((~ 
								((IData)(vlTOPp->du_dsr) 
								 >> 1)) 
							       << 8))) 
							| ((((IData)(vlTOPp->sig_illegal) 
							     & (~ 
								((IData)(vlTOPp->du_dsr) 
								 >> 6))) 
							    << 7) 
							   | ((((IData)(vlTOPp->sig_align) 
								& (~ 
								   ((IData)(vlTOPp->du_dsr) 
								    >> 5))) 
							       << 6) 
							      | ((((IData)(vlTOPp->sig_dtlbmiss) 
								   & (~ 
								      ((IData)(vlTOPp->du_dsr) 
								       >> 8))) 
								  << 5) 
								 | ((((IData)(vlTOPp->sig_dmmufault) 
								      & (~ 
									 ((IData)(vlTOPp->du_dsr) 
									  >> 2))) 
								     << 4) 
								    | ((((IData)(vlTOPp->sig_dbuserr) 
									 & (~ 
									    ((IData)(vlTOPp->du_dsr) 
									     >> 1))) 
									<< 3) 
								       | ((((IData)(vlTOPp->sig_range) 
									    & (~ 
									       ((IData)(vlTOPp->du_dsr) 
										>> 0xa))) 
									   << 2) 
									  | (((((IData)(vlTOPp->sig_trap) 
										& (~ 
										((IData)(vlTOPp->du_dsr) 
										>> 0xd))) 
									       & (~ (IData)(vlTOPp->ex_freeze))) 
									      << 1) 
									     | (((IData)(vlTOPp->sig_syscall) 
										& (~ 
										((IData)(vlTOPp->du_dsr) 
										>> 0xb))) 
										& (~ (IData)(vlTOPp->ex_freeze)))))))))))))));
}

void Vtop::_sequent__TOP__6(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_sequent__TOP__6\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->except_started = ((IData)(vlTOPp->extend_flush) 
			      & (IData)(vlTOPp->except_start));
}

void Vtop::_settle__TOP__7(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_settle__TOP__7\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->except_stop = ((0xfffff000 & (((IData)(vlTOPp->v__DOT__tick_pending) 
					   << 0xc) 
					  & ((IData)(vlTOPp->du_dsr) 
					     << 8))) 
			   | ((0xfffff800 & (((IData)(vlTOPp->v__DOT__int_pending) 
					      << 0xb) 
					     & ((IData)(vlTOPp->du_dsr) 
						<< 4))) 
			      | ((0x400 & (((IData)(vlTOPp->v__DOT__ex_exceptflags) 
					    << 9) & 
					   ((IData)(vlTOPp->du_dsr) 
					    << 1))) 
				 | ((0x200 & (((IData)(vlTOPp->v__DOT__ex_exceptflags) 
					       << 9) 
					      & ((IData)(vlTOPp->du_dsr) 
						 << 6))) 
				    | ((0x100 & (((IData)(vlTOPp->v__DOT__ex_exceptflags) 
						  << 6) 
						 & ((IData)(vlTOPp->du_dsr) 
						    << 7))) 
				       | ((0xffffff80 
					   & (((IData)(vlTOPp->sig_illegal) 
					       << 7) 
					      & ((IData)(vlTOPp->du_dsr) 
						 << 1))) 
					  | ((0xffffffc0 
					      & (((IData)(vlTOPp->sig_align) 
						  << 6) 
						 & ((IData)(vlTOPp->du_dsr) 
						    << 1))) 
					     | ((0x1fffffe0 
						 & (((IData)(vlTOPp->sig_dtlbmiss) 
						     << 5) 
						    & ((IData)(vlTOPp->du_dsr) 
						       >> 3))) 
						| ((0xfffffff0 
						    & (((IData)(vlTOPp->sig_dmmufault) 
							<< 4) 
						       & ((IData)(vlTOPp->du_dsr) 
							  << 2))) 
						   | ((0xfffffff8 
						       & (((IData)(vlTOPp->sig_dbuserr) 
							   << 3) 
							  & ((IData)(vlTOPp->du_dsr) 
							     << 2))) 
						      | ((0xfffffc 
							  & (((IData)(vlTOPp->sig_range) 
							      << 2) 
							     & ((IData)(vlTOPp->du_dsr) 
								>> 8))) 
							 | ((0xffffe 
							     & ((((IData)(vlTOPp->sig_trap) 
								  << 1) 
								 & ((IData)(vlTOPp->du_dsr) 
								    >> 0xc)) 
								& ((~ (IData)(vlTOPp->ex_freeze)) 
								   << 1))) 
							    | (((IData)(vlTOPp->sig_syscall) 
								& ((IData)(vlTOPp->du_dsr) 
								   >> 0xb)) 
							       & (~ (IData)(vlTOPp->ex_freeze)))))))))))))));
    vlTOPp->v__DOT__except_trig = ((((IData)(vlTOPp->v__DOT__tick_pending) 
				     & (~ ((IData)(vlTOPp->du_dsr) 
					   >> 4))) 
				    << 0xc) | ((((IData)(vlTOPp->v__DOT__int_pending) 
						 & (~ 
						    ((IData)(vlTOPp->du_dsr) 
						     >> 7))) 
						<< 0xb) 
					       | ((0x400 
						   & (((IData)(vlTOPp->v__DOT__ex_exceptflags) 
						       << 9) 
						      & ((~ 
							  ((IData)(vlTOPp->du_dsr) 
							   >> 9)) 
							 << 0xa))) 
						  | ((0x200 
						      & (((IData)(vlTOPp->v__DOT__ex_exceptflags) 
							  & (~ 
							     ((IData)(vlTOPp->du_dsr) 
							      >> 3))) 
							 << 9)) 
						     | ((0x100 
							 & (((IData)(vlTOPp->v__DOT__ex_exceptflags) 
							     << 6) 
							    & ((~ 
								((IData)(vlTOPp->du_dsr) 
								 >> 1)) 
							       << 8))) 
							| ((((IData)(vlTOPp->sig_illegal) 
							     & (~ 
								((IData)(vlTOPp->du_dsr) 
								 >> 6))) 
							    << 7) 
							   | ((((IData)(vlTOPp->sig_align) 
								& (~ 
								   ((IData)(vlTOPp->du_dsr) 
								    >> 5))) 
							       << 6) 
							      | ((((IData)(vlTOPp->sig_dtlbmiss) 
								   & (~ 
								      ((IData)(vlTOPp->du_dsr) 
								       >> 8))) 
								  << 5) 
								 | ((((IData)(vlTOPp->sig_dmmufault) 
								      & (~ 
									 ((IData)(vlTOPp->du_dsr) 
									  >> 2))) 
								     << 4) 
								    | ((((IData)(vlTOPp->sig_dbuserr) 
									 & (~ 
									    ((IData)(vlTOPp->du_dsr) 
									     >> 1))) 
									<< 3) 
								       | ((((IData)(vlTOPp->sig_range) 
									    & (~ 
									       ((IData)(vlTOPp->du_dsr) 
										>> 0xa))) 
									   << 2) 
									  | (((((IData)(vlTOPp->sig_trap) 
										& (~ 
										((IData)(vlTOPp->du_dsr) 
										>> 0xd))) 
									       & (~ (IData)(vlTOPp->ex_freeze))) 
									      << 1) 
									     | (((IData)(vlTOPp->sig_syscall) 
										& (~ 
										((IData)(vlTOPp->du_dsr) 
										>> 0xb))) 
										& (~ (IData)(vlTOPp->ex_freeze)))))))))))))));
    vlTOPp->v__DOT__except_flushpipe = ((0 != (IData)(vlTOPp->v__DOT__except_trig)) 
					& (~ (IData)(
						     (0 
						      != (IData)(vlTOPp->v__DOT__state)))));
}

void Vtop::_combo__TOP__8(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_combo__TOP__8\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->v__DOT__except_flushpipe = ((0 != (IData)(vlTOPp->v__DOT__except_trig)) 
					& (~ (IData)(
						     (0 
						      != (IData)(vlTOPp->v__DOT__state)))));
    vlTOPp->flushpipe = (((IData)(vlTOPp->v__DOT__except_flushpipe) 
			  | (IData)(vlTOPp->pc_we)) 
			 | (IData)(vlTOPp->extend_flush));
}

void Vtop::_settle__TOP__9(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_settle__TOP__9\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->flushpipe = (((IData)(vlTOPp->v__DOT__except_flushpipe) 
			  | (IData)(vlTOPp->pc_we)) 
			 | (IData)(vlTOPp->extend_flush));
}

void Vtop::_eval(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_eval\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_combo__TOP__1(vlSymsp);
    if ((((IData)(vlTOPp->clk) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk))) 
	 | ((IData)(vlTOPp->rst) & (~ (IData)(vlTOPp->__Vclklast__TOP__rst))))) {
	vlTOPp->_sequent__TOP__3(vlSymsp);
    }
    vlTOPp->_combo__TOP__5(vlSymsp);
    if ((((IData)(vlTOPp->clk) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk))) 
	 | ((IData)(vlTOPp->rst) & (~ (IData)(vlTOPp->__Vclklast__TOP__rst))))) {
	vlTOPp->_sequent__TOP__6(vlSymsp);
    }
    vlTOPp->_combo__TOP__8(vlSymsp);
    // Final
    vlTOPp->__Vclklast__TOP__clk = vlTOPp->clk;
    vlTOPp->__Vclklast__TOP__rst = vlTOPp->rst;
}

void Vtop::_eval_initial(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_eval_initial\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vtop::final() {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::final\n"); );
    // Variables
    Vtop__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vtop::_eval_settle(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_eval_settle\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_combo__TOP__1(vlSymsp);
    vlTOPp->_settle__TOP__4(vlSymsp);
    vlTOPp->_settle__TOP__7(vlSymsp);
    vlTOPp->_settle__TOP__9(vlSymsp);
}

IData Vtop::_change_request(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_change_request\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    IData __req = false;  // Logically a bool
    return __req;
}
