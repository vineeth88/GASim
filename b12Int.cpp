#include "b12Int.h"

using namespace std;

keyVal_t state_t :: getHash() {
	keyVal_t hash_value = 
		state_val.substr(8,2) +  // data_out
		state_val.substr(12,3) +  // sound
		state_val.substr(13,1) +  // play
		state_val.substr(17,3) +  // counter
		state_val.substr(86,5) +  // gamma
		state_val.substr(93,5) +  // scan
		state_val.substr(98,5) +  // max
		state_val.substr(109,6);   // count2
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
		<< (bitset<1>)	copy_obj->v__DOT__wr 		
		<< (bitset<5>)	copy_obj->v__DOT__address	
		<< (bitset<2>)	copy_obj->v__DOT__data_in	
		<< (bitset<2>)	copy_obj->v__DOT__data_out	
		<< (bitset<2>)	copy_obj->v__DOT__num		
		<< (bitset<3>)	copy_obj->v__DOT__sound		
		<< (bitset<1>)	copy_obj->v__DOT__play		
		<< (bitset<1>)	copy_obj->v__DOT__s			
		<< (bitset<3>)	copy_obj->v__DOT__counter	
		<< (bitset<2>)	copy_obj->v__DOT__count;
	// 22	 
	for (int __Vi0=0; __Vi0<32; ++__Vi0) {
		ss << (bitset<2>) copy_obj->v__DOT__memory[__Vi0];
    }
	// 86
	ss
		<< (bitset<5>)	copy_obj->v__DOT__gamma 	
		<< (bitset<2>)	copy_obj->v__DOT__ind 		
		<< (bitset<5>)	copy_obj->v__DOT__scan		
		<< (bitset<5>)	copy_obj->v__DOT__max		
		<< (bitset<6>)	copy_obj->v__DOT__timebase 
		<< (bitset<6>)	copy_obj->v__DOT__count2;
	
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
		case 0 :	sstr = state_val.substr(8,2);
					break;

		case 1 :	sstr = state_val.substr(12,3);
					break;

		case 2 :	sstr = state_val.substr(13,1);
					break;

		case 3 :	sstr = state_val.substr(17,3);
					break;

		case 4 :	sstr = state_val.substr(86,5);
					break;

		case 5 :	sstr = state_val.substr(93,5);
					break;

		case 6 :	sstr = state_val.substr(98,5);
					break;

		case 7 :	sstr = state_val.substr(109,6);
					break;

		default:	break;
	}

	return sstr;
}

void state_t::printState (bool full_) {
	if (full_)
    	cout << state_val << endl;
	else {
		if(state_val.length() < 115) {
			cout << state_val.length() << endl;
			assert(false);
		}
		cout << state_val.substr(8,2) 	<< " "  // data_out
			 << state_val.substr(12,3) 	<< " "  // sound
			 << state_val.substr(13,1) 	<< " "  // play
			 << state_val.substr(17,3) 	<< " "  // counter
			 << state_val.substr(86,5) 	<< " "  // gamma
			 << state_val.substr(93,5) 	<< " "  // scan
			 << state_val.substr(98,5) 	<< " "  // max
			 << state_val.substr(109,6)	<< endl;   // count2
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
			case 1 :	top->v__DOT__wr= val;
						val = 0;
						break;

			case 6 :	top->v__DOT__address = val;
						val = 0;
						break;

			case 8 :	top->v__DOT__data_in = val;
						val = 0;
						break;

			case 10:	top->v__DOT__data_out = val;
						val = 0;
						break;

			case 12:	top->v__DOT__num = val;
						val = 0;
						break;

			case 15:	top->v__DOT__sound = val;
						val = 0;
						break;

			case 16:	top->v__DOT__play = val;
						val = 0;
						break;

			case 17:	top->v__DOT__s = val;
						val = 0;
						break;

			case 20:	top->v__DOT__counter = val;
						val = 0;
						break;

			case 22:	top->v__DOT__count = val;
						val = 0;
						break;
			
			case 91:	top->v__DOT__gamma = val;
						val = 0;
						break;
			
			case 93:	top->v__DOT__ind = val;
						val = 0;
						break;
			
			case 98:	top->v__DOT__scan = val;
						val = 0;
						break;
			
			case 103:	top->v__DOT__max = val;
						val = 0;
						break;
			
			case 109:	top->v__DOT__timebase = val;
						val = 0;
						break;
			
			case 115:	top->v__DOT__count2 = val;
						val = 0;
						break;
			
			default:	break;
		}

		if (i == 21)
			i = 85;
	}

	for(int i = 0; i < 64; ++i) {

		val = val << 1;
		if (state_val[22 + i] == '1')
			val++;
		
		if (i%2) {
			top->v__DOT__memory[i/2] = val;
			val = 0;
		}
	}
}

// String -> Integer values
void set_input(Vtop *top, const vecIn_t& input)
{
	assert(input.length() == (uint)CONST_NUM_INPUT_BITS);
	top->start = input[0] - 48;
	top->k = 0;
	for (int i = 0; i < 4; ++i)
		top->k = ((top->k << 1) | (input[1+i] - 48));
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

	top->v__DOT__wr 		=	rand() & 0x01;
	top->v__DOT__address	=	rand() & 0x1f; 
	top->v__DOT__data_in	=	rand() & 0x03;
	top->v__DOT__data_out	=	rand() & 0x03;
	top->v__DOT__num		=	rand() & 0x03;
	top->v__DOT__sound		=	rand() & 0x07;
	top->v__DOT__play		=	rand() & 0x01;
	top->v__DOT__s			=	rand() & 0x01;
	top->v__DOT__counter	=	rand() & 0x07;
	top->v__DOT__count		=	rand() & 0x03;

	for (int __Vi0=0; __Vi0<32; ++__Vi0) {
		top->v__DOT__memory[__Vi0] = rand() & 0x03;
    }

	top->v__DOT__gamma 		=	rand() & 0x1f; 
	top->v__DOT__ind 		=	rand() & 0x03;
	top->v__DOT__scan		=	rand() & 0x1f;
	top->v__DOT__max		=	rand() & 0x1f;
	top->v__DOT__timebase 	=	rand() & 0x3f;
	top->v__DOT__count2		=	rand() & 0x3f;

}

void printCktState (Vtop* top) {
	cout
		<< (uint) top->v__DOT__wr 		<< " "					
		<< (uint) top->v__DOT__address	<< " "
		<< (uint) top->v__DOT__data_in	<< " "
		<< (uint) top->v__DOT__data_out	<< " "
		<< (uint) top->v__DOT__num		<< " "
		<< (uint) top->v__DOT__sound	<< " "	
		<< (uint) top->v__DOT__play		<< " "
		<< (uint) top->v__DOT__s		<< " "	
		<< (uint) top->v__DOT__counter	<< " "
		<< (uint) top->v__DOT__count	<< " "
	// [.. - 22] + [86 - ..]
		<< (uint) top->v__DOT__gamma 	<< " "		
		<< (uint) top->v__DOT__ind 		<< " "
		<< (uint) top->v__DOT__scan		<< " "
		<< (uint) top->v__DOT__max		<< " "
		<< (uint) top->v__DOT__timebase	<< " "
		<< (uint) top->v__DOT__count2	<< " "
		<< endl;
	cout << "[ ";
	for (int __Vi0=0; __Vi0<32; ++__Vi0) {
		cout << (uint) top->v__DOT__memory[__Vi0] << " ";
    }
	cout << "]" << endl;
	
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
