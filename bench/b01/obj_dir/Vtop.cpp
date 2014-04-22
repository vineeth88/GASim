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
    line1 = VL_RAND_RESET_I(1);
    line2 = VL_RAND_RESET_I(1);
    reset = VL_RAND_RESET_I(1);
    outp = VL_RAND_RESET_I(1);
    overflw = VL_RAND_RESET_I(1);
    clock = VL_RAND_RESET_I(1);
    v__DOT__process_1_stato = VL_RAND_RESET_I(3);
    __Vclklast__TOP__clock = VL_RAND_RESET_I(1);
    __Vclklast__TOP__reset = VL_RAND_RESET_I(1);
}

void Vtop::__Vconfigure(Vtop__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    // Coverage Declarations
    __vlCoverInsert(&(vlSymsp->__Vcoverage[0]), first, "top.v", 68, 0, ".v", "v_line/b01", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[1]), first, "top.v", 79, 0, ".v", "v_line/b01", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[2]), first, "top.v", 84, 0, ".v", "v_line/b01", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[3]), first, "top.v", 77, 0, ".v", "v_line/b01", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[4]), first, "top.v", 92, 0, ".v", "v_line/b01", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[5]), first, "top.v", 97, 0, ".v", "v_line/b01", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[6]), first, "top.v", 90, 0, ".v", "v_line/b01", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[7]), first, "top.v", 105, 0, ".v", "v_line/b01", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[8]), first, "top.v", 110, 0, ".v", "v_line/b01", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[9]), first, "top.v", 103, 0, ".v", "v_line/b01", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[10]), first, "top.v", 118, 0, ".v", "v_line/b01", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[11]), first, "top.v", 123, 0, ".v", "v_line/b01", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[12]), first, "top.v", 116, 0, ".v", "v_line/b01", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[13]), first, "top.v", 131, 0, ".v", "v_line/b01", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[14]), first, "top.v", 136, 0, ".v", "v_line/b01", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[15]), first, "top.v", 129, 0, ".v", "v_line/b01", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[16]), first, "top.v", 144, 0, ".v", "v_line/b01", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[17]), first, "top.v", 149, 0, ".v", "v_line/b01", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[18]), first, "top.v", 142, 0, ".v", "v_line/b01", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[19]), first, "top.v", 157, 0, ".v", "v_line/b01", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[20]), first, "top.v", 162, 0, ".v", "v_line/b01", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[21]), first, "top.v", 155, 0, ".v", "v_line/b01", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[22]), first, "top.v", 170, 0, ".v", "v_line/b01", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[23]), first, "top.v", 175, 0, ".v", "v_line/b01", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[24]), first, "top.v", 168, 0, ".v", "v_line/b01", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[25]), first, "top.v", 75, 0, ".v", "v_line/b01", "else");
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

void Vtop::_sequent__TOP__1(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_sequent__TOP__1\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // ALWAYS at top.v:66
    if (vlTOPp->reset) {
	++(vlSymsp->__Vcoverage[0]);
	vlTOPp->v__DOT__process_1_stato = 0;
	vlTOPp->outp = 0;
	vlTOPp->overflw = 0;
    } else {
	++(vlSymsp->__Vcoverage[25]);
	if (((((((((0 == (IData)(vlTOPp->v__DOT__process_1_stato)) 
		   | (3 == (IData)(vlTOPp->v__DOT__process_1_stato))) 
		  | (1 == (IData)(vlTOPp->v__DOT__process_1_stato))) 
		 | (4 == (IData)(vlTOPp->v__DOT__process_1_stato))) 
		| (2 == (IData)(vlTOPp->v__DOT__process_1_stato))) 
	       | (5 == (IData)(vlTOPp->v__DOT__process_1_stato))) 
	      | (6 == (IData)(vlTOPp->v__DOT__process_1_stato))) 
	     | (7 == (IData)(vlTOPp->v__DOT__process_1_stato)))) {
	    if ((0 == (IData)(vlTOPp->v__DOT__process_1_stato))) {
		++(vlSymsp->__Vcoverage[3]);
		if (((IData)(vlTOPp->line1) & (IData)(vlTOPp->line2))) {
		    ++(vlSymsp->__Vcoverage[1]);
		    vlTOPp->v__DOT__process_1_stato = 4;
		} else {
		    ++(vlSymsp->__Vcoverage[2]);
		    vlTOPp->v__DOT__process_1_stato = 1;
		}
		vlTOPp->outp = ((IData)(vlTOPp->line1) 
				^ (IData)(vlTOPp->line2));
		vlTOPp->overflw = 0;
	    } else {
		if ((3 == (IData)(vlTOPp->v__DOT__process_1_stato))) {
		    ++(vlSymsp->__Vcoverage[6]);
		    if (((IData)(vlTOPp->line1) & (IData)(vlTOPp->line2))) {
			++(vlSymsp->__Vcoverage[4]);
			vlTOPp->v__DOT__process_1_stato = 4;
		    } else {
			++(vlSymsp->__Vcoverage[5]);
			vlTOPp->v__DOT__process_1_stato = 1;
		    }
		    vlTOPp->outp = ((IData)(vlTOPp->line1) 
				    ^ (IData)(vlTOPp->line2));
		    vlTOPp->overflw = 1;
		} else {
		    if ((1 == (IData)(vlTOPp->v__DOT__process_1_stato))) {
			++(vlSymsp->__Vcoverage[9]);
			if (((IData)(vlTOPp->line1) 
			     & (IData)(vlTOPp->line2))) {
			    ++(vlSymsp->__Vcoverage[7]);
			    vlTOPp->v__DOT__process_1_stato = 5;
			} else {
			    ++(vlSymsp->__Vcoverage[8]);
			    vlTOPp->v__DOT__process_1_stato = 2;
			}
			vlTOPp->outp = ((IData)(vlTOPp->line1) 
					^ (IData)(vlTOPp->line2));
			vlTOPp->overflw = 0;
		    } else {
			if ((4 == (IData)(vlTOPp->v__DOT__process_1_stato))) {
			    ++(vlSymsp->__Vcoverage[12]);
			    if (((IData)(vlTOPp->line1) 
				 | (IData)(vlTOPp->line2))) {
				++(vlSymsp->__Vcoverage[10]);
				vlTOPp->v__DOT__process_1_stato = 5;
			    } else {
				++(vlSymsp->__Vcoverage[11]);
				vlTOPp->v__DOT__process_1_stato = 2;
			    }
			    vlTOPp->outp = (1 & (~ 
						 ((IData)(vlTOPp->line1) 
						  ^ (IData)(vlTOPp->line2))));
			    vlTOPp->overflw = 0;
			} else {
			    if ((2 == (IData)(vlTOPp->v__DOT__process_1_stato))) {
				++(vlSymsp->__Vcoverage[15]);
				if (((IData)(vlTOPp->line1) 
				     & (IData)(vlTOPp->line2))) {
				    ++(vlSymsp->__Vcoverage[13]);
				    vlTOPp->v__DOT__process_1_stato = 7;
				} else {
				    ++(vlSymsp->__Vcoverage[14]);
				    vlTOPp->v__DOT__process_1_stato = 6;
				}
				vlTOPp->outp = ((IData)(vlTOPp->line1) 
						^ (IData)(vlTOPp->line2));
				vlTOPp->overflw = 0;
			    } else {
				if ((5 == (IData)(vlTOPp->v__DOT__process_1_stato))) {
				    ++(vlSymsp->__Vcoverage[18]);
				    if (((IData)(vlTOPp->line1) 
					 | (IData)(vlTOPp->line2))) {
					++(vlSymsp->__Vcoverage[16]);
					vlTOPp->v__DOT__process_1_stato = 7;
				    } else {
					++(vlSymsp->__Vcoverage[17]);
					vlTOPp->v__DOT__process_1_stato = 6;
				    }
				    vlTOPp->outp = 
					(1 & (~ ((IData)(vlTOPp->line1) 
						 ^ (IData)(vlTOPp->line2))));
				    vlTOPp->overflw = 0;
				} else {
				    if ((6 == (IData)(vlTOPp->v__DOT__process_1_stato))) {
					++(vlSymsp->__Vcoverage[21]);
					if (((IData)(vlTOPp->line1) 
					     & (IData)(vlTOPp->line2))) {
					    ++(vlSymsp->__Vcoverage[19]);
					    vlTOPp->v__DOT__process_1_stato = 3;
					} else {
					    ++(vlSymsp->__Vcoverage[20]);
					    vlTOPp->v__DOT__process_1_stato = 0;
					}
					vlTOPp->outp 
					    = ((IData)(vlTOPp->line1) 
					       ^ (IData)(vlTOPp->line2));
					vlTOPp->overflw = 0;
				    } else {
					++(vlSymsp->__Vcoverage[24]);
					if (((IData)(vlTOPp->line1) 
					     | (IData)(vlTOPp->line2))) {
					    ++(vlSymsp->__Vcoverage[22]);
					    vlTOPp->v__DOT__process_1_stato = 3;
					} else {
					    ++(vlSymsp->__Vcoverage[23]);
					    vlTOPp->v__DOT__process_1_stato = 0;
					}
					vlTOPp->outp 
					    = (1 & 
					       (~ ((IData)(vlTOPp->line1) 
						   ^ (IData)(vlTOPp->line2))));
					vlTOPp->overflw = 0;
				    }
				}
			    }
			}
		    }
		}
	    }
	}
    }
}

void Vtop::_eval(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_eval\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if ((((IData)(vlTOPp->clock) & (~ (IData)(vlTOPp->__Vclklast__TOP__clock))) 
	 | ((IData)(vlTOPp->reset) & (~ (IData)(vlTOPp->__Vclklast__TOP__reset))))) {
	vlTOPp->_sequent__TOP__1(vlSymsp);
    }
    // Final
    vlTOPp->__Vclklast__TOP__clock = vlTOPp->clock;
    vlTOPp->__Vclklast__TOP__reset = vlTOPp->reset;
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
}

IData Vtop::_change_request(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_change_request\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    IData __req = false;  // Logically a bool
    return __req;
}
