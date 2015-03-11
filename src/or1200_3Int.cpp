#include "or1200_3Int.h"

using namespace std;

keyVal_t state_t :: getHash() {
	// TODO
	keyVal_t hash_value = state_val; 
//		state_val.substr(0,32) +  // id_except 
//		state_val.substr(32,1) +  // ex_except
//		state_val.substr(33,1) +  // state
//		state_val.substr(34,1) +  // ex_dslot
//		state_val.substr(35,32) +  //except_flush
//		state_val.substr(67,2) +  // delayed_iee
//		state_val.substr(69,3) +  // delayed_tee
//		state_val.substr(72,1) +  // int_pending
//		state_val.substr(73,1) +  // tick_pending
//		state_val.substr(74,1) +  // except_trig
//		state_val.substr(74,1) +  // ex_pc
//		state_val.substr(74,1);   // wb_pc

	return hash_value;               
}

state_t::state_t() {
	
	branch_index = std::vector<int>();
	state_index = -1;
	hit_count = 0;
	
	state_val = string(CONST_NUM_STATE_BITS, '0');

	mem_alloc_cnt++;
}

state_t::state_t(const state_t& copy_obj) {

	state_index = copy_obj.state_index;
	state_val = copy_obj.state_val;

	branch_index = copy_obj.branch_index;
	hit_count = copy_obj.hit_count;

	pIndiv = copy_obj.pIndiv;
	state_fitness = copy_obj.state_fitness;

	mem_alloc_cnt++;
}

state_t::state_t(const Vtop* copy_obj, int index_) :
	state_index(index_)
{
	stringstream ss;
	ss
    	<< (bitset<3>)	copy_obj->v__DOT__id_exceptflags
    	<< (bitset<3>)	copy_obj->v__DOT__ex_exceptflags
    	<< (bitset<3>)	copy_obj->v__DOT__state
    	<< (bitset<1>)	copy_obj->v__DOT__ex_dslot
    	<< (bitset<1>)	copy_obj->v__DOT__except_flushpipe
    	<< (bitset<3>)	copy_obj->v__DOT__delayed_iee
    	<< (bitset<3>)	copy_obj->v__DOT__delayed_tee
    	<< (bitset<1>)	copy_obj->v__DOT__int_pending
    	<< (bitset<1>)	copy_obj->v__DOT__tick_pending
    	<< (bitset<12>)	copy_obj->v__DOT__except_trig
    	<< (bitset<31>)	copy_obj->v__DOT__ex_pc
    	<< (bitset<31>)	copy_obj->v__DOT__wb_pc;

	ss >> state_val;

	GetCoverage(copy_obj, branch_index);

	mem_alloc_cnt++;
}

state_t::~state_t() {
	state_index = -1;
	hit_count = 0;

	//state_val = "INVALID STATE";
	branch_index.clear();

	mem_alloc_cnt--;
	#ifdef _DBG_DEST_CALL_
	cout << endl << "Deleted state_t " << mem_alloc_cnt << endl;
	#endif
}

state_t& state_t::operator=(const state_t& copy_obj) {   
    if (this != &copy_obj) {

		state_index = copy_obj.state_index;
		branch_index = copy_obj.branch_index;
		hit_count = copy_obj.hit_count;
		state_val = copy_obj.state_val;
	}

	return *this;
}        

bool state_t::operator==(const state_t& lhs) {   
	return (state_val.compare(lhs.state_val) == 0);
}

//string state_t::operator[] (int index_) {
//	assert(index_ < CONST_NUM_VARS);
//	string sstr;
//
//	switch(index_) {
//		case 0 :	sstr = state_val.substr(0,32);  // addr
//					break;
//
//		case 1 :	sstr = state_val.substr(32,1);	// read
//					break;
//
//		case 2 :	sstr = state_val.substr(33,1);	// miss
//					break;
//
//		case 3 :	sstr = state_val.substr(34,1);  // miss_ack
//					break;
//
//		case 4 :	sstr = state_val.substr(35,32);  // saved_addr
//					break;
//
//		case 5 :	sstr = state_val.substr(67,2);  // state
//					break;
//
//		case 6 :	sstr = state_val.substr(69,3);  // cnt
//					break;
//
//		case 7 :	sstr = state_val.substr(72,1);  // hitmiss_eval
//					break;
//
//		case 8 :	sstr = state_val.substr(73,1);  // load
//					break;
//
//		case 9 :	sstr = state_val.substr(74,1);  // inhibit
//					break;
//
//		default:	break;
//	}
//
//	return sstr;
//}

void state_t::printState (bool full_) {
	if (full_)
    	cout << state_val << endl;
	else {
		cout
		<< " " << state_val.substr(0,3) 	// id_except 
		<< " " << state_val.substr(3,3) 	// ex_except
		<< " " << state_val.substr(6,3) 	// state
		<< " " << state_val.substr(9,1) 	// ex_dslot 
		<< " " << state_val.substr(10,1)	// except_flushpipe
		<< " " << state_val.substr(11,3) 	// delayed_iee
		<< " " << state_val.substr(14,3) 	// delayed_tee
		<< " " << state_val.substr(17,1) 	// int_pending
		<< " " << state_val.substr(18,1) 	// tick_pending
		<< " " << state_val.substr(19,13)	// except_trig
		<< " " << state_val.substr(32,32)	// ex_pc
		<< " " << state_val.substr(64,32)	// wb_pc
		<< endl;
	}
	for (vector<int>::iterator it = branch_index.begin();
			it != branch_index.end(); ++it)
		cout << *it << " ";
	cout << endl;
}

void state_t::setCktState(Vtop* top) {
	int val = 0;
	for(int i = 0; i < CONST_NUM_STATE_BITS; ++i) {

		val = val << 1;
		if (state_val[i] == '1')
			val++;
		
		switch(i+1) {
			case 3 :	top->v__DOT__id_exceptflags = val;
						val = 0;
						break;

			case 6 :	top->v__DOT__ex_exceptflags = val;
						val = 0;
						break;

			case 9 :	top->v__DOT__state = val;
						val = 0;
						break;

			case 10 :	top->v__DOT__ex_dslot = val;
						val = 0;
						break;

			case 11 :	top->v__DOT__except_flushpipe = val;
						val = 0;
						break;

			case 14 :	top->v__DOT__delayed_iee = val;
						val = 0;
						break;

			case 17 :	top->v__DOT__delayed_tee = val;
						val = 0;
						break;

			case 18 :	top->v__DOT__int_pending = val;
						val = 0;
						break;

			case 19 :	top->v__DOT__tick_pending = val;
						val = 0;
						break;

			case 32 :	top->v__DOT__except_trig = val;
						val = 0;
						break;
			
			case 64 :	top->v__DOT__ex_pc = val;
						val = 0;
						break;

			case 96 :	top->v__DOT__wb_pc = val;
						val = 0;
						break;

			default:	break;
		}

	}

}

void set_input(Vtop *top, const vecIn_t& input)	{
// TODO
	assert(input.length() == (uint)CONST_NUM_INPUT_BITS);

    top->du_dsr = 0; // 0-13
    top->to_sr = 0; // 0-15
    top->sr = 0; // 0-15
    top->if_pc = 0;
    top->datain = 0;
    top->lsu_addr = 0;

	int cnt = 0;

    top->sig_ibuserr = input[cnt++] - '0';
    top->sig_dbuserr = input[cnt++] - '0';
    top->sig_illegal = input[cnt++] - '0';
    top->sig_align = input[cnt++] - '0';
    top->sig_range = input[cnt++] - '0';
    top->sig_dtlbmiss = input[cnt++] - '0';
    top->sig_dmmufault = input[cnt++] - '0';
    top->sig_int = input[cnt++] - '0';
    top->sig_syscall = input[cnt++] - '0';
    top->sig_trap = input[cnt++] - '0';
    top->sig_itlbmiss = input[cnt++] - '0';
    top->sig_immufault = input[cnt++] - '0';
    top->sig_tick = input[cnt++] - '0';
    top->branch_taken = input[cnt++] - '0';
    top->genpc_freeze = input[cnt++] - '0';
    top->id_freeze = input[cnt++] - '0';
    top->ex_freeze = input[cnt++] - '0';
    top->wb_freeze = input[cnt++] - '0';
    top->if_stall = input[cnt++] - '0';
    top->epcr_we = input[cnt++] - '0';
    top->eear_we = input[cnt++] - '0';
    top->esr_we = input[cnt++] - '0';
    top->pc_we = input[cnt++] - '0';
    top->sr_we = input[cnt++] - '0';
    top->ex_void = input[cnt++] - '0';
    top->icpu_ack_i = input[cnt++] - '0';
    top->icpu_err_i = input[cnt++] - '0';
    top->dcpu_ack_i = input[cnt++] - '0';
    top->dcpu_err_i = input[cnt++] - '0';
    
    for (int i = 0; i < 14; i++) {
        top->du_dsr = (top->du_dsr << 1) | (input[cnt++] - '0');
    }
    for (int i = 0; i < 16; i++) {
        top->to_sr = (top->to_sr << 1) | (input[cnt++] - '0');
    }
    for (int i = 0; i < 16; i++) {
        top->sr = (top->sr << 1) | (input[cnt++] - '0');
    }
    for (int i = 0; i < 32; i++) {
        top->if_pc = (top->if_pc << 1) | (input[cnt++] - '0');
    }
    for (int i = 0; i < 32; i++) {
        top->datain = (top->datain << 1) | (input[cnt++] - '0');
    }
    for (int i = 0; i < 32; i++) {
        top->lsu_addr = (top->lsu_addr << 1) | (input[cnt++] - '0');
    }

	assert (cnt == CONST_NUM_INPUT_BITS);
}

void RandomVecIn(vecIn_t& vecIn)
{
	vecIn = string(CONST_NUM_INPUT_BITS, '0');
    for (uint i = 0; i < vecIn.length(); i++) 
		vecIn[i] = (rand() & 0x01) + 48;
    assert(vecIn.length() == (uint)CONST_NUM_INPUT_BITS);
}

void SimMultiCycle(Vtop* top, int NUM_CYCLES) {

    vecIn_t vecIn(CONST_NUM_INPUT_BITS,'0');
    init_reset_clock(top);

    bool rstFlag = false;
    int main_time = 0;
    int cycle_cnt = 0;
    int rstCycle = 10;

    while ((main_time < (rstCycle + 2*NUM_CYCLES))
            && !Verilated::gotFinish()) {

        if (main_time >= rstCycle && !rstFlag) {
            rstFlag = true;
            sim_reset_clock(top);
        }

        if (start_sim(top)) {
            RandomVecIn(vecIn);
            set_input(top, vecIn);
            cycle_cnt++;
        }

        top->eval();

        main_time++;
        ToggleClk(top);
    }
    
}

void SimMultiCycle(Vtop* top, vecIn_t& inputVec) {
    init_reset_clock(top);   
    bool rstFlag = false;
    int main_time = 0;                                                           
    int cycle_cnt = 0;                                                                  
    int rstCycle = 10;

	int vec_length = (inputVec.length()/CONST_NUM_INPUT_BITS);
	cout << "SimMultiCycle (" << vec_length << ")" << endl;    

    while ((main_time < (rstCycle + 2*vec_length))
            && !Verilated::gotFinish()) {

        if (main_time >= rstCycle && !rstFlag) {
			rstFlag = true;
			sim_reset_clock(top);
        }

        if (start_sim(top)) {
			set_input(top, inputVec.substr(cycle_cnt, CONST_NUM_INPUT_BITS));
			cycle_cnt += CONST_NUM_INPUT_BITS;
        }   
            
        top->eval();                                                                     
        main_time++;
        ToggleClk(top);
    }   
        
}

void SimOneCycle(Vtop* top, vecIn_t& vecIn) {
	
    uint main_time = 0;
	sim_reset_clock(top);
//	cout << "Simulating " << vecIn << endl;

    while ((main_time < 2) && !Verilated::gotFinish()) {
    	if (start_sim(top))
            set_input(top, vecIn);

        top->eval();

        main_time++;
        ToggleClk(top);
    }
   	
}

int GetCoverage(const Vtop* top, bool printCnt) {
    uint count = 0;
    for (int ind = 0; ind < CONST_NUM_BRANCH; ++ind) {
        if (top->__VlSymsp->__Vcoverage[ind]) {
			if (printCnt)
        		cout << ind << "(" << top->__VlSymsp->__Vcoverage[ind] << ") ";
            count++;
		}
    }
	if (printCnt)
    	cout << endl;

    return count;
}

int GetCoverage(const Vtop* top, int index) {
    assert((index >= 0) && (index < CONST_NUM_BRANCH));
    return top->__VlSymsp->__Vcoverage[index];
}

void GetCoverage(const Vtop* top, vector<int>& indVec) {
	indVec.clear();
    for (int ind = 0; ind < CONST_NUM_BRANCH; ++ind) {
        if (top->__VlSymsp->__Vcoverage[ind]) {
			indVec.push_back(ind);
		}
    }

}

int GetBranchCounters(const Vtop* top, vector<int>& branchHit) {
	uint numBranchHit = 0;
	if (branchHit.size() != (uint)CONST_NUM_BRANCH)
		branchHit = vector<int>(CONST_NUM_BRANCH);

    for (int ind = 0; ind < CONST_NUM_BRANCH; ++ind) {
		branchHit[ind] = top->__VlSymsp->__Vcoverage[ind];
        	if (branchHit[ind])	
				numBranchHit++;
    }

	return numBranchHit;
}

void GetResetBranch(Vtop* top, vector<int>& resetBranch) {
    SimMultiCycle(top, 0);
	GetBranchCounters(top, resetBranch);
}

void ResetCounters(Vtop* top) {
    for (int ind = 0; ind < CONST_NUM_BRANCH; ++ind)
        top->__VlSymsp->__Vcoverage[ind] = 0;
}

void setAllXState(Vtop* top) {

	top->v__DOT__id_exceptflags	=	rand() & 0x07;
	top->v__DOT__ex_exceptflags	=	rand() & 0x07; 
	top->v__DOT__state		=	rand() & 0x07;
	top->v__DOT__ex_dslot	=	rand() & 0x01;
	top->v__DOT__except_flushpipe	=	rand() & 0x01;
	top->v__DOT__delayed_iee	=	rand() & 0x01;
	top->v__DOT__delayed_tee	=	rand() & 0x07;
	top->v__DOT__int_pending	=	rand() & 0x07;
	top->v__DOT__tick_pending	=	rand() & 0x01;
	top->v__DOT__except_trig	=	rand() & 0x01;
	top->v__DOT__ex_pc		=	rand() & 0xffffffff;
	top->v__DOT__wb_pc		=	rand() & 0xffffffff;

}

void printCktState (Vtop* top) {
	cout
    	<< (uint) (top->v__DOT__id_exceptflags & 0x07) 	<< " "
    	<< (uint) (top->v__DOT__ex_exceptflags & 0x07)	<< " "
    	<< (uint) (top->v__DOT__state	& 0x07)	<< " "
    	<< (uint) (top->v__DOT__ex_dslot & 0x01)	<< " "
    	<< (uint) (top->v__DOT__except_flushpipe & 0x1)	<< " "
    	<< (uint) (top->v__DOT__delayed_iee & 0x07)	<< " "
    	<< (uint) (top->v__DOT__delayed_tee	& 0x07)	<< " "
    	<< (uint) (top->v__DOT__int_pending	& 0x01)	<< " "
    	<< (uint) (top->v__DOT__tick_pending & 0x01)	<< " "
    	<< (uint) (top->v__DOT__except_trig	& 0xfff)	<< " "
    	<< (uint) ((top->v__DOT__ex_pc >> 24) & 0xff) << "." 
    	<< (uint) ((top->v__DOT__ex_pc >> 16) & 0xff) << "." 
    	<< (uint) ((top->v__DOT__ex_pc >> 8) & 0xff) << "." 
    	<< (uint) (top->v__DOT__ex_pc & 0xff) << " " 
    	<< (uint) ((top->v__DOT__wb_pc >> 24) & 0xff)	<< " "
    	<< (uint) ((top->v__DOT__wb_pc >> 16) & 0xff)	<< " "
    	<< (uint) ((top->v__DOT__wb_pc >> 8) & 0xff)	<< " "
    	<< (uint) (top->v__DOT__wb_pc) 
		<< endl;	
}

//void modifyVecIn(vecIn_t& vecIn, const rstIn_t& resetInput) {
//	assert(resetInput.length() == (uint) CONST_NUM_INPUT_BITS);
//	assert(vecIn.length() == (uint) CONST_NUM_INPUT_BITS);
//
//	for (uint it = 0; it < vecIn.length(); ++it) {	
//		if(resetInput[it] == '1')
//			vecIn[it] = '0';
//		else if(resetInput[it] == '0')
//			vecIn[it] = '1';
//	}
//}
