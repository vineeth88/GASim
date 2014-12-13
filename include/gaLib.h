#ifndef GA_LIB_H
#define GA_LIB_H

#include "incl.h"
#include "verInclude.h"
#include "vType.h" 

const int NUM_INDIV_FWD = 2;

class gaIndiv_t {
	
	public:

	int index;
	
	int vec_length;
	vecIn_t input_vec;

	state_t* start_state;
	state_pVec state_list;

	vector<int> branch_cov;
	vector<int> branch_cycle;
	int max_index;
	int num_branch;

	fitness_t fitness;

	//Constructors
	gaIndiv_t(int index_, int length_);
	gaIndiv_t(int index_, int length_, state_t*);
	/*	Copy constructor */
	gaIndiv_t(const gaIndiv_t&);	

	//Destructors
	~gaIndiv_t();

	bool operator<(gaIndiv_t* indiv) const;
	bool operator==(gaIndiv_t* indiv) const;

	//Simulate ckt
	void simCkt(Vtop*);
	void simCkt(Vtop*, int);
	
	void initRandom(int);
	void seedIndiv(gaIndiv_t*, int, int);
	void printIndiv(bool state_ = false);

	static int mem_alloc_cnt;
};

class gaPopulation_t {
	
	public:
	
	int pop_size;
	int indiv_vec_length;
	gaIndiv_pVec indiv_vec;
	vector<fitness_t> fitness_set;

	gaPopulation_t(int pop_size_, int indiv_vec_length_);

	~gaPopulation_t();

	void initPopulation(const state_pVec&);

	// GA Operators
	int gaSelect(gaIndiv_t*&, gaIndiv_t*&);
	int gaCrossover(gaIndiv_t*, gaIndiv_t*, gaIndiv_t*&, gaIndiv_t*&);
	void gaMutate();
	void gaEvolve();

	static int mem_alloc_cnt;
};

int SelectTournament(const vector<fitness_t>&);
bool compFitness(gaIndiv_t*, gaIndiv_t*);

#endif // GA_LIB_H
