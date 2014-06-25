#ifndef _VTYPE_H_
#define _VTYPE_H_

#include "incl.h"

using namespace std;

// Data types
typedef unsigned int uint;
typedef unsigned long int ulint;
typedef unsigned long long int ullint;

// Vectors
typedef std::string vecIn_t;
typedef std::string rstIn_t;

// State
typedef string keyVal_t;

class state_t;
class gaIndiv_t;
class gaPopulation_t;

typedef map<keyVal_t, state_t*> stateMap_t;
typedef stateMap_t::iterator stateMap_iter;
typedef pair<stateMap_iter, bool> retVal_t;

typedef vector<state_t*> state_pVec;
typedef state_pVec::iterator state_pVec_iter;

typedef vector<gaIndiv_t*> gaIndiv_pVec;
typedef vector<gaIndiv_t*>::iterator gaIndiv_pVec_iter;
typedef int fitness_t;

typedef vector<int> int_vec;
typedef vector<int>::iterator int_vec_iter;

class state_t {

	public:

	int state_index;
	gaIndiv_t* pIndiv;

	int hit_count;
	string state_val;
	
	vector<int> branch_index;
	
	fitness_t state_fitness;

	// Constructors
	state_t();
    state_t(const state_t& copy_obj);
	state_t(const Vtop* copy_obj, int index_ = -1);
	// Destructor
	~state_t();

    state_t& operator = (const state_t& copy_obj);
	bool operator==(const state_t& lhs);
	string operator[](int index_);

	// Member functions
	void setCktState(Vtop*);				// Set state as curr state
	void printState(bool full_=false);	// Print (full/control) state
	keyVal_t getHash();

	static int mem_alloc_cnt;
};

class gVarClass {

public:
	static string			fName, vName;

	static vector<vecIn_t> 	inputComb;
	static state_t			ctrlState;
	static state_pVec		ctrlStateVec;

	static rstIn_t			resetInput;
	static state_t			resetState;
	static state_pVec		resetStateVec;

	static set<int>			brUncover;

	static stateMap_t		stateMap;
	static state_pVec		stateList;

	static vector<vecIn_t>	inputVec;
};
#endif 	// _VTYPE_H_
