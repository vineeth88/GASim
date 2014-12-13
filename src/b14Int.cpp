#include "b14Int.h"

using namespace std;

keyVal_t state_t :: getHash() {
	keyVal_t hash_value = 
    	state_val.substr(128,1) + // process_1_B //(1);
    	state_val.substr(193,2) + // process_1_mf //(2);
    	state_val.substr(195,3) + // process_1_df //(3);
    	state_val.substr(198,1) + // process_1_cf //(1);
    	state_val.substr(199,4) + // process_1_ff //(4);
    	state_val.substr(235,32) + // process_1_IR //(32);
    	state_val.substr(267,1) + // process_1_state //(1);
    	state_val.substr(268,32) + // process_1_r //(32);
    	state_val.substr(300,32) + // process_1_m //(32);
    	state_val.substr(332,32) + // process_1_t //(32);
    	state_val.substr(364,32) + // process_1_d //(32);
    	state_val.substr(396,2); // process_1_s //(2);
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
	ss 	<< (bitset<32>)	copy_obj->v__DOT__process_1_reg0
    	<< (bitset<32>)	copy_obj->v__DOT__process_1_reg1
    	<< (bitset<32>)	copy_obj->v__DOT__process_1_reg2
    	<< (bitset<32>)	copy_obj->v__DOT__process_1_reg3 
    	<< (bitset<1>)	copy_obj->v__DOT__process_1_B 
    	<< (bitset<32>)	copy_obj->v__DOT__process_1_MAR 
    	<< (bitset<32>)	copy_obj->v__DOT__process_1_MBR 
    	<< (bitset<2>)	copy_obj->v__DOT__process_1_mf 
    	<< (bitset<3>)	copy_obj->v__DOT__process_1_df 
    	<< (bitset<1>)	copy_obj->v__DOT__process_1_cf 
    	<< (bitset<4>)	copy_obj->v__DOT__process_1_ff 
    	<< (bitset<32>)	copy_obj->v__DOT__process_1_tail 
    	<< (bitset<32>)	copy_obj->v__DOT__process_1_IR 
    	<< (bitset<1>)	copy_obj->v__DOT__process_1_state 
    	<< (bitset<32>)	copy_obj->v__DOT__process_1_r 
    	<< (bitset<32>)	copy_obj->v__DOT__process_1_m 
    	<< (bitset<32>)	copy_obj->v__DOT__process_1_t 
    	<< (bitset<32>)	copy_obj->v__DOT__process_1_d 
    	<< (bitset<2>)	copy_obj->v__DOT__process_1_s; 
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
		case 0	:	sstr = state_val.substr(0,32);	// process_1_reg0 //(32);
					break;

    	case 1	:	sstr = state_val.substr(32,32);	// process_1_reg1 //(32);
					break;

    	case 2	:	sstr = state_val.substr(64,32); // process_1_reg2 //(32);
					break;

    	case 3	:	sstr = state_val.substr(96,32);	// process_1_reg3 //(32);
					break;

    	case 4	:	sstr = state_val.substr(128,1);	// process_1_B //(1);
					break;

    	case 5	:	sstr = state_val.substr(129,32);	// process_1_MAR //(32);
					break;

    	case 6	:	sstr = state_val.substr(161,32); // process_1_MBR //(32);
					break;

    	case 7	:	sstr = state_val.substr(193,2);	// process_1_mf //(2);
					break;

    	case 8	:	sstr = state_val.substr(195,3);	// process_1_df //(3);
					break;

    	case 9	:	sstr = state_val.substr(198,1);	// process_1_cf //(1);
					break;

    	case 10 :	sstr = state_val.substr(199,4);	// process_1_ff //(4);
					break;

    	case 11	:	sstr = state_val.substr(203,32);	// process_1_tail //(32);
					break;

    	case 12	:	sstr = state_val.substr(235,32);	// process_1_IR //(32);
					break;

    	case 13	:	sstr = state_val.substr(267,1);	// process_1_state //(1);
					break;

    	case 14	:	sstr = state_val.substr(268,32);	// process_1_r //(32);
					break;

    	case 15	:	sstr = state_val.substr(300,32);	// process_1_m //(32);
					break;

    	case 16	:	sstr = state_val.substr(332,32);	// process_1_t //(32);
					break;

    	case 17	:	sstr = state_val.substr(364,32);	// process_1_d //(32);
					break;

    	case 18	:	sstr = state_val.substr(396,2);	// process_1_s //(2);
					break;

		default:	break;
	}

	return sstr;
}

void state_t::printState (bool full_) {
	if (full_) {
		cout << state_val.substr(0,32) << " "// process_1_reg0 //(32);
    		 << state_val.substr(32,32) << " " // process_1_reg1 //(32);
    		 << state_val.substr(64,32) << " " // process_1_reg2 //(32);
    		 << state_val.substr(96,32) << " " // process_1_reg3 //(32);
    		 << state_val.substr(128,1) << " " // process_1_B //(1);
    		 << state_val.substr(129,32) << " " // process_1_MAR //(32);
    		 << state_val.substr(161,32) << " " // process_1_MBR //(32);
    		 << state_val.substr(193,2) << " " // process_1_mf //(2);
    		 << state_val.substr(195,3) << " " // process_1_df //(3);
    		 << state_val.substr(198,1) << " " // process_1_cf //(1);
    		 << state_val.substr(199,4) << " " // process_1_ff //(4);
    		 << state_val.substr(203,32) << " " // process_1_tail //(32);
    		 << state_val.substr(235,32) << " " // process_1_IR //(32);
    		 << state_val.substr(267,1) << " " // process_1_state //(1);
    		 << state_val.substr(268,32) << " " // process_1_r //(32);
    		 << state_val.substr(300,32) << " " // process_1_m //(32);
    		 << state_val.substr(332,32) << " " // process_1_t //(32);
    		 << state_val.substr(364,32) << " " // process_1_d //(32);
    		 << state_val.substr(396,2)	// process_1_s //(2);
			 << endl;
	}
	else {
		cout << state_val.substr(128,1) << " " // process_1_B //(1);
    		 << state_val.substr(193,2) << " " // process_1_mf //(2);
    		 << state_val.substr(195,3) << " " // process_1_df //(3);
    		 << state_val.substr(198,1) << " " // process_1_cf //(1);
    		 << state_val.substr(199,4) << " " // process_1_ff //(4);
    		 << state_val.substr(235,32) << " " // process_1_IR //(32);
    		 << state_val.substr(267,1) << " " // process_1_state //(1);
    		 << state_val.substr(268,32) << " " // process_1_r //(32);
    		 << state_val.substr(300,32) << " " // process_1_m //(32);
    		 << state_val.substr(332,32) << " " // process_1_t //(32);
    		 << state_val.substr(364,32) << " " // process_1_d //(32);
    		 << state_val.substr(396,2)	 // process_1_s //(2);
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
			case 32 :	top->v__DOT__process_1_reg0 = val;
						val = 0;
						break;

			case 64 :	top->v__DOT__process_1_reg1 = val;
						val = 0;
						break;

			case 96 :	top->v__DOT__process_1_reg2 = val;
						val = 0;
						break;

			case 128:	top->v__DOT__process_1_reg3 = val;
						val = 0;
						break;

			case 129:	top->v__DOT__process_1_B = val;
						val = 0;
						break;

			case 161:	top->v__DOT__process_1_MAR = val;
						val = 0;
						break;

			case 193:	top->v__DOT__process_1_MBR = val;
						val = 0;
						break;

			case 195:	top->v__DOT__process_1_mf = val;
						val = 0;
						break;

			case 198:	top->v__DOT__process_1_df = val;
						val = 0;
						break;

			case 199:	top->v__DOT__process_1_cf = val;
						val = 0;
						break;
			
			case 203:	top->v__DOT__process_1_ff = val;
						val = 0;
						break;
			
			case 235:	top->v__DOT__process_1_tail = val;
						val = 0;
						break;
			
			case 267:	top->v__DOT__process_1_IR = val;
						val = 0;
						break;
			
			case 268:	top->v__DOT__process_1_state = val;
						val = 0;
						break;
			
			case 300:	top->v__DOT__process_1_r = val;
						val = 0;
						break;
			
			case 332:	top->v__DOT__process_1_m = val;
						val = 0;
						break;
			
			case 364:	top->v__DOT__process_1_t = val;
						val = 0;
						break;

			case 396:	top->v__DOT__process_1_d = val;
						val = 0;
						break;
			
			case 398:	top->v__DOT__process_1_s = val;
						val = 0;
						break;

			default:	break;
		}
	}
}

// String -> Integer values
void set_input(Vtop *top, const vecIn_t& input)
{
	assert(input.length() == (uint)CONST_NUM_INPUT_BITS);
	top->datai = 0;
	for (int i = 0; i < CONST_NUM_INPUT_BITS; ++i)
		top->datai = ((top->datai << 1) | (input[i] - 48));
}

void RandomVecIn(vecIn_t& vecIn)
{
	vecIn = string(CONST_NUM_INPUT_BITS, '0');
    for (uint i = 0; i < vecIn.length(); i++) 
		vecIn[i] = (rand() & 0x01) + 48;
	//cout << "RandomVecIn: " << vecIn.length() << endl;
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
	//cout << "Simulating " << vecIn << endl;
	
	assert (vecIn.length() == (uint)CONST_NUM_INPUT_BITS);
	#ifdef _ResetMask_
	ModifyVecIn(vecIn, gVarClass::resetInput);
	#endif
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

	top->v__DOT__process_1_reg0	=	rand() & 0xffffffff; 
	top->v__DOT__process_1_reg1	=	rand() & 0xffffffff;
	top->v__DOT__process_1_reg2	=	rand() & 0xffffffff;
	top->v__DOT__process_1_reg3	=	rand() & 0xffffffff;
	top->v__DOT__process_1_B	=	rand() & 0x01;
	top->v__DOT__process_1_MAR	=	rand() & 0xffffffff;
	top->v__DOT__process_1_MBR	=	rand() & 0xffffffff;
	top->v__DOT__process_1_mf	=	rand() & 0x03;
	top->v__DOT__process_1_df	=	rand() & 0x07;
	top->v__DOT__process_1_cf	=	rand() & 0x01;
	top->v__DOT__process_1_ff	=	rand() & 0x0f;
	top->v__DOT__process_1_tail	=	rand() & 0xffffffff;
	top->v__DOT__process_1_IR	=	rand() & 0xffffffff;
	top->v__DOT__process_1_state=	rand() & 0x01;
	top->v__DOT__process_1_r	=	rand() & 0xffffffff;
	top->v__DOT__process_1_m	=	rand() & 0xffffffff;
	top->v__DOT__process_1_t	=	rand() & 0xffffffff;
	top->v__DOT__process_1_d	=	rand() & 0xffffffff;
	top->v__DOT__process_1_s	=	rand() & 0x03;
}

void printCktState (Vtop* top) {
	cout 		
 		 		<< (bitset<8>) (top->v__DOT__process_1_reg0 & 0xff000000)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_reg0 & 0x00ff0000)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_reg0 & 0x0000ff00)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_reg0 & 0x000000ff)
    	 << " " << (bitset<8>) (top->v__DOT__process_1_reg1 & 0xff000000)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_reg1 & 0x00ff0000)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_reg1 & 0x0000ff00)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_reg1 & 0x000000ff)
    	 << " " << (bitset<8>) (top->v__DOT__process_1_reg2 & 0xff000000)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_reg2 & 0x00ff0000)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_reg2 & 0x0000ff00)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_reg2 & 0x000000ff)
    	 << " " << (bitset<8>) (top->v__DOT__process_1_reg3 & 0xff000000)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_reg3 & 0x00ff0000)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_reg3 & 0x0000ff00)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_reg3 & 0x000000ff)
    	 << " " << (bitset<1>) top->v__DOT__process_1_B
    	 << " " << (bitset<8>) (top->v__DOT__process_1_MAR & 0xff000000)
    	 << " " << (bitset<8>) (top->v__DOT__process_1_MAR & 0x00ff0000)
    	 << " " << (bitset<8>) (top->v__DOT__process_1_MAR & 0x0000ff00)
    	 << " " << (bitset<8>) (top->v__DOT__process_1_MAR & 0x000000ff)
    	 << " " << (bitset<8>) (top->v__DOT__process_1_MBR & 0xff000000)
    	 << " " << (bitset<8>) (top->v__DOT__process_1_MBR & 0x00ff0000)
    	 << " " << (bitset<8>) (top->v__DOT__process_1_MBR & 0x0000ff00)
    	 << " " << (bitset<8>) (top->v__DOT__process_1_MBR & 0x000000ff)
    	 << " " << (bitset<2>) top->v__DOT__process_1_mf
    	 << " " << (bitset<3>) top->v__DOT__process_1_df
    	 << " " << (bitset<1>) top->v__DOT__process_1_cf
    	 << " " << (bitset<4>) top->v__DOT__process_1_ff
    	 << " " << (bitset<8>) (top->v__DOT__process_1_tail & 0xff000000)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_tail & 0x00ff0000)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_tail & 0x0000ff00)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_tail & 0x000000ff)
    	 << " " << (bitset<8>) (top->v__DOT__process_1_IR & 0xff000000)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_IR & 0x00ff0000)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_IR & 0x0000ff00)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_IR & 0x000000ff)
    	 << " " << (bitset<1>) top->v__DOT__process_1_state
    	 << " " << (bitset<8>) (top->v__DOT__process_1_r & 0xff000000)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_r & 0x00ff0000)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_r & 0x0000ff00)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_r & 0x000000ff)
    	 << " " << (bitset<8>) (top->v__DOT__process_1_m & 0xff000000)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_m & 0x00ff0000)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_m & 0x0000ff00)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_m & 0x000000ff)
    	 << " " << (bitset<8>) (top->v__DOT__process_1_t & 0xff000000)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_t & 0x00ff0000)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_t & 0x0000ff00)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_t & 0x000000ff)
    	 << " " << (bitset<8>) (top->v__DOT__process_1_d & 0xff000000)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_d & 0x00ff0000)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_d & 0x0000ff00)
    	 << "." << (bitset<8>) (top->v__DOT__process_1_d & 0x000000ff)
    	 << " " << (bitset<2>) top->v__DOT__process_1_s
		 << endl;
	
}

void ModifyVecIn(vecIn_t& vecIn, const rstIn_t& resetInput) {
	assert(resetInput.length() == (uint) CONST_NUM_INPUT_BITS);
	assert(vecIn.length() == (uint) CONST_NUM_INPUT_BITS);

	for (uint it = 0; it < vecIn.length(); ++it) {	
		if(resetInput[it] == '1')
			vecIn[it] = '0';
		else if(resetInput[it] == '0')
			vecIn[it] = '1';
	}
}
