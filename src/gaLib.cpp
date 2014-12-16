#include "gaLib.h"

using namespace std;
// GA Individual
gaIndiv_t::gaIndiv_t (int index_, int vec_length_) :
	index(index_),
	vec_length(vec_length_) 
{
	//input_vec = string(vec_length * CONST_NUM_INPUT_BITS, 'X');
	input_vec.clear();
	state_list = vector<state_t*> (vec_length, NULL);
	fitness = 0;
	branch_cov = vector<int>(CONST_NUM_BRANCH, 0);
	branch_cycle = vector<int>(vec_length, 0);
	max_index = -1, num_branch = 0;

	mem_alloc_cnt++;
}

gaIndiv_t::gaIndiv_t (int index_, int vec_length_, state_t* start_) :
	index(index_),
	vec_length(vec_length_) 
{
	//input_vec = string(vec_length * CONST_NUM_INPUT_BITS, 'X');
	input_vec.clear();
	start_state = start_;
	state_list = vector<state_t*> (vec_length, NULL);
	fitness = 0;
	branch_cov = vector<int>(CONST_NUM_BRANCH, 0);
	branch_cycle = vector<int>(vec_length, 0);
	max_index = -1, num_branch = 0;

	mem_alloc_cnt++;
}

gaIndiv_t::gaIndiv_t(const gaIndiv_t& copy_obj) :
	index(copy_obj.index), 
	vec_length(copy_obj.vec_length) {
	
	input_vec = copy_obj.input_vec;

	start_state = copy_obj.start_state;
	state_list = state_pVec(copy_obj.state_list);  

	branch_cov = copy_obj.branch_cov; 
	branch_cycle = copy_obj.branch_cycle;
	
	fitness = copy_obj.fitness;
	max_index = copy_obj.max_index;
	num_branch = copy_obj.num_branch;

	mem_alloc_cnt++;
}

gaIndiv_t::~gaIndiv_t() {
	input_vec.clear();
	assert((uint)vec_length == state_list.size());
	for (int i = 0; i < vec_length; ++i) {
		if (state_list[i]) {
			delete state_list[i];
			state_list[i] = NULL;
		}
	}
	state_list.clear();
	branch_cov.clear();

	mem_alloc_cnt--;
	#ifdef _DBG_DEST_CALL_
	cout << endl << "Deleting gaIndiv_t " << mem_alloc_cnt << endl;
	#endif
}

bool gaIndiv_t :: operator<(gaIndiv_t* indiv_) const {
	// TODO

	if (num_branch > indiv_->num_branch)
		return false;
	else {
		keyVal_t hash_ = (indiv_->start_state)->getHash();
		int eqFl = hash_.compare(start_state->getHash());
		if (eqFl < 0)
			return false;
		else if (eqFl > 0)
			return true;
	}
	
	return false;
}

bool gaIndiv_t :: operator==(gaIndiv_t* indiv_) const {
	// TODO
	if (num_branch != indiv_->num_branch)
		return false;
	else {
		keyVal_t hash_ = (indiv_->start_state)->getHash();
		/* Compare start states	*/
		if (hash_.compare(start_state->getHash()))
			return false;

		/* Compare if same branches reached	
		EDIT: 
		If same states reached, then same branches reached*/

//		assert(branch_cov.size() == (uint)CONST_NUM_BRANCH);
//		assert(indiv_->branch_cov.size() == (uint)CONST_NUM_BRANCH);
//		bool ret = false;
//		for (int i = 0; i < CONST_NUM_BRANCH; ++i) {
//			if ((branch_cov[i] == 0) && (indiv_->branch_cov[i]))
//				ret = true;
//			else if ((branch_cov[i]) && (indiv_->branch_cov[i] == 0))
//				ret = true;
//		}
//
//		if (ret == false)
//			return false;
//		else {
		
		/* Compare if state lists are identical	*/
		bool state_comp = true;
		assert(state_list.size() == (uint)vec_length);
		assert(indiv_->state_list.size() == (uint)vec_length);
		
		keyVal_t hash1, hash2;
		for (int st = 0; st < vec_length && state_comp; ++st) {
			hash1 = state_list[st]->getHash();
			hash2 = (indiv_->state_list[st])->getHash();

			state_comp = (hash1.compare(hash2) == 0);
		}
		
		return state_comp;
	}
	return true;
}

void gaIndiv_t :: simCkt(Vtop* ckt) {
	
	assert(input_vec.length() == (uint)(vec_length * CONST_NUM_INPUT_BITS));
	int cycle = 0;
	sim_reset_clock(ckt);
	start_state->setCktState(ckt);

//	cout << index << ": " << vec_length << endl;
//	printCktState(ckt);

	branch_cycle = vector<int>(vec_length,0);
	branch_cov = vector<int>(CONST_NUM_BRANCH,0);

	for (cycle = 0; cycle < vec_length; ++cycle ) {
		simCkt(ckt, cycle);
		if (cycle) 
			branch_cycle[cycle] += branch_cycle[cycle-1];
	}

	num_branch = 0;
	for (int br = 0; br < CONST_NUM_BRANCH; ++br)
		if (branch_cov[br])
			num_branch++;
	
	max_index = 0;
	for (cycle = 1; cycle < vec_length; ++cycle) {
		if ((branch_cycle[cycle] == num_branch) &&
			(branch_cycle[cycle] > branch_cycle[cycle-1])) 
			max_index = cycle;
	}	
	return;
}

void gaIndiv_t :: simCkt(Vtop* ckt, int cycle) {
	
	assert(cycle < vec_length);

	vecIn_t inp = input_vec.substr(cycle*CONST_NUM_INPUT_BITS, CONST_NUM_INPUT_BITS);
	SimOneCycle(ckt, inp);

/* 	Not required as modifyVecIn has been disabled	*/
//	input_vec.replace(cycle*CONST_NUM_INPUT_BITS, CONST_NUM_INPUT_BITS, inp);	
	
	state_t *tmp_state = new state_t(ckt, cycle);
	tmp_state->pIndiv = this;

	if (state_list[cycle])
		delete state_list[cycle];
	state_list[cycle] = tmp_state;

	for(vector<int>::iterator it = tmp_state->branch_index.begin();
			it != tmp_state->branch_index.end(); ++it) {
		if (branch_cov[*it] == 0)
			branch_cycle[cycle]++;
		branch_cov[*it]++;
	}

	ResetCounters(ckt);
}
	
void gaIndiv_t :: initRandom(int start_) {
	
	assert (input_vec.length() == (uint)(start_ * CONST_NUM_INPUT_BITS));

    for (int i = start_; i < vec_length; i++) {
        vecIn_t tmpVec(CONST_NUM_INPUT_BITS, 'X');
        RandomVecIn(tmpVec);
        input_vec += tmpVec;
    }
	assert(input_vec.length() == (uint)(vec_length * CONST_NUM_INPUT_BITS));
}

void gaIndiv_t :: seedIndiv(gaIndiv_t* indiv, int start_, int len_) {
	
	assert ((start_ < vec_length) && (start_ + len_ < vec_length));
	input_vec = input_vec.substr(0,start_) 
				+ (indiv->input_vec).substr(start_, len_);
	
}

void gaIndiv_t :: printIndiv(bool full_) {
	
	cout << endl << "I: " << index << endl;
	for (uint i = 0; i < input_vec.length(); i++) {
		cout << input_vec[i];
		if ((i+1) % CONST_NUM_INPUT_BITS == 0)
			cout << endl;
		#if defined(__or1200_1)
			if (i % CONST_NUM_INPUT_BITS == 5) {
				i += CONST_NUM_INPUT_BITS - 6;
				cout << endl;
			}
		#endif
	}
	
	if (full_) {
		cout << endl;

		for (state_pVec_iter it = state_list.begin();
				it != state_list.end(); ++it) {
			if (*it)
				(*it)->printState();
			else
				cout << "--" << endl;
		}
		cout << endl;
	}
		for (vector<int>::iterator it = branch_cycle.begin();
				it != branch_cycle.end(); ++it)
			cout << *it << " ";
		cout << endl;

		for (int i = 0; i < CONST_NUM_BRANCH; ++i)
			if (branch_cov[i])
				cout << i << "(" << branch_cov[i] << ") ";
		cout << endl;
	//}
	cout << endl;
}

// GA Population
gaPopulation_t :: gaPopulation_t(int pop_size_, int indiv_vec_length_) :
	pop_size(pop_size_),
	indiv_vec_length(indiv_vec_length_)
{
	indiv_vec = vector<gaIndiv_t*> (pop_size_, NULL);
	fitness_set = vector<fitness_t> (pop_size_, 0);
}

gaPopulation_t :: ~gaPopulation_t() {
	fitness_set.clear();
	for (int i = 0; i < pop_size; ++i)
		if (indiv_vec[i] != NULL) {
			delete indiv_vec[i];
			indiv_vec[i] = NULL;
		}
	indiv_vec.clear();
}
	
void gaPopulation_t :: initPopulation(const state_pVec& startPool) {
	for (int i = 0; i < pop_size; ++i) {
		indiv_vec[i] = new gaIndiv_t(i, indiv_vec_length);
		indiv_vec[i]->initRandom(0);

		int state_ind = rand() % startPool.size();
		assert(startPool[state_ind] != NULL);
		indiv_vec[i]->start_state = startPool[state_ind];

		// TODO
		//indiv_vec[i]->start_state = SelectTournament(startPool);
	}
	
}

int gaPopulation_t :: gaSelect(gaIndiv_t*& A, gaIndiv_t*& B) {
    
	uint p1, p2;

	p1 = SelectTournament(fitness_set);
	p2 = fitness_set.size() + 1;
	while ((p2 > fitness_set.size()) || (p1 == p2))
	{   p2 = SelectTournament(fitness_set);   }

	A = indiv_vec[p1];
	B = indiv_vec[p2];
	
	return 1;
}

int gaPopulation_t :: gaCrossover( gaIndiv_t* indA, gaIndiv_t* indB, 
	gaIndiv_t*& newIndA, gaIndiv_t*& newIndB) {
	
	assert(indA->input_vec.length() == indB->input_vec.length());
	assert(indA->input_vec.length() == (uint) indiv_vec_length * CONST_NUM_INPUT_BITS);

	newIndA = new gaIndiv_t(pop_size, indiv_vec_length);
	newIndB = new gaIndiv_t(pop_size, indiv_vec_length);
	
	newIndA->input_vec = vecIn_t(indiv_vec_length * CONST_NUM_INPUT_BITS, 'X');
	newIndB->input_vec = vecIn_t(indiv_vec_length * CONST_NUM_INPUT_BITS, 'X');

	bool indA_bit = true, indB_bit = false;
	for (int i = 0; i < indiv_vec_length * CONST_NUM_INPUT_BITS; i++) {
		bool bit = rand() & 0x01;
		if (bit) {
			newIndA->input_vec[i] = indA->input_vec[i];
			newIndB->input_vec[i] = indB->input_vec[i];
		}
		
		else {
			newIndA->input_vec[i] = indB->input_vec[i];
			newIndB->input_vec[i] = indA->input_vec[i];
		}

		if (i % 2)
			indA_bit ^= bit;
		else
			indB_bit ^= bit;
	}             

	/*	For selecting the start_state, we compute the parity of the bits 
	   	in the even and odd above and then assign either A or B	*/
	if (indA_bit)
		newIndA->start_state = indA->start_state;
	else
		newIndA->start_state = indB->start_state;

	if (indB_bit)
		newIndB->start_state = indB->start_state;
	else
		newIndB->start_state = indA->start_state;

	assert(newIndA->start_state != NULL);
	assert(newIndB->start_state != NULL);

	return 1;
}

void gaPopulation_t :: gaMutate() {
	ulint max_value = (CONST_NUM_INPUT_BITS * pop_size * indiv_vec_length);
	ulint mutate_bit = rand() % max_value;

	int indiv_index = mutate_bit & pop_size;
	int bit_index = mutate_bit / pop_size;

	if (indiv_vec[indiv_index]->input_vec[bit_index] == '1')
		indiv_vec[indiv_index]->input_vec[bit_index] = '0';
	else
		indiv_vec[indiv_index]->input_vec[bit_index] = '1';
		
}

void gaPopulation_t :: gaEvolve() {
	
//	cout << "Orig pop size: " << indiv_vec.size() << endl;
	assert (indiv_vec.size() == (uint)pop_size);

	std::sort(indiv_vec.begin(), indiv_vec.end(), compFitness);

//	cout << "Indiv before evolution: " << gaIndiv_t::mem_alloc_cnt << endl;
	/*	Clearing the state_list of all individuals	*/
	for (gaIndiv_pVec_iter gt = indiv_vec.begin(); gt != indiv_vec.end(); ++gt) {
		for (state_pVec_iter st = (*gt)->state_list.begin(); 
				st != (*gt)->state_list.end(); ++st)
			if (*st) {
				delete *st;
				*st = NULL;
			}
	}
	
	vector<gaIndiv_t*> new_indiv_vec(pop_size, NULL);
	vector<int> new_index_vec;
	int size_= 0;

	/*	Indiv Fwding */

	for (int i = 0; size_ < NUM_INDIV_FWD; ++i) {
	//for (int i = 0; i < NUM_INDIV_FWD; ++i) {
		int ind = rand() % (4 * NUM_INDIV_FWD);
		bool twinFlag = false;
		for (uint j = 0; j < new_indiv_vec.size(); ++j)
			if (new_indiv_vec[j] == indiv_vec[ind]) {
				twinFlag = true;
				break;
			}
		if (twinFlag == false) {
			new_indiv_vec[size_] = indiv_vec[ind];
			new_index_vec.push_back(ind);
			size_++;
		}
	}
	
	if (size_ % 2) {
		new_indiv_vec[size_] = NULL;
		size_--;
	}
//	cout << "Indiv FWD: " << new_indiv_vec.size() << ":" << size_ << endl;

	cout << "gaEvolve: " << endl;
	// Selection/Crossover	
	for (int i = size_; i < pop_size; i = i + 2) {

		gaIndiv_t *c1 = NULL, *c2 = NULL;
		gaIndiv_t *p1 = NULL, *p2 = NULL;
		
		gaSelect(p1, p2);
		assert ((p1 != NULL) && (p2 != NULL));

		gaCrossover(p1, p2, c1, c2);
		assert ((c1 != NULL) && (c2 != NULL));
//		cout << endl 
//			 << size_ 
//			 << ": " << c1->input_vec.length()
//			 << ": " << c2->input_vec.length() << endl;

		assert(c1->input_vec.length() == (uint)(indiv_vec_length * CONST_NUM_INPUT_BITS));
		assert(c2->input_vec.length() == (uint)(indiv_vec_length * CONST_NUM_INPUT_BITS));
		new_indiv_vec[size_++] = c1;
		new_indiv_vec[size_++] = c2;
	}

	assert (size_ == pop_size);
	
	for (vector<int>::iterator it = new_index_vec.begin();
			it != new_index_vec.end(); ++it) 
		indiv_vec[*it] = NULL;
	new_index_vec.clear();
	
//	cout << "Indiv after evolution: " << gaIndiv_t::mem_alloc_cnt << endl;

	int num_del = 0;
	for (gaIndiv_pVec_iter it = indiv_vec.begin();
			it != indiv_vec.end(); ++it) {
		if (*it != NULL) {
			delete *it;
			num_del++;
		}
	}
//	cout << "Deleted " << num_del << " indiv" << endl;
	indiv_vec.clear();

//	cout << "Indiv after deletion: " << gaIndiv_t::mem_alloc_cnt << endl;

	indiv_vec = new_indiv_vec;
	new_indiv_vec.clear();

	assert (indiv_vec.size() == (uint)pop_size);
	int ind = 0;
	for (gaIndiv_pVec_iter it = indiv_vec.begin(); it != indiv_vec.end(); ++it, ++ind) {
		assert((*it)->input_vec.length() == (uint)(indiv_vec_length * CONST_NUM_INPUT_BITS));
		(*it)->index = ind;
	}
	
}

//int SelectRoulette (const vector<fitness_t>& fitness_set) {
//    
//    fitness_t sum = 0;
//    for (int i = 0; i < fitness_set.size(); i++)
//        sum += fitness_set[i];
//
//    double rand_num = rand() / (double) (RAND_MAX);
//    double tmp = 0;
//
//    for (int i = 0; i < fitness_set.size(); i++) {
//        tmp += (double) fitness_set[i] / (double) sum;
//        if (tmp >= rand_num)
//            return i;
//    }
//
//    cerr << "Wrong! : SelectRoulette" << endl;
//    exit(-1);
//}

int SelectTournament (const vector<fitness_t>& fitness_set) {
    
    int c1, c2;

    int MAX_SIZE = fitness_set.size();
    assert(MAX_SIZE > 1);

    c1 = rand() % MAX_SIZE;
    c2 = rand() % MAX_SIZE;
	
    while (c1 == c2) {
        c2 = rand() % MAX_SIZE;
    }
	
    if (fitness_set[c1] >= fitness_set[c2])
        return c1;
    else
        return c2;

}

bool compFitness(gaIndiv_t* A, gaIndiv_t* B) { 
	return (A->fitness < B->fitness);
}
