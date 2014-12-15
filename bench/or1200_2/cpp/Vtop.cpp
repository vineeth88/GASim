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
    clmode = VL_RAND_RESET_I(2);
    wb_clk_i = VL_RAND_RESET_I(1);
    wb_rst_i = VL_RAND_RESET_I(1);
    wb_ack_i = VL_RAND_RESET_I(1);
    wb_err_i = VL_RAND_RESET_I(1);
    wb_rty_i = VL_RAND_RESET_I(1);
    wb_dat_i = VL_RAND_RESET_I(32);
    wb_cyc_o = VL_RAND_RESET_I(1);
    wb_adr_o = VL_RAND_RESET_I(32);
    wb_stb_o = VL_RAND_RESET_I(1);
    wb_we_o = VL_RAND_RESET_I(1);
    wb_sel_o = VL_RAND_RESET_I(4);
    wb_dat_o = VL_RAND_RESET_I(32);
    wb_cab_o = VL_RAND_RESET_I(1);
    biu_dat_i = VL_RAND_RESET_I(32);
    biu_adr_i = VL_RAND_RESET_I(32);
    biu_cyc_i = VL_RAND_RESET_I(1);
    biu_stb_i = VL_RAND_RESET_I(1);
    biu_we_i = VL_RAND_RESET_I(1);
    biu_cab_i = VL_RAND_RESET_I(1);
    biu_sel_i = VL_RAND_RESET_I(4);
    biu_dat_o = VL_RAND_RESET_I(32);
    biu_ack_o = VL_RAND_RESET_I(1);
    biu_err_o = VL_RAND_RESET_I(1);
    v__DOT__valid_div = VL_RAND_RESET_I(2);
    v__DOT__aborted = VL_RAND_RESET_I(1);
    v__DOT__aborted_r = VL_RAND_RESET_I(1);
    __Vclklast__TOP__wb_clk_i = VL_RAND_RESET_I(1);
    __Vclklast__TOP__wb_rst_i = VL_RAND_RESET_I(1);
    __Vclklast__TOP__clk = VL_RAND_RESET_I(1);
    __Vclklast__TOP__rst = VL_RAND_RESET_I(1);
}

void Vtop::__Vconfigure(Vtop__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    // Coverage Declarations
    __vlCoverInsert(&(vlSymsp->__Vcoverage[0]), first, "or1200_wb_biu.v", 216, 0, ".v", "v_line/or1200_wb_biu", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[1]), first, "or1200_wb_biu.v", 218, 0, ".v", "v_line/or1200_wb_biu", "elsif");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[2]), first, "or1200_wb_biu.v", 242, 0, ".v", "v_line/or1200_wb_biu", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[3]), first, "or1200_wb_biu.v", 244, 0, ".v", "v_line/or1200_wb_biu", "elsif");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[4]), first, "or1200_wb_biu.v", 256, 0, ".v", "v_line/or1200_wb_biu", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[5]), first, "or1200_wb_biu.v", 259, 0, ".v", "v_line/or1200_wb_biu", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[6]), first, "or1200_wb_biu.v", 353, 0, ".v", "v_line/or1200_wb_biu", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[7]), first, "or1200_wb_biu.v", 355, 0, ".v", "v_line/or1200_wb_biu", "elsif");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[8]), first, "or1200_wb_biu.v", 357, 0, ".v", "v_line/or1200_wb_biu", "elsif");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[9]), first, "or1200_wb_biu.v", 369, 0, ".v", "v_line/or1200_wb_biu", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[10]), first, "or1200_wb_biu.v", 375, 0, ".v", "v_line/or1200_wb_biu", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[11]), first, "or1200_wb_biu.v", 390, 0, ".v", "v_line/or1200_wb_biu", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[12]), first, "or1200_wb_biu.v", 393, 0, ".v", "v_line/or1200_wb_biu", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[13]), first, "or1200_wb_biu.v", 403, 0, ".v", "v_line/or1200_wb_biu", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[14]), first, "or1200_wb_biu.v", 406, 0, ".v", "v_line/or1200_wb_biu", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[15]), first, "or1200_wb_biu.v", 416, 0, ".v", "v_line/or1200_wb_biu", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[16]), first, "or1200_wb_biu.v", 419, 0, ".v", "v_line/or1200_wb_biu", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[17]), first, "or1200_wb_biu.v", 430, 0, ".v", "v_line/or1200_wb_biu", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[18]), first, "or1200_wb_biu.v", 433, 0, ".v", "v_line/or1200_wb_biu", "else");
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
    vlTOPp->biu_dat_o = vlTOPp->wb_dat_i;
}

void Vtop::_sequent__TOP__3(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_sequent__TOP__3\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    VL_SIG8(__Vdly__wb_we_o,0,0);
    //char	__VpadToAlign25[3];
    // Body
    __Vdly__wb_we_o = vlTOPp->wb_we_o;
    // ALWAYS at or1200_wb_biu.v:415
    if (vlTOPp->wb_rst_i) {
	++(vlSymsp->__Vcoverage[15]);
	vlTOPp->wb_sel_o = 0;
    } else {
	++(vlSymsp->__Vcoverage[16]);
	vlTOPp->wb_sel_o = vlTOPp->biu_sel_i;
    }
    // ALWAYS at or1200_wb_biu.v:429
    if (vlTOPp->wb_rst_i) {
	++(vlSymsp->__Vcoverage[17]);
	vlTOPp->wb_cab_o = 0;
    } else {
	++(vlSymsp->__Vcoverage[18]);
	vlTOPp->wb_cab_o = vlTOPp->biu_cab_i;
    }
    // ALWAYS at or1200_wb_biu.v:368
    if (vlTOPp->wb_rst_i) {
	++(vlSymsp->__Vcoverage[9]);
	vlTOPp->wb_cyc_o = 0;
    } else {
	++(vlSymsp->__Vcoverage[10]);
	vlTOPp->wb_cyc_o = ((((IData)(vlTOPp->biu_cyc_i) 
			      & (~ (IData)(vlTOPp->wb_ack_i))) 
			     | (IData)(vlTOPp->biu_cab_i)) 
			    | ((IData)(vlTOPp->v__DOT__aborted) 
			       & (~ (IData)(vlTOPp->wb_ack_i))));
    }
    // ALWAYS at or1200_wb_biu.v:402
    if (vlTOPp->wb_rst_i) {
	++(vlSymsp->__Vcoverage[13]);
	__Vdly__wb_we_o = 0;
    } else {
	__Vdly__wb_we_o = ((((IData)(vlTOPp->biu_cyc_i) 
			     & (IData)(vlTOPp->biu_stb_i)) 
			    & (IData)(vlTOPp->biu_we_i)) 
			   | ((IData)(vlTOPp->v__DOT__aborted) 
			      & (IData)(vlTOPp->wb_we_o)));
	++(vlSymsp->__Vcoverage[14]);
    }
    // ALWAYS at or1200_wb_biu.v:241
    if (vlTOPp->wb_rst_i) {
	++(vlSymsp->__Vcoverage[2]);
	vlTOPp->wb_dat_o = 0;
    } else {
	if (((((IData)(vlTOPp->biu_cyc_i) & (IData)(vlTOPp->biu_stb_i)) 
	      & (~ (IData)(vlTOPp->wb_ack_i))) & (~ (IData)(vlTOPp->v__DOT__aborted)))) {
	    ++(vlSymsp->__Vcoverage[3]);
	    vlTOPp->wb_dat_o = vlTOPp->biu_dat_i;
	}
    }
    // ALWAYS at or1200_wb_biu.v:352
    if (vlTOPp->wb_rst_i) {
	++(vlSymsp->__Vcoverage[6]);
	vlTOPp->v__DOT__aborted_r = 0;
    } else {
	if (((IData)(vlTOPp->wb_ack_i) | (IData)(vlTOPp->wb_err_i))) {
	    ++(vlSymsp->__Vcoverage[7]);
	    vlTOPp->v__DOT__aborted_r = 0;
	} else {
	    if (vlTOPp->v__DOT__aborted) {
		++(vlSymsp->__Vcoverage[8]);
		vlTOPp->v__DOT__aborted_r = 1;
	    }
	}
    }
    // ALWAYS at or1200_wb_biu.v:215
    if (vlTOPp->wb_rst_i) {
	++(vlSymsp->__Vcoverage[0]);
	vlTOPp->wb_adr_o = 0;
    } else {
	if ((((((IData)(vlTOPp->biu_cyc_i) & (IData)(vlTOPp->biu_stb_i)) 
	       & (~ (IData)(vlTOPp->wb_ack_i))) & (~ (IData)(vlTOPp->v__DOT__aborted))) 
	     & (~ ((IData)(vlTOPp->wb_stb_o) & (~ (IData)(vlTOPp->wb_ack_i)))))) {
	    ++(vlSymsp->__Vcoverage[1]);
	    vlTOPp->wb_adr_o = vlTOPp->biu_adr_i;
	}
    }
    vlTOPp->wb_we_o = __Vdly__wb_we_o;
    // ALWAYS at or1200_wb_biu.v:389
    if (vlTOPp->wb_rst_i) {
	++(vlSymsp->__Vcoverage[11]);
	vlTOPp->wb_stb_o = 0;
    } else {
	++(vlSymsp->__Vcoverage[12]);
	vlTOPp->wb_stb_o = ((((IData)(vlTOPp->biu_cyc_i) 
			      & (IData)(vlTOPp->biu_stb_i)) 
			     & (~ (IData)(vlTOPp->wb_ack_i))) 
			    | ((IData)(vlTOPp->v__DOT__aborted) 
			       & (~ (IData)(vlTOPp->wb_ack_i))));
    }
}

void Vtop::_sequent__TOP__4(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_sequent__TOP__4\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    VL_SIG8(__Vdly__v__DOT__valid_div,1,0);
    //char	__VpadToAlign49[3];
    // Body
    __Vdly__v__DOT__valid_div = vlTOPp->v__DOT__valid_div;
    // ALWAYS at or1200_wb_biu.v:255
    if (vlTOPp->rst) {
	++(vlSymsp->__Vcoverage[4]);
	__Vdly__v__DOT__valid_div = 0;
    } else {
	__Vdly__v__DOT__valid_div = (3 & ((IData)(1) 
					  + (IData)(vlTOPp->v__DOT__valid_div)));
	++(vlSymsp->__Vcoverage[5]);
    }
    vlTOPp->v__DOT__valid_div = __Vdly__v__DOT__valid_div;
}

void Vtop::_combo__TOP__5(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_combo__TOP__5\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->v__DOT__aborted = (((IData)(vlTOPp->wb_stb_o) 
				& (~ ((IData)(vlTOPp->biu_cyc_i) 
				      & (IData)(vlTOPp->biu_stb_i)))) 
			       & (~ ((IData)(vlTOPp->wb_ack_i) 
				     | (IData)(vlTOPp->wb_err_i))));
    vlTOPp->biu_ack_o = (((IData)(vlTOPp->wb_ack_i) 
			  & (~ (IData)(vlTOPp->v__DOT__aborted_r))) 
			 & ((IData)(vlTOPp->v__DOT__valid_div) 
			    | (~ (IData)(vlTOPp->clmode))));
    vlTOPp->biu_err_o = (((IData)(vlTOPp->wb_err_i) 
			  & (~ (IData)(vlTOPp->v__DOT__aborted_r))) 
			 & ((IData)(vlTOPp->v__DOT__valid_div) 
			    | (~ (IData)(vlTOPp->clmode))));
}

void Vtop::_eval(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_eval\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_combo__TOP__1(vlSymsp);
    if ((((IData)(vlTOPp->wb_clk_i) & (~ (IData)(vlTOPp->__Vclklast__TOP__wb_clk_i))) 
	 | ((IData)(vlTOPp->wb_rst_i) & (~ (IData)(vlTOPp->__Vclklast__TOP__wb_rst_i))))) {
	vlTOPp->_sequent__TOP__3(vlSymsp);
    }
    if ((((IData)(vlTOPp->clk) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk))) 
	 | ((IData)(vlTOPp->rst) & (~ (IData)(vlTOPp->__Vclklast__TOP__rst))))) {
	vlTOPp->_sequent__TOP__4(vlSymsp);
    }
    vlTOPp->_combo__TOP__5(vlSymsp);
    // Final
    vlTOPp->__Vclklast__TOP__wb_clk_i = vlTOPp->wb_clk_i;
    vlTOPp->__Vclklast__TOP__wb_rst_i = vlTOPp->wb_rst_i;
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
    vlTOPp->_combo__TOP__5(vlSymsp);
}

IData Vtop::_change_request(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_change_request\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    IData __req = false;  // Logically a bool
    return __req;
}
