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
    clock = VL_RAND_RESET_I(1);
    reset = VL_RAND_RESET_I(1);
    x_in = VL_RAND_RESET_I(6);
    stbi = VL_RAND_RESET_I(1);
    x_out = VL_RAND_RESET_I(6);
    v__DOT__r_in = VL_RAND_RESET_I(6);
    v__DOT__stato = VL_RAND_RESET_I(4);
    v__DOT__cont = VL_RAND_RESET_I(6);
    v__DOT__cont1 = VL_RAND_RESET_I(32);
    __Vclklast__TOP__clock = VL_RAND_RESET_I(1);
}

void Vtop::__Vconfigure(Vtop__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    // Coverage Declarations
    __vlCoverInsert(&(vlSymsp->__Vcoverage[0]), first, "b11.v", 64, 0, ".v", "v_line/b11", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[1]), first, "b11.v", 75, 0, ".v", "v_line/b11", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[2]), first, "b11.v", 86, 0, ".v", "v_line/b11", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[3]), first, "b11.v", 89, 0, ".v", "v_line/b11", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[4]), first, "b11.v", 83, 0, ".v", "v_line/b11", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[5]), first, "b11.v", 96, 0, ".v", "v_line/b11", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[6]), first, "b11.v", 99, 0, ".v", "v_line/b11", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[7]), first, "b11.v", 94, 0, ".v", "v_line/b11", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[8]), first, "b11.v", 103, 0, ".v", "v_line/b11", "elsif");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[9]), first, "b11.v", 106, 0, ".v", "v_line/b11", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[10]), first, "b11.v", 92, 0, ".v", "v_line/b11", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[11]), first, "b11.v", 111, 0, ".v", "v_line/b11", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[12]), first, "b11.v", 114, 0, ".v", "v_line/b11", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[13]), first, "b11.v", 109, 0, ".v", "v_line/b11", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[14]), first, "b11.v", 120, 0, ".v", "v_line/b11", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[15]), first, "b11.v", 126, 0, ".v", "v_line/b11", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[16]), first, "b11.v", 118, 0, ".v", "v_line/b11", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[17]), first, "b11.v", 134, 0, ".v", "v_line/b11", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[18]), first, "b11.v", 140, 0, ".v", "v_line/b11", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[19]), first, "b11.v", 132, 0, ".v", "v_line/b11", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[20]), first, "b11.v", 145, 0, ".v", "v_line/b11", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[21]), first, "b11.v", 151, 0, ".v", "v_line/b11", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[22]), first, "b11.v", 143, 0, ".v", "v_line/b11", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[23]), first, "b11.v", 156, 0, ".v", "v_line/b11", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[24]), first, "b11.v", 158, 0, ".v", "v_line/b11", "elsif");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[25]), first, "b11.v", 160, 0, ".v", "v_line/b11", "elsif");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[26]), first, "b11.v", 163, 0, ".v", "v_line/b11", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[27]), first, "b11.v", 154, 0, ".v", "v_line/b11", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[28]), first, "b11.v", 169, 0, ".v", "v_line/b11", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[29]), first, "b11.v", 172, 0, ".v", "v_line/b11", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[30]), first, "b11.v", 167, 0, ".v", "v_line/b11", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[31]), first, "b11.v", 178, 0, ".v", "v_line/b11", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[32]), first, "b11.v", 73, 0, ".v", "v_line/b11", "else");
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
    // ALWAYS at b11.v:62
    if (vlTOPp->reset) {
	++(vlSymsp->__Vcoverage[0]);
	vlTOPp->v__DOT__stato = 0;
	vlTOPp->v__DOT__r_in = 0;
	vlTOPp->v__DOT__cont = 0;
	vlTOPp->v__DOT__cont1 = 0;
	vlTOPp->x_out = 0;
    } else {
	++(vlSymsp->__Vcoverage[32]);
	if ((8 & (IData)(vlTOPp->v__DOT__stato))) {
	    if ((4 & (IData)(vlTOPp->v__DOT__stato))) {
		++(vlSymsp->__Vcoverage[31]);
		vlTOPp->v__DOT__stato = 0;
	    } else {
		if ((2 & (IData)(vlTOPp->v__DOT__stato))) {
		    ++(vlSymsp->__Vcoverage[31]);
		    vlTOPp->v__DOT__stato = 0;
		} else {
		    if ((1 & (IData)(vlTOPp->v__DOT__stato))) {
			++(vlSymsp->__Vcoverage[31]);
			vlTOPp->v__DOT__stato = 0;
		    } else {
			++(vlSymsp->__Vcoverage[30]);
			if (VL_GTS_III(1,32,32, 0, vlTOPp->v__DOT__cont1)) {
			    ++(vlSymsp->__Vcoverage[28]);
			    vlTOPp->x_out = (0x3f & 
					     VL_MODDIVS_III(32, 
							    VL_NEGATE_I(vlTOPp->v__DOT__cont1), (IData)(0x40)));
			} else {
			    ++(vlSymsp->__Vcoverage[29]);
			    vlTOPp->x_out = (0x3f & 
					     VL_MODDIVS_III(32, vlTOPp->v__DOT__cont1, (IData)(0x40)));
			}
			vlTOPp->v__DOT__stato = 1;
		    }
		}
	    }
	} else {
	    if ((4 & (IData)(vlTOPp->v__DOT__stato))) {
		if ((2 & (IData)(vlTOPp->v__DOT__stato))) {
		    if ((1 & (IData)(vlTOPp->v__DOT__stato))) {
			if ((0 == VL_MODDIV_III(32, 
						((IData)(vlTOPp->v__DOT__r_in) 
						 >> 2), (IData)(4)))) {
			    vlTOPp->v__DOT__cont1 = 
				(vlTOPp->v__DOT__cont1 
				 - (IData)(0x15));
			    ++(vlSymsp->__Vcoverage[23]);
			} else {
			    if ((1 == VL_MODDIV_III(32, 
						    ((IData)(vlTOPp->v__DOT__r_in) 
						     >> 2), (IData)(4)))) {
				vlTOPp->v__DOT__cont1 
				    = (vlTOPp->v__DOT__cont1 
				       - (IData)(0x2a));
				++(vlSymsp->__Vcoverage[24]);
			    } else {
				if ((2 == VL_MODDIV_III(32, 
							((IData)(vlTOPp->v__DOT__r_in) 
							 >> 2), (IData)(4)))) {
				    vlTOPp->v__DOT__cont1 
					= ((IData)(7) 
					   + vlTOPp->v__DOT__cont1);
				    ++(vlSymsp->__Vcoverage[25]);
				} else {
				    vlTOPp->v__DOT__cont1 
					= ((IData)(0x1c) 
					   + vlTOPp->v__DOT__cont1);
				    ++(vlSymsp->__Vcoverage[26]);
				}
			    }
			}
			++(vlSymsp->__Vcoverage[27]);
			vlTOPp->v__DOT__stato = 8;
		    } else {
			++(vlSymsp->__Vcoverage[22]);
			if (VL_LTS_III(1,32,32, 0x3f, vlTOPp->v__DOT__cont1)) {
			    vlTOPp->v__DOT__cont1 = 
				((IData)(0x1a) + vlTOPp->v__DOT__cont1);
			    ++(vlSymsp->__Vcoverage[20]);
			    vlTOPp->v__DOT__stato = 6;
			} else {
			    ++(vlSymsp->__Vcoverage[21]);
			    vlTOPp->v__DOT__stato = 7;
			}
		    }
		} else {
		    if ((1 & (IData)(vlTOPp->v__DOT__stato))) {
			++(vlSymsp->__Vcoverage[19]);
			if (VL_LTS_III(1,32,32, 0x1a, vlTOPp->v__DOT__cont1)) {
			    vlTOPp->v__DOT__cont1 = 
				(vlTOPp->v__DOT__cont1 
				 - (IData)(0x1a));
			    ++(vlSymsp->__Vcoverage[17]);
			    vlTOPp->v__DOT__stato = 5;
			} else {
			    ++(vlSymsp->__Vcoverage[18]);
			    vlTOPp->v__DOT__stato = 7;
			}
		    } else {
			++(vlSymsp->__Vcoverage[16]);
			if ((1 == (VL_MODDIV_III(32, (IData)(vlTOPp->v__DOT__r_in), (IData)(4)) 
				   >> 1))) {
			    vlTOPp->v__DOT__cont1 = 
				((IData)(vlTOPp->v__DOT__r_in) 
				 + vlTOPp->v__DOT__cont1);
			    ++(vlSymsp->__Vcoverage[14]);
			    vlTOPp->v__DOT__stato = 5;
			} else {
			    vlTOPp->v__DOT__cont1 = 
				((IData)(vlTOPp->v__DOT__r_in) 
				 - vlTOPp->v__DOT__cont1);
			    ++(vlSymsp->__Vcoverage[15]);
			    vlTOPp->v__DOT__stato = 6;
			}
		    }
		}
	    } else {
		if ((2 & (IData)(vlTOPp->v__DOT__stato))) {
		    if ((1 & (IData)(vlTOPp->v__DOT__stato))) {
			++(vlSymsp->__Vcoverage[13]);
			if ((1 == VL_MODDIV_III(32, (IData)(vlTOPp->v__DOT__r_in), (IData)(2)))) {
			    ++(vlSymsp->__Vcoverage[11]);
			    vlTOPp->v__DOT__cont1 = 
				((IData)(vlTOPp->v__DOT__cont) 
				 << 1);
			} else {
			    ++(vlSymsp->__Vcoverage[12]);
			    vlTOPp->v__DOT__cont1 = vlTOPp->v__DOT__cont;
			}
			vlTOPp->v__DOT__stato = 4;
		    } else {
			++(vlSymsp->__Vcoverage[10]);
			if (((0 == (IData)(vlTOPp->v__DOT__r_in)) 
			     | (0x3f == (IData)(vlTOPp->v__DOT__r_in)))) {
			    if ((0x19 > (IData)(vlTOPp->v__DOT__cont))) {
				vlTOPp->v__DOT__cont 
				    = (0x3f & ((IData)(1) 
					       + (IData)(vlTOPp->v__DOT__cont)));
				++(vlSymsp->__Vcoverage[5]);
			    } else {
				++(vlSymsp->__Vcoverage[6]);
				vlTOPp->v__DOT__cont = 0;
			    }
			    ++(vlSymsp->__Vcoverage[7]);
			    vlTOPp->v__DOT__cont1 = vlTOPp->v__DOT__r_in;
			    vlTOPp->v__DOT__stato = 8;
			} else {
			    if ((0x1a >= (IData)(vlTOPp->v__DOT__r_in))) {
				++(vlSymsp->__Vcoverage[8]);
				vlTOPp->v__DOT__stato = 3;
			    } else {
				++(vlSymsp->__Vcoverage[9]);
				vlTOPp->v__DOT__stato = 1;
			    }
			}
		    }
		} else {
		    if ((1 & (IData)(vlTOPp->v__DOT__stato))) {
			++(vlSymsp->__Vcoverage[4]);
			vlTOPp->v__DOT__r_in = vlTOPp->x_in;
			if (vlTOPp->stbi) {
			    ++(vlSymsp->__Vcoverage[2]);
			    vlTOPp->v__DOT__stato = 1;
			} else {
			    ++(vlSymsp->__Vcoverage[3]);
			    vlTOPp->v__DOT__stato = 2;
			}
		    } else {
			++(vlSymsp->__Vcoverage[1]);
			vlTOPp->v__DOT__cont = 0;
			vlTOPp->v__DOT__r_in = vlTOPp->x_in;
			vlTOPp->v__DOT__stato = 1;
			vlTOPp->x_out = 0;
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
    if (((IData)(vlTOPp->clock) & (~ (IData)(vlTOPp->__Vclklast__TOP__clock)))) {
	vlTOPp->_sequent__TOP__1(vlSymsp);
    }
    // Final
    vlTOPp->__Vclklast__TOP__clock = vlTOPp->clock;
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
