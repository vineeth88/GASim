#include "b06Int.h"

using namespace std;

keyVal_t state_t :: getHash() {
    return state_val;
}

state_t::state_t() {
	
	branch_index = std::vector<int>();
	state_index = -1;
	hit_count = 0;
	
	state_val = string(CONST_NUM_STATE_BITS, '0');
}

state_t::state_t(const state_t& copy_obj) {

	state_index = copy_obj.state_index;
	branch_index = copy_obj.branch_index;
	hit_count = copy_obj.hit_count;

	state_val = copy_obj.state_val;
}

state_t::state_t(const Vtop* copy_obj, int state_index_) :
	state_index(state_index_) 
{
	stringstream ss;
	ss << (bitset<3>) copy_obj->v__DOT__curr_state;
	ss >> state_val;

	GetCoverage(copy_obj, branch_index);
}

state_t::~state_t() {
	branch_index.clear();
	state_index = -1;
	hit_count = 0;

	state_val = "INVALID STATE";
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
	
	return state_val;
}

void state_t::printState (bool full_) {
    cout << state_val << endl;
}

void state_t::setCktState(Vtop* top) {
	int val = 0;
	for(uint i = 0; i < state_val.length(); ++i) {
		val = val << 1;
		if (state_val[i] == '1')
			val++;
	}
    top->v__DOT__curr_state = val;
}

// String -> Integer values
void set_input(Vtop *top, const vecIn_t& input)
{
	assert(input.length() == (uint)CONST_NUM_INPUT_BITS);
	top->eql = input[0] - 48;
	top->cont_eql = input[1] - 48;
}

void RandomVecIn(vecIn_t& vecIn)
{
	vecIn = string(CONST_NUM_INPUT_BITS, '0');
    for (uint i = 0; i < vecIn.size(); i++) 
		vecIn[i] = (rand() & 0x01) + 48;
    assert(vecIn.length() == (uint)CONST_NUM_INPUT_BITS);
}

void SimMultiCycle(Vtop* top, int NUM_CYCLES) {

    vecIn_t vecIn(CONST_NUM_INPUT_BITS, '0');
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
    top->v__DOT__curr_state = rand() & 0x07;
}

void printCktState (Vtop* top) {
    cout << (uint) top->v__DOT__curr_state << endl ;
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
