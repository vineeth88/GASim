#include "b10Int.h"

using namespace std;

keyVal_t state_t :: getHash() {
    return state_val;
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
   	ss 	<< (bitset<4>) copy_obj->v__DOT__stato
    	<< (bitset<1>) copy_obj->v__DOT__voto0
   		<< (bitset<1>) copy_obj->v__DOT__voto1
   		<< (bitset<1>) copy_obj->v__DOT__voto2
   		<< (bitset<1>) copy_obj->v__DOT__voto3
   		<< (bitset<4>) copy_obj->v__DOT__sign
   		<< (bitset<1>) copy_obj->v__DOT__last_g
   		<< (bitset<1>) copy_obj->v__DOT__last_r;

	ss >> state_val;

	GetCoverage(copy_obj, branch_index);

	mem_alloc_cnt++;
}

state_t::~state_t() {
	state_index = -1;
	hit_count = 0;

	state_val = "INVALID STATE";
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
		case 0 :	sstr = state_val.substr(0,4);
					break;

		case 1 :	sstr = state_val.substr(4,1);
					break;

		case 2 :	sstr = state_val.substr(5,1);
					break;

		case 3 :	sstr = state_val.substr(6,1);
					break;

		case 4 :	sstr = state_val.substr(7,1);
					break;

		case 5 :	sstr = state_val.substr(8,4);
					break;

		case 6 :	sstr = state_val.substr(12,1);
					break;

		case 7 :	sstr = state_val.substr(13,1);
					break;

		default:	break;
	}

	return sstr;
}

void state_t::printState (bool full_) {
    cout
		<< state_val.substr(0,4)	<< " "
		<< state_val.substr(4,1)	<< " "
		<< state_val.substr(5,1)	<< " "
		<< state_val.substr(6,1)	<< " "
		<< state_val.substr(7,1)	<< " "
		<< state_val.substr(8,4)	<< " "
		<< state_val.substr(12,1)	<< " "
		<< state_val.substr(13,1)	<< " "
		<< endl;

	for (vector<int>::iterator it = branch_index.begin();
			it != branch_index.end(); ++it)
		cout << *it << " ";
	cout << endl;
}

void state_t::setCktState(Vtop* top) {
	int val = 0;
	for(uint i = 0; i < state_val.length(); ++i) {

		val = val << 1;
		if (state_val[i] == '1')
			val++;

		switch(i+1) {
			case 4 :	top->v__DOT__stato= val;
						val = 0;
						break;

			case 5 :	top->v__DOT__voto0 = val;
						val = 0;
						break;

			case 6 :	top->v__DOT__voto1 = val;
						val = 0;
						break;

			case 7:		top->v__DOT__voto2 = val;
						val = 0;
						break;

			case 8:		top->v__DOT__voto3 = val;
						val = 0;
						break;

			case 12:	top->v__DOT__sign = val;
						val = 0;
						break;

			case 13:	top->v__DOT__last_g = val;
						val = 0;
						break;

			case 14:	top->v__DOT__last_r = val;
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
    top->r_button = input[0] - 48;
    top->g_button = input[1] - 48;
    top->key 	= input[2] - 48;
    top->start 	= input[3] - 48;
    top->test 	= input[4] - 48;
    top->rts 	= input[5] - 48;
    top->rtr 	= input[6] - 48;
	top->v_in 	= 0; 
	for(uint i = 0; i < 4; ++i) {
		top->v_in = top->v_in << 1;
		if (input[7+i] == '1')
			top->v_in++;
	}

//	cout << (uint) top->r_button
//		 << (uint) top->g_button
//		 << (uint) top->key 	
//		 << (uint) top->start 	
//		 << (uint) top->test 	
//		 << (uint) top->rts 	
//		 << (uint) top->rtr 	
//		 << (uint) top->v_in << endl;

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
	GetCoverage(top, resetBranch);
}

void ResetCounters(Vtop* top) {
    for (int ind = 0; ind < CONST_NUM_BRANCH; ++ind)
        top->__VlSymsp->__Vcoverage[ind] = 0;
}

void setAllXState(Vtop* top) {

   	top->v__DOT__stato	= rand() & 0x0f; 	
    top->v__DOT__voto0	= rand() & 0x01;
   	top->v__DOT__voto1	= rand() & 0x01;
   	top->v__DOT__voto2	= rand() & 0x01;
   	top->v__DOT__voto3	= rand() & 0x01;
   	top->v__DOT__sign	= rand() & 0x0f;
   	top->v__DOT__last_g	= rand() & 0x01;
   	top->v__DOT__last_r = rand() & 0x01;

}

void printCktState (Vtop* top) {
   	cout << (uint) top->v__DOT__stato	<< " "
    	 << (uint) top->v__DOT__voto0	<< " "
   		 << (uint) top->v__DOT__voto1	<< " "
   		 << (uint) top->v__DOT__voto2	<< " "
   		 << (uint) top->v__DOT__voto3	<< " "
   		 << (uint) top->v__DOT__sign	<< " "
   		 << (uint) top->v__DOT__last_g	<< " "
   		 << (uint) top->v__DOT__last_r 	<< endl;
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
