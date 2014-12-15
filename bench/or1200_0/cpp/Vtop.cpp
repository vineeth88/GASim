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
    icbiu_dat_o = VL_RAND_RESET_I(32);
    icbiu_adr_o = VL_RAND_RESET_I(32);
    icbiu_cyc_o = VL_RAND_RESET_I(1);
    icbiu_stb_o = VL_RAND_RESET_I(1);
    icbiu_we_o = VL_RAND_RESET_I(1);
    icbiu_sel_o = VL_RAND_RESET_I(4);
    icbiu_cab_o = VL_RAND_RESET_I(1);
    icbiu_dat_i = VL_RAND_RESET_I(32);
    icbiu_ack_i = VL_RAND_RESET_I(1);
    icbiu_err_i = VL_RAND_RESET_I(1);
    ic_en = VL_RAND_RESET_I(1);
    icqmem_adr_i = VL_RAND_RESET_I(32);
    icqmem_cycstb_i = VL_RAND_RESET_I(1);
    icqmem_ci_i = VL_RAND_RESET_I(1);
    icqmem_sel_i = VL_RAND_RESET_I(4);
    icqmem_tag_i = VL_RAND_RESET_I(4);
    icqmem_dat_o = VL_RAND_RESET_I(32);
    icqmem_ack_o = VL_RAND_RESET_I(1);
    icqmem_rty_o = VL_RAND_RESET_I(1);
    icqmem_err_o = VL_RAND_RESET_I(1);
    icqmem_tag_o = VL_RAND_RESET_I(4);
    spr_cs = VL_RAND_RESET_I(1);
    spr_write = VL_RAND_RESET_I(1);
    spr_dat_i = VL_RAND_RESET_I(32);
    v__DOT__ic_addr = VL_RAND_RESET_I(32);
    v__DOT__icfsm_biu_read = VL_RAND_RESET_I(1);
    v__DOT__tagcomp_miss = VL_RAND_RESET_I(1);
    v__DOT__icfsm_first_miss_ack = VL_RAND_RESET_I(1);
    v__DOT__or1200_ic_fsm__DOT__saved_addr_r = VL_RAND_RESET_I(32);
    v__DOT__or1200_ic_fsm__DOT__state = VL_RAND_RESET_I(2);
    v__DOT__or1200_ic_fsm__DOT__cnt = VL_RAND_RESET_I(3);
    v__DOT__or1200_ic_fsm__DOT__hitmiss_eval = VL_RAND_RESET_I(1);
    v__DOT__or1200_ic_fsm__DOT__load = VL_RAND_RESET_I(1);
    v__DOT__or1200_ic_fsm__DOT__cache_inhibit = VL_RAND_RESET_I(1);
    __Vclklast__TOP__clk = VL_RAND_RESET_I(1);
    __Vclklast__TOP__rst = VL_RAND_RESET_I(1);
}

void Vtop::__Vconfigure(Vtop__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    // Coverage Declarations
    __vlCoverInsert(&(vlSymsp->__Vcoverage[0]), first, "or1200_ic_top.v", 270, 0, ".v", "v_line/or1200_ic_top", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[1]), first, "or1200_ic_top.v", 273, 0, ".v", "v_line/or1200_ic_top", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[2]), first, "or1200_ic_fsm.v", 190, 0, ".v.or1200_ic_fsm", "v_line/or1200_ic_fsm", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[3]), first, "or1200_ic_fsm.v", 201, 0, ".v.or1200_ic_fsm", "v_line/or1200_ic_fsm", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[4]), first, "or1200_ic_fsm.v", 208, 0, ".v.or1200_ic_fsm", "v_line/or1200_ic_fsm", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[5]), first, "or1200_ic_fsm.v", 200, 0, ".v.or1200_ic_fsm", "v_line/or1200_ic_fsm", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[6]), first, "or1200_ic_fsm.v", 214, 0, ".v.or1200_ic_fsm", "v_line/or1200_ic_fsm", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[7]), first, "or1200_ic_fsm.v", 216, 0, ".v.or1200_ic_fsm", "v_line/or1200_ic_fsm", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[8]), first, "or1200_ic_fsm.v", 218, 0, ".v.or1200_ic_fsm", "v_line/or1200_ic_fsm", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[9]), first, "or1200_ic_fsm.v", 227, 0, ".v.or1200_ic_fsm", "v_line/or1200_ic_fsm", "elsif");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[10]), first, "or1200_ic_fsm.v", 234, 0, ".v.or1200_ic_fsm", "v_line/or1200_ic_fsm", "elsif");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[11]), first, "or1200_ic_fsm.v", 238, 0, ".v.or1200_ic_fsm", "v_line/or1200_ic_fsm", "elsif");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[12]), first, "or1200_ic_fsm.v", 245, 0, ".v.or1200_ic_fsm", "v_line/or1200_ic_fsm", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[13]), first, "or1200_ic_fsm.v", 213, 0, ".v.or1200_ic_fsm", "v_line/or1200_ic_fsm", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[14]), first, "or1200_ic_fsm.v", 248, 0, ".v.or1200_ic_fsm", "v_line/or1200_ic_fsm", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[15]), first, "or1200_ic_fsm.v", 252, 0, ".v.or1200_ic_fsm", "v_line/or1200_ic_fsm", "elsif");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[16]), first, "or1200_ic_fsm.v", 247, 0, ".v.or1200_ic_fsm", "v_line/or1200_ic_fsm", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[17]), first, "or1200_ic_fsm.v", 259, 0, ".v.or1200_ic_fsm", "v_line/or1200_ic_fsm", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[18]), first, "or1200_ic_fsm.v", 199, 0, ".v.or1200_ic_fsm", "v_line/or1200_ic_fsm", "else");
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

void Vtop::_settle__TOP__1(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_settle__TOP__1\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // ALWAYS at or1200_ic_top.v:270
    ++(vlSymsp->__Vcoverage[0]);
}

void Vtop::_initial__TOP__2(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_initial__TOP__2\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // INITIAL at or1200_ic_top.v:232
    vlTOPp->icbiu_dat_o = 0;
    // INITIAL at or1200_ic_top.v:239
    vlTOPp->icbiu_we_o = 0;
}

void Vtop::_sequent__TOP__3(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_sequent__TOP__3\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    VL_SIG8(__Vdly__v__DOT__or1200_ic_fsm__DOT__state,1,0);
    VL_SIG8(__Vdly__v__DOT__or1200_ic_fsm__DOT__hitmiss_eval,0,0);
    VL_SIG8(__Vdly__v__DOT__or1200_ic_fsm__DOT__cnt,2,0);
    VL_SIG8(__Vdly__v__DOT__or1200_ic_fsm__DOT__cache_inhibit,0,0);
    VL_SIG(__Vdly__v__DOT__or1200_ic_fsm__DOT__saved_addr_r,31,0);
    // Body
    __Vdly__v__DOT__or1200_ic_fsm__DOT__cnt = vlTOPp->v__DOT__or1200_ic_fsm__DOT__cnt;
    __Vdly__v__DOT__or1200_ic_fsm__DOT__saved_addr_r 
	= vlTOPp->v__DOT__or1200_ic_fsm__DOT__saved_addr_r;
    __Vdly__v__DOT__or1200_ic_fsm__DOT__cache_inhibit 
	= vlTOPp->v__DOT__or1200_ic_fsm__DOT__cache_inhibit;
    __Vdly__v__DOT__or1200_ic_fsm__DOT__hitmiss_eval 
	= vlTOPp->v__DOT__or1200_ic_fsm__DOT__hitmiss_eval;
    __Vdly__v__DOT__or1200_ic_fsm__DOT__state = vlTOPp->v__DOT__or1200_ic_fsm__DOT__state;
    // ALWAYS at or1200_ic_fsm.v:189
    if (vlTOPp->rst) {
	++(vlSymsp->__Vcoverage[2]);
	__Vdly__v__DOT__or1200_ic_fsm__DOT__state = 0;
	__Vdly__v__DOT__or1200_ic_fsm__DOT__saved_addr_r = 0;
	__Vdly__v__DOT__or1200_ic_fsm__DOT__hitmiss_eval = 0;
	vlTOPp->v__DOT__or1200_ic_fsm__DOT__load = 0;
	__Vdly__v__DOT__or1200_ic_fsm__DOT__cnt = 0;
	__Vdly__v__DOT__or1200_ic_fsm__DOT__cache_inhibit = 0;
    } else {
	++(vlSymsp->__Vcoverage[18]);
	if ((0 == (IData)(vlTOPp->v__DOT__or1200_ic_fsm__DOT__state))) {
	    ++(vlSymsp->__Vcoverage[5]);
	    if (((IData)(vlTOPp->ic_en) & (IData)(vlTOPp->icqmem_cycstb_i))) {
		++(vlSymsp->__Vcoverage[3]);
		__Vdly__v__DOT__or1200_ic_fsm__DOT__state = 1;
		__Vdly__v__DOT__or1200_ic_fsm__DOT__saved_addr_r 
		    = vlTOPp->icqmem_adr_i;
		__Vdly__v__DOT__or1200_ic_fsm__DOT__hitmiss_eval = 1;
		vlTOPp->v__DOT__or1200_ic_fsm__DOT__load = 1;
		__Vdly__v__DOT__or1200_ic_fsm__DOT__cache_inhibit = 0;
	    } else {
		++(vlSymsp->__Vcoverage[4]);
		__Vdly__v__DOT__or1200_ic_fsm__DOT__hitmiss_eval = 0;
		vlTOPp->v__DOT__or1200_ic_fsm__DOT__load = 0;
		__Vdly__v__DOT__or1200_ic_fsm__DOT__cache_inhibit = 0;
	    }
	} else {
	    if ((1 == (IData)(vlTOPp->v__DOT__or1200_ic_fsm__DOT__state))) {
		++(vlSymsp->__Vcoverage[13]);
		if (((IData)(vlTOPp->icqmem_cycstb_i) 
		     & (IData)(vlTOPp->icqmem_ci_i))) {
		    ++(vlSymsp->__Vcoverage[6]);
		    __Vdly__v__DOT__or1200_ic_fsm__DOT__cache_inhibit = 1;
		}
		if (vlTOPp->v__DOT__or1200_ic_fsm__DOT__hitmiss_eval) {
		    ++(vlSymsp->__Vcoverage[7]);
		    __Vdly__v__DOT__or1200_ic_fsm__DOT__saved_addr_r 
			= ((0x1fff & __Vdly__v__DOT__or1200_ic_fsm__DOT__saved_addr_r) 
			   | (0xffffe000 & vlTOPp->icqmem_adr_i));
		}
		if ((1 & ((((~ (IData)(vlTOPp->ic_en)) 
			    | ((IData)(vlTOPp->v__DOT__or1200_ic_fsm__DOT__hitmiss_eval) 
			       & (~ (IData)(vlTOPp->icqmem_cycstb_i)))) 
			   | (IData)(vlTOPp->icbiu_err_i)) 
			  | ((IData)(vlTOPp->v__DOT__or1200_ic_fsm__DOT__cache_inhibit) 
			     & (IData)(vlTOPp->icbiu_ack_i))))) {
		    ++(vlSymsp->__Vcoverage[8]);
		    __Vdly__v__DOT__or1200_ic_fsm__DOT__state = 0;
		    __Vdly__v__DOT__or1200_ic_fsm__DOT__hitmiss_eval = 0;
		    vlTOPp->v__DOT__or1200_ic_fsm__DOT__load = 0;
		    __Vdly__v__DOT__or1200_ic_fsm__DOT__cache_inhibit = 0;
		} else {
		    if (((IData)(vlTOPp->v__DOT__tagcomp_miss) 
			 & (IData)(vlTOPp->icbiu_ack_i))) {
			__Vdly__v__DOT__or1200_ic_fsm__DOT__saved_addr_r 
			    = ((0xfffffff3 & __Vdly__v__DOT__or1200_ic_fsm__DOT__saved_addr_r) 
			       | (0xc & (((IData)(1) 
					  + (vlTOPp->v__DOT__or1200_ic_fsm__DOT__saved_addr_r 
					     >> 2)) 
					 << 2)));
			++(vlSymsp->__Vcoverage[9]);
			__Vdly__v__DOT__or1200_ic_fsm__DOT__state = 2;
			__Vdly__v__DOT__or1200_ic_fsm__DOT__hitmiss_eval = 0;
			__Vdly__v__DOT__or1200_ic_fsm__DOT__cnt = 2;
			__Vdly__v__DOT__or1200_ic_fsm__DOT__cache_inhibit = 0;
		    } else {
			if ((1 & ((~ (IData)(vlTOPp->v__DOT__tagcomp_miss)) 
				  & (~ (IData)(vlTOPp->icqmem_ci_i))))) {
			    ++(vlSymsp->__Vcoverage[10]);
			    __Vdly__v__DOT__or1200_ic_fsm__DOT__saved_addr_r 
				= vlTOPp->icqmem_adr_i;
			    __Vdly__v__DOT__or1200_ic_fsm__DOT__cache_inhibit = 0;
			} else {
			    if (vlTOPp->icqmem_cycstb_i) {
				++(vlSymsp->__Vcoverage[12]);
				__Vdly__v__DOT__or1200_ic_fsm__DOT__hitmiss_eval = 0;
			    } else {
				++(vlSymsp->__Vcoverage[11]);
				__Vdly__v__DOT__or1200_ic_fsm__DOT__state = 0;
				__Vdly__v__DOT__or1200_ic_fsm__DOT__hitmiss_eval = 0;
				vlTOPp->v__DOT__or1200_ic_fsm__DOT__load = 0;
				__Vdly__v__DOT__or1200_ic_fsm__DOT__cache_inhibit = 0;
			    }
			}
		    }
		}
	    } else {
		if ((2 == (IData)(vlTOPp->v__DOT__or1200_ic_fsm__DOT__state))) {
		    ++(vlSymsp->__Vcoverage[16]);
		    if (((IData)(vlTOPp->icbiu_ack_i) 
			 & (0 != (IData)(vlTOPp->v__DOT__or1200_ic_fsm__DOT__cnt)))) {
			__Vdly__v__DOT__or1200_ic_fsm__DOT__cnt 
			    = (7 & ((IData)(vlTOPp->v__DOT__or1200_ic_fsm__DOT__cnt) 
				    - (IData)(1)));
			++(vlSymsp->__Vcoverage[14]);
			__Vdly__v__DOT__or1200_ic_fsm__DOT__saved_addr_r 
			    = ((0xfffffff3 & __Vdly__v__DOT__or1200_ic_fsm__DOT__saved_addr_r) 
			       | (0xc & (((IData)(1) 
					  + (vlTOPp->v__DOT__or1200_ic_fsm__DOT__saved_addr_r 
					     >> 2)) 
					 << 2)));
		    } else {
			if (vlTOPp->icbiu_ack_i) {
			    ++(vlSymsp->__Vcoverage[15]);
			    __Vdly__v__DOT__or1200_ic_fsm__DOT__saved_addr_r 
				= vlTOPp->icqmem_adr_i;
			    __Vdly__v__DOT__or1200_ic_fsm__DOT__state = 0;
			    __Vdly__v__DOT__or1200_ic_fsm__DOT__hitmiss_eval = 0;
			    vlTOPp->v__DOT__or1200_ic_fsm__DOT__load = 0;
			}
		    }
		} else {
		    ++(vlSymsp->__Vcoverage[17]);
		    __Vdly__v__DOT__or1200_ic_fsm__DOT__state = 0;
		}
	    }
	}
    }
    vlTOPp->v__DOT__or1200_ic_fsm__DOT__cnt = __Vdly__v__DOT__or1200_ic_fsm__DOT__cnt;
    vlTOPp->v__DOT__or1200_ic_fsm__DOT__saved_addr_r 
	= __Vdly__v__DOT__or1200_ic_fsm__DOT__saved_addr_r;
    vlTOPp->v__DOT__or1200_ic_fsm__DOT__cache_inhibit 
	= __Vdly__v__DOT__or1200_ic_fsm__DOT__cache_inhibit;
    vlTOPp->v__DOT__or1200_ic_fsm__DOT__hitmiss_eval 
	= __Vdly__v__DOT__or1200_ic_fsm__DOT__hitmiss_eval;
    vlTOPp->v__DOT__or1200_ic_fsm__DOT__state = __Vdly__v__DOT__or1200_ic_fsm__DOT__state;
}

void Vtop::_settle__TOP__4(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_settle__TOP__4\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // ALWAYS at or1200_ic_top.v:271
    vlTOPp->v__DOT__tagcomp_miss = 1;
    vlTOPp->icqmem_err_o = ((IData)(vlTOPp->ic_en) ? 
			    ((1 == (IData)(vlTOPp->v__DOT__or1200_ic_fsm__DOT__state)) 
			     & (IData)(vlTOPp->icbiu_err_i))
			     : (IData)(vlTOPp->icbiu_err_i));
    vlTOPp->v__DOT__icfsm_first_miss_ack = ((1 == (IData)(vlTOPp->v__DOT__or1200_ic_fsm__DOT__state)) 
					    & (IData)(vlTOPp->icbiu_ack_i));
    vlTOPp->icbiu_cab_o = ((IData)(vlTOPp->ic_en) & 
			   (((1 == (IData)(vlTOPp->v__DOT__or1200_ic_fsm__DOT__state)) 
			     & (~ (IData)(vlTOPp->v__DOT__or1200_ic_fsm__DOT__cache_inhibit))) 
			    | (2 == (IData)(vlTOPp->v__DOT__or1200_ic_fsm__DOT__state))));
    vlTOPp->v__DOT__icfsm_biu_read = ((IData)(vlTOPp->v__DOT__or1200_ic_fsm__DOT__hitmiss_eval) 
				      | (IData)(vlTOPp->v__DOT__or1200_ic_fsm__DOT__load));
}

void Vtop::_combo__TOP__5(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_combo__TOP__5\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->icqmem_err_o = ((IData)(vlTOPp->ic_en) ? 
			    ((1 == (IData)(vlTOPp->v__DOT__or1200_ic_fsm__DOT__state)) 
			     & (IData)(vlTOPp->icbiu_err_i))
			     : (IData)(vlTOPp->icbiu_err_i));
    vlTOPp->v__DOT__icfsm_first_miss_ack = ((1 == (IData)(vlTOPp->v__DOT__or1200_ic_fsm__DOT__state)) 
					    & (IData)(vlTOPp->icbiu_ack_i));
    vlTOPp->icbiu_cab_o = ((IData)(vlTOPp->ic_en) & 
			   ((((1 == (IData)(vlTOPp->v__DOT__or1200_ic_fsm__DOT__state)) 
			      & (IData)(vlTOPp->v__DOT__tagcomp_miss)) 
			     & (~ (IData)(vlTOPp->v__DOT__or1200_ic_fsm__DOT__cache_inhibit))) 
			    | (2 == (IData)(vlTOPp->v__DOT__or1200_ic_fsm__DOT__state))));
    vlTOPp->icqmem_tag_o = ((IData)(vlTOPp->icqmem_err_o)
			     ? 0xb : (IData)(vlTOPp->icqmem_tag_i));
    vlTOPp->icqmem_dat_o = ((1 & ((IData)(vlTOPp->v__DOT__icfsm_first_miss_ack) 
				  | (~ (IData)(vlTOPp->ic_en))))
			     ? vlTOPp->icbiu_dat_i : 0);
    vlTOPp->icqmem_ack_o = ((IData)(vlTOPp->ic_en) ? 
			    ((((((1 == (IData)(vlTOPp->v__DOT__or1200_ic_fsm__DOT__state)) 
				 & (IData)(vlTOPp->v__DOT__or1200_ic_fsm__DOT__hitmiss_eval)) 
				& (~ (IData)(vlTOPp->v__DOT__tagcomp_miss))) 
			       & (~ (IData)(vlTOPp->v__DOT__or1200_ic_fsm__DOT__cache_inhibit))) 
			      & (~ (IData)(vlTOPp->icqmem_ci_i))) 
			     | (IData)(vlTOPp->v__DOT__icfsm_first_miss_ack))
			     : (IData)(vlTOPp->icbiu_ack_i));
}

void Vtop::_sequent__TOP__6(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_sequent__TOP__6\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->v__DOT__icfsm_biu_read = (((IData)(vlTOPp->v__DOT__or1200_ic_fsm__DOT__hitmiss_eval) 
				       & (IData)(vlTOPp->v__DOT__tagcomp_miss)) 
				      | ((~ (IData)(vlTOPp->v__DOT__or1200_ic_fsm__DOT__hitmiss_eval)) 
					 & (IData)(vlTOPp->v__DOT__or1200_ic_fsm__DOT__load)));
}

void Vtop::_settle__TOP__7(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_settle__TOP__7\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->icqmem_tag_o = ((IData)(vlTOPp->icqmem_err_o)
			     ? 0xb : (IData)(vlTOPp->icqmem_tag_i));
    vlTOPp->icqmem_dat_o = ((1 & ((IData)(vlTOPp->v__DOT__icfsm_first_miss_ack) 
				  | (~ (IData)(vlTOPp->ic_en))))
			     ? vlTOPp->icbiu_dat_i : 0);
    vlTOPp->icqmem_ack_o = ((IData)(vlTOPp->ic_en) ? (IData)(vlTOPp->v__DOT__icfsm_first_miss_ack)
			     : (IData)(vlTOPp->icbiu_ack_i));
    vlTOPp->icbiu_cyc_o = ((IData)(vlTOPp->ic_en) ? (IData)(vlTOPp->v__DOT__icfsm_biu_read)
			    : (IData)(vlTOPp->icqmem_cycstb_i));
    vlTOPp->icbiu_stb_o = ((IData)(vlTOPp->ic_en) ? (IData)(vlTOPp->v__DOT__icfsm_biu_read)
			    : (IData)(vlTOPp->icqmem_cycstb_i));
    vlTOPp->icbiu_sel_o = (((IData)(vlTOPp->ic_en) 
			    & (IData)(vlTOPp->v__DOT__icfsm_biu_read))
			    ? 0xf : (IData)(vlTOPp->icqmem_sel_i));
    vlTOPp->v__DOT__ic_addr = ((IData)(vlTOPp->v__DOT__icfsm_biu_read)
			        ? vlTOPp->v__DOT__or1200_ic_fsm__DOT__saved_addr_r
			        : vlTOPp->icqmem_adr_i);
    vlTOPp->icqmem_rty_o = (1 & ((~ (IData)(vlTOPp->icqmem_ack_o)) 
				 & (~ (IData)(vlTOPp->icqmem_err_o))));
}

void Vtop::_combo__TOP__8(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_combo__TOP__8\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->icbiu_cyc_o = ((IData)(vlTOPp->ic_en) ? (IData)(vlTOPp->v__DOT__icfsm_biu_read)
			    : (IData)(vlTOPp->icqmem_cycstb_i));
    vlTOPp->icbiu_stb_o = ((IData)(vlTOPp->ic_en) ? (IData)(vlTOPp->v__DOT__icfsm_biu_read)
			    : (IData)(vlTOPp->icqmem_cycstb_i));
    vlTOPp->icbiu_sel_o = (((IData)(vlTOPp->ic_en) 
			    & (IData)(vlTOPp->v__DOT__icfsm_biu_read))
			    ? 0xf : (IData)(vlTOPp->icqmem_sel_i));
    vlTOPp->v__DOT__ic_addr = ((IData)(vlTOPp->v__DOT__icfsm_biu_read)
			        ? vlTOPp->v__DOT__or1200_ic_fsm__DOT__saved_addr_r
			        : vlTOPp->icqmem_adr_i);
    vlTOPp->icqmem_rty_o = (1 & ((~ (IData)(vlTOPp->icqmem_ack_o)) 
				 & (~ (IData)(vlTOPp->icqmem_err_o))));
    vlTOPp->icbiu_adr_o = vlTOPp->v__DOT__ic_addr;
}

void Vtop::_settle__TOP__9(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_settle__TOP__9\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->icbiu_adr_o = vlTOPp->v__DOT__ic_addr;
}

void Vtop::_eval(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_eval\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
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
    // Body
    vlTOPp->_initial__TOP__2(vlSymsp);
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
    vlTOPp->_settle__TOP__1(vlSymsp);
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
