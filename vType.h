#ifndef _VTYPE_H_
#define _VTYPE_H_

#include "incl.h"

//#if defined(__b01)
//	#include "b01Int.h"
//	#include "bench/b01/obj_dir/Vtop__Syms.h"
//	#include "bench/b01/obj_dir/Vtop.h"
//	#define benchCkt "b01"
//	#define benchDir "bench/b01/"
//	
//#elif defined(__b06)
//	#include "b06Int.h"
//	#include "bench/b06/obj_dir/Vtop__Syms.h"
//	#include "bench/b06/obj_dir/Vtop.h"
//	#define benchCkt "b06"
//	#define benchDir "bench/b06/"
//
//#elif defined(__b07)
//	#include "b07Int.h"
//	#include "bench/b07/obj_dir/Vtop__Syms.h"
//	#include "bench/b07/obj_dir/Vtop.h"
//	#define benchCkt "b07"
//	#define benchDir "bench/b07/"
//
//#elif defined(__b10)
//	#include "b10Int.h"
//	#include "bench/b10/obj_dir/Vtop__Syms.h"
//	#include "bench/b10/obj_dir/Vtop.h"
//	#define benchCkt "b10"
//	#define benchDir "bench/b10/"
//
//#elif defined(__b11)
//	#include "b11Int.h"
//	#include "bench/b11/obj_dir/Vtop__Syms.h"
//	#include "bench/b11/obj_dir/Vtop.h"
//	#define benchCkt "b11"
//	#define benchDir "bench/b11/"
//
//#elif defined(__b12)
//	#include "b12Int.h"
//	#include "bench/b12/obj_dir/Vtop__Syms.h"
//	#include "bench/b12/obj_dir/Vtop.h"
//	#define benchCkt "b12"
//	#define benchDir "bench/b12/"
//	#define rstInput 1
//
//#endif

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
