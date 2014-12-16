#include "include/or1200_1Int.h"

using namespace std;

keyVal_t state_t :: getHash() {
	// TODO
	keyVal_t hash_value = 
		state_val.substr(67,3) +  // state
		state_val.substr(70,3) +  // cnt
		state_val.substr(73,1) +  // hitmiss_eval
		state_val.substr(76,1);  // inhibit
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
    	<< (bitset<32>)	copy_obj->v__DOT__dc_addr 
    	<< (bitset<1>)	copy_obj->v__DOT__dcfsm_biu_read
    	<< (bitset<1>)	copy_obj->v__DOT__tagcomp_miss
    	<< (bitset<1>)	copy_obj->v__DOT__dcfsm_first_miss_ack
    	<< (bitset<32>)	copy_obj->v__DOT__or1200_dc_fsm__DOT__saved_addr_r
    	<< (bitset<3>)	copy_obj->v__DOT__or1200_dc_fsm__DOT__state 
    	<< (bitset<3>)	copy_obj->v__DOT__or1200_dc_fsm__DOT__cnt
    	<< (bitset<1>)	copy_obj->v__DOT__or1200_dc_fsm__DOT__hitmiss_eval
    	<< (bitset<1>)	copy_obj->v__DOT__or1200_dc_fsm__DOT__store 
    	<< (bitset<1>)	copy_obj->v__DOT__or1200_dc_fsm__DOT__load 
    	<< (bitset<1>)	copy_obj->v__DOT__or1200_dc_fsm__DOT__cache_inhibit;

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

string state_t::operator[] (int index_) {
	assert(index_ < CONST_NUM_VARS);
	string sstr;

	switch(index_) {
		case 0 :	sstr = state_val.substr(0,32);  // addr
					break;

		case 1 :	sstr = state_val.substr(32,1);	// read
					break;

		case 2 :	sstr = state_val.substr(33,1);	// miss
					break;

		case 3 :	sstr = state_val.substr(34,1);  // miss_ack
					break;

		case 4 :	sstr = state_val.substr(35,32);  // saved_addr
					break;

		case 5 :	sstr = state_val.substr(67,3);  // state
					break;

		case 6 :	sstr = state_val.substr(70,3);  // cnt
					break;

		case 7 :	sstr = state_val.substr(73,1);  // hitmiss_eval
					break;

		case 8 :	sstr = state_val.substr(74,1);  // store
					break;

		case 9 :	sstr = state_val.substr(75,1);  // load
					break;

		case 10 :	sstr = state_val.substr(76,1);  // inhibit
					break;

		default:	break;
	}

	return sstr;
}

void state_t::printState (bool full_) {
	if (full_)
    	cout << state_val << endl;
	else {
		cout
		<< " " << state_val.substr(67,3) 	// state
		<< " " << state_val.substr(70,3) 	// cnt
		<< " " << state_val.substr(73,1) 	// hitmiss_eval
		<< " " << state_val.substr(76,1)	// inhibit
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
			case 32 :	top->v__DOT__dc_addr = val;
						val = 0;
						break;

			case 33 :	top->v__DOT__dcfsm_biu_read = val;
						val = 0;
						break;

			case 34 :	top->v__DOT__tagcomp_miss = val;
						val = 0;
						break;

			case 35 :	top->v__DOT__dcfsm_first_miss_ack = val;
						val = 0;
						break;

			case 67 :	top->v__DOT__or1200_dc_fsm__DOT__saved_addr_r = val;
						val = 0;
						break;

			case 70 :	top->v__DOT__or1200_dc_fsm__DOT__state = val;
						val = 0;
						break;

			case 73 :	top->v__DOT__or1200_dc_fsm__DOT__cnt = val;
						val = 0;
						break;

			case 74 :	top->v__DOT__or1200_dc_fsm__DOT__hitmiss_eval = val;
						val = 0;
						break;

			case 75 :	top->v__DOT__or1200_dc_fsm__DOT__store = val;
						val = 0;
						break;

			case 76 :	top->v__DOT__or1200_dc_fsm__DOT__load = val;
						val = 0;
						break;

			case 77 :	top->v__DOT__or1200_dc_fsm__DOT__cache_inhibit = val;
						val = 0;
						break;
			
			default:	break;
		}

	}

}

void set_input(Vtop *top, const vecIn_t& input)	{
// TODO
	assert(input.length() == (uint)CONST_NUM_INPUT_BITS);
    top->dcqmem_sel_i = 0;
    top->dcqmem_tag_i  = 0;
    top->dcsb_dat_i = 0;
    top->dcqmem_adr_i = 0;
    top->dcqmem_dat_i = 0;
	top->spr_dat_i = 0;

	int cnt = 0;
    top->dcsb_ack_i = input[cnt++] - '0';
    top->dcsb_err_i = input[cnt++] - '0';
    top->dc_en 		 = input[cnt++] - '0';
    top->dcqmem_cycstb_i = input[cnt++] - '0';
    top->dcqmem_ci_i = input[cnt++] - '0';
    top->dcqmem_we_i =  input[cnt++] - '0';

    for (int i = 0; i < 4; i++) {
        top->dcqmem_sel_i = ((top->dcqmem_sel_i << 1) | (input[cnt++] - 48));
    }
    for (int i = 0; i < 4; i++) {
        top->dcqmem_tag_i = ((top->dcqmem_tag_i << 1) | (input[cnt++] - 48));
    }
    top->spr_cs = input[cnt++] - '0';
    top->spr_write = input[cnt++] - '0';
    for (int i = 0; i < 32; i++) {
        top->dcsb_dat_i = ((top->dcsb_dat_i << 1) | (input[cnt++] - 48));
    }
    for (int i = 0; i < 32; i++) {
        top->dcqmem_adr_i = ((top->dcqmem_adr_i << 1) | (input[cnt++] - 48));
    }
    for (int i = 0; i < 32; i++) {
        top->dcqmem_dat_i = ((top->dcqmem_dat_i << 1) | (input[cnt++] - 48));
    }
    for (int i = 0; i < 32; i++) {
        top->spr_dat_i = ((top->spr_dat_i << 1) | (input[cnt++] - 48));
    }

//	cout 
//	<< (bitset<1>) top->dcbiu_ack_i
//	<< (bitset<1>) top->dcbiu_err_i
//	<< (bitset<1>) top->dc_en 		
//	<< (bitset<1>) top->dcqmem_cycstb_i 
//	<< (bitset<1>) top->dcqmem_ci_i
//	<< (bitset<4>) top->dcqmem_sel_i
//	<< (bitset<4>) top->dcqmem_tag_i
//	<< (bitset<1>) top->spr_cs
//	<< (bitset<1>) top->spr_write 
//	<< (bitset<32>) top->dcbiu_dat_i
//	<< (bitset<32>) top->dcqmem_adr_i
//	<< (bitset<32>) top->spr_dat_i
//	<< endl;

	assert (cnt == CONST_NUM_INPUT_BITS);
}

void RandomVecIn(vecIn_t& vecIn)
{
	vecIn = string(CONST_NUM_INPUT_BITS, '0');
//	// Only inputs which are control-flow variables
//    for (uint i = 0; i < 6; i++) 
//		vecIn[i] = (rand() & 0x01) + 48;
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

	top->v__DOT__dc_addr 				=	rand() & 0xffffffff;
	top->v__DOT__dcfsm_biu_read			=	rand() & 0x01; 
	top->v__DOT__tagcomp_miss			=	rand() & 0x01;
	top->v__DOT__dcfsm_first_miss_ack	=	rand() & 0x01;
	top->v__DOT__or1200_dc_fsm__DOT__saved_addr_r	=	rand() & 0xffffffff;
	top->v__DOT__or1200_dc_fsm__DOT__state			=	rand() & 0x07;
	top->v__DOT__or1200_dc_fsm__DOT__cnt			=	rand() & 0x07;
	top->v__DOT__or1200_dc_fsm__DOT__hitmiss_eval	=	rand() & 0x01;
	top->v__DOT__or1200_dc_fsm__DOT__store			=	rand() & 0x01;
	top->v__DOT__or1200_dc_fsm__DOT__load			=	rand() & 0x01;
	top->v__DOT__or1200_dc_fsm__DOT__cache_inhibit	=	rand() & 0x01;

}

void printCktState (Vtop* top) {
	cout
    	<< (uint) ((top->v__DOT__dc_addr >> 24) & 0xff) << "."
    	<< (uint) ((top->v__DOT__dc_addr >> 16) & 0xff) << "."
    	<< (uint) ((top->v__DOT__dc_addr >> 8) & 0xff) 	<< "."
    	<< (uint) (top->v__DOT__dc_addr & 0xff) << " "
    	<< (uint) top->v__DOT__dcfsm_biu_read	<< " "
    	<< (uint) top->v__DOT__tagcomp_miss	<< " "
    	<< (uint) top->v__DOT__dcfsm_first_miss_ack	<< " "
    	<< (uint) ((top->v__DOT__or1200_dc_fsm__DOT__saved_addr_r >> 24) & 0xff ) << "." 
    	<< (uint) ((top->v__DOT__or1200_dc_fsm__DOT__saved_addr_r >> 16) & 0xff ) << "."
    	<< (uint) ((top->v__DOT__or1200_dc_fsm__DOT__saved_addr_r >> 8)  & 0xff ) << "."
    	<< (uint) ((top->v__DOT__or1200_dc_fsm__DOT__saved_addr_r)	& 0xff) << " "
    	<< (uint) top->v__DOT__or1200_dc_fsm__DOT__state 			<< " "
    	<< (uint) top->v__DOT__or1200_dc_fsm__DOT__cnt				<< " "
    	<< (uint) top->v__DOT__or1200_dc_fsm__DOT__hitmiss_eval	<< " "
    	<< (uint) top->v__DOT__or1200_dc_fsm__DOT__store 			<< " "
    	<< (uint) top->v__DOT__or1200_dc_fsm__DOT__load 			<< " "
    	<< (uint) top->v__DOT__or1200_dc_fsm__DOT__cache_inhibit
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
