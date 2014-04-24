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
	branchCov = vector<int>(CONST_NUM_BRANCH, 0);
}

gaIndiv_t::~gaIndiv_t() {
	input_vec.clear();
	for (int i = 0; i < vec_length; ++i) {
		if (state_list[i]) {
			delete state_list[i];
			state_list[i] = NULL;
		}
	}
	state_list.clear();
	branchCov.clear();
}

bool gaIndiv_t :: operator<(gaIndiv_t* indiv) const {
	// TODO

	return false;
}

bool gaIndiv_t :: operator==(gaIndiv_t* indiv) const {
	// TODO

	return false;
}

void gaIndiv_t :: simCkt(Vtop* top) {
	
//	cout << "simCkt: " << input_vec.length() << endl;
	assert(input_vec.length() == (uint)(vec_length * CONST_NUM_INPUT_BITS));
	int cycle = 0;
	sim_reset_clock(top);

	for (cycle = 0; cycle < vec_length; ++cycle )
		simCkt(top, cycle);

//	cout << "B-Cov" << endl;
//	for(int ind = 0; ind < CONST_NUM_BRANCH; ++ind)
//		cout << branchCov[ind] << " ";
//	cout << endl;
	return;
}

void gaIndiv_t :: simCkt(Vtop* top, int cycle) {
	
	assert(cycle < vec_length);

	vecIn_t inp = input_vec.substr(cycle*CONST_NUM_INPUT_BITS, CONST_NUM_INPUT_BITS);
	SimOneCycle(top, inp);

	input_vec.replace(cycle*CONST_NUM_INPUT_BITS, CONST_NUM_INPUT_BITS, inp);	
	
	state_t *tmp_state = new state_t(top, cycle);
	state_list[cycle] = tmp_state;

	for(vector<int>::iterator it = tmp_state->branch_index.begin();
			it != tmp_state->branch_index.end(); ++it)
		branchCov[*it]++;

	ResetCounters(top);
}
	
void gaIndiv_t :: initRandom(int start_) {
	
//	cout << "initRandom: "
//		 << input_vec.length() << " - " << (start_ * CONST_NUM_INPUT_BITS) << endl;

	assert (input_vec.length() == (uint)(start_ * CONST_NUM_INPUT_BITS));

    for (int i = start_; i < vec_length; i++) {
        vecIn_t tmpVec(CONST_NUM_INPUT_BITS, 'X');
        RandomVecIn(tmpVec);
        input_vec += tmpVec;
    }
//	cout << input_vec.length() << " - " << (vec_length * CONST_NUM_INPUT_BITS) << endl;
	assert(input_vec.length() == (uint)(vec_length * CONST_NUM_INPUT_BITS));
}

void gaIndiv_t :: seedIndiv(gaIndiv_t* indiv, int start_, int len_) {
	
	assert ((start_ < vec_length) && (start_ + len_ < vec_length));
	input_vec = input_vec.substr(0,start_) 
				+ (indiv->input_vec).substr(start_, len_);
	
}

void gaIndiv_t :: printIndiv(bool state_) {
	
	cout << "I: " << index << endl;
	//cout << endl << "# Vectors: " << inputVec.size()/CONST_NUM_INPUT_BITS << endl;
	for (uint i = 0; i < input_vec.length(); i++) {
		cout << input_vec[i];
		if ((i+1) % CONST_NUM_INPUT_BITS == 0)
			cout << endl;
	}
	
	if (state_) {
		for (stateVec_iter it = state_list.begin();
				it != state_list.end(); ++it) {
			(*it)->printState();
			cout << endl;
		}

		for (int i = 0; i < CONST_NUM_BRANCH; ++i)
			if (branchCov[i])
				cout << i << "(" << branchCov[i] << ") ";
		cout << endl;
	}
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
	
void gaPopulation_t :: initPopulation() {
	for (int i = 0; i < pop_size; ++i) {
		indiv_vec[i] = new gaIndiv_t(i, indiv_vec_length);
		indiv_vec[i]->initRandom(0);
	}
	
}

int gaPopulation_t :: gaSelect(gaIndiv_t*& A, gaIndiv_t*& B) {
    
	uint p1, p2;

//	case GA_ROULETTE:
//		p1 = SelectRoulette(fitness_set);
//		p2 = fitness_set.size() + 1;
//		while ((p2 > fitness_set.size()) || (p1 == p2))
//		{   p2 = SelectRoulette(fitness_set);   }
//
//		A = indiv_vec[p1];
//		B = indiv_vec[p2];
		
//	case GA_TOURNAMENT:
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

//	cout << "Crossover: " << endl
//		 << "A: " << indA->input_vec << endl
//		 << "B: " << indB->input_vec << endl
//		 << "X: ";
	for (int i = 0; i < indiv_vec_length * CONST_NUM_INPUT_BITS; i++) {
		bool bit = rand() & 0x01;
		cout << (uint)bit;
		if (bit) {
			newIndA->input_vec[i] = indA->input_vec[i];
			newIndB->input_vec[i] = indB->input_vec[i];
		}
		
		else {
			newIndA->input_vec[i] = indB->input_vec[i];
			newIndB->input_vec[i] = indA->input_vec[i];
		}
	}             

//	cout << endl
//		 << "1: " << newIndA->input_vec << endl
//		 << "2: " << newIndB->input_vec << endl;

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
	
	vector<gaIndiv_t*> new_indiv_vec(pop_size, NULL);
	vector<int> new_index_vec;
	int size_= 0;

	// Indiv Fwding 
	for (int i = 0; i < NUM_INDIV_FWD; ++i) {
		int ind = rand() % (2 * NUM_INDIV_FWD);
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

	for (gaIndiv_vec_iter it = indiv_vec.begin();
			it != indiv_vec.end(); ++it) {
		if (*it != NULL)
			delete *it;
	}
	indiv_vec.clear();

	indiv_vec = new_indiv_vec;
	
	assert (indiv_vec.size() == (uint)pop_size);
	int ind = 0;
	for (gaIndiv_vec_iter it = indiv_vec.begin(); it != indiv_vec.end(); ++it, ++ind) {
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