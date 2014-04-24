#include "verInclude.h"
#include "incl.h"
#include "verilated.h"
#include "gaLib.h"

//class gVarClass {
//
//public:
//	static string			fName, vName;
//
//	static vector<vecIn_t> 	inputComb;
//	static state_t			ctrlState;
//	static stateVec_t		ctrlStateVec;
//
//	static vecIn_t			resetInput;
//	static state_t			resetState;
//	static stateVec_t		resetStateVec;
//
//	static set<int>			brUncover;
//
//	static stateMap_t		stateMap;
//	static stateVec_t		stateList;
//
//	static vector<vecIn_t>	inputVec;
//};

rstIn_t gVarClass::resetInput = rstIn_t(CONST_NUM_INPUT_BITS, 'X');

class ga_Gen0_Param {
	
public:
	int 	NUM_INDIV, VEC_LENGTH;
	int 	NUM_TOP_INDIV;
	int 	NUM_GEN, POP_SIZE;

	vecIn_t			inputVec;
	state_t			*startState, *endState;
	vector<int>		branchHit;
	
	ga_Gen0_Param() {
		startState = endState = NULL;
	}
};

void ga_Gen0_generateVectors(ga_Gen0_Param* objGen0);
int checkCoverage(const vecIn_t&);
void mapDisplay(stateMap_t&);
void printVec(vector<int>&);
void PrintVectorSet(const vecIn_t& inputVec);

int main(int argc, char* argv[]) {

//#define _ResetMode_
#define random_seed
#ifdef random_seed
    uint seed = time(NULL);
    cout << seed << endl;
    srand(seed);
#endif

#ifdef __b12
    gVarClass::resetInput[0] = '1';
#endif

#ifdef _ResetMode_  
	char vecFile[100];
    if (argc < 2) {
        cout << "Missing parameter <vecFile> " << endl
            << "Input format: " << argv[0] << " <vecFile> " << endl;
        exit(-1);
    }

    uint numVec = -1;
    if (argc >= 2) {
        if (strcmp(argv[1], "-h") == 0) {
            cout << "Input format: " << endl
                << argv[0] << " "
                << argv[1] << " [<NumVec>]" << endl;
        }
        else {
            sprintf(vecFile, "%s", argv[1]);
            cout << "Reading " << vecFile << ".vec";

            if (argc >= 3) {
                numVec = atoi(argv[2]);
                cout << " - " << numVec << " vectors";
            }
            cout << endl;
        }
    }
#endif

//#ifdef _ResetMode_
   	ga_Gen0_Param *objGen0 = new ga_Gen0_Param;
	objGen0->NUM_GEN = 8;
	objGen0->POP_SIZE = 256;
	objGen0->VEC_LENGTH = 20/CONST_NUM_INPUT_BITS;
	objGen0->NUM_INDIV = 2;
	objGen0->NUM_TOP_INDIV = 4;

	ga_Gen0_generateVectors(objGen0);
	checkCoverage(objGen0->inputVec);
	PrintVectorSet(objGen0->inputVec);

	//ga_Gen0_generateVectors(objGen0);
	//checkCoverage(objGen0->inputVec);
	//PrintVectorSet(objGen0->inputVec);

    //ReadInputVectors(inputVec, numVec);

//#endif
}

int checkCoverage(const vecIn_t& inputVec) {
	
	Vtop* top = new Vtop;

	int numVec = inputVec.length()/CONST_NUM_INPUT_BITS;
	vector<int> brCoverage(CONST_NUM_BRANCH, 0);
	set<int> brSet;
	
	// Add reset branches to the set 
	vector<int> resetBranch;
	GetResetBranch(top, resetBranch);
	for (vector<int>::iterator it = resetBranch.begin();
			it != resetBranch.end(); ++it) {
		brSet.insert(*it);
	}

	int index;

	for (int i = 0; i < numVec; ++i) {
		string inputStr = inputVec.substr(i*CONST_NUM_INPUT_BITS, CONST_NUM_INPUT_BITS);
		cout << inputStr << endl;
		SimOneCycle(top, inputStr);
//		printCktState(top);

		vector<int> currBranch;
		GetCoverage(top, currBranch);
		for (vector<int>::iterator it = currBranch.begin();
				it != currBranch.end(); ++it) {
			brCoverage[*it]++;
			brSet.insert(*it);
		}
	
		if(brSet.size() == CONST_NUM_BRANCH-1) {
			index = i;
			cout << "#Vectors = " << index << endl;
			break;
		}

	}
	cout << "Coverage: " << GetCoverage(top, true) << endl;
	return index;
}


void ga_Gen0_generateVectors(ga_Gen0_Param* objGen) {

	int NUM_GEN	= objGen->NUM_GEN;
	int POP_SIZE = objGen->POP_SIZE;
	int VEC_LEN	= objGen->VEC_LENGTH;
	int NUM_INDIV = objGen->NUM_INDIV;
	int TOP_INDIV = objGen->NUM_TOP_INDIV;
	
	cout << "GA Stage 1: " << endl
		 << POP_SIZE << " individuals / " << VEC_LEN << " vectors" << endl;

	double WT_FIT_STATE = 0.3;
	double WT_FIT_BRANCH = 0.7;

	Vtop* cktVar = new Vtop;
	
	state_t *startState;
	SimMultiCycle(cktVar,0);
	if (objGen->startState == NULL) {
		cout << "New state" << endl;
		startState = new state_t(cktVar, 0);
	}
	else
		startState = objGen->startState;

	assert(startState != NULL);

	startState->printState();
	stateMap_t popStateMap;

	for (int round = 0; round < NUM_INDIV; ++round) {
		
//		if (round == 0)
//			startState = objGen->startState;
		
		cout << "R " << round << endl
			 << "Start state: ";
		startState->printState();

		gaPopulation_t gen0Pop(POP_SIZE,VEC_LEN);
		gen0Pop.initPopulation();

		for (int gen = 0; gen < NUM_GEN; ++gen) {	
			vector<int> fit_state(POP_SIZE);
			vector<int> fit_branch(POP_SIZE);
		
			vector<int> bCoverage(CONST_NUM_BRANCH, 0);
			int avgCov = 0;
			for (int i = 0; i < gen0Pop.pop_size; ++i) {
				// ClearCktVar
				ResetCounters(cktVar);

				// Set starting state
				startState->setCktState(cktVar);

				// Simulate each individual
				gaIndiv_t *tmpIndiv = gen0Pop.indiv_vec[i];
				assert (tmpIndiv->input_vec.length() == (uint)(VEC_LEN * CONST_NUM_INPUT_BITS));
				tmpIndiv->simCkt(cktVar);

				// Add states to StateMap
				state_t *currState = NULL;
				for (stateVec_iter it = tmpIndiv->state_list.begin(); 
						it != tmpIndiv->state_list.end(); ++it) {
					currState = *it;
					retVal_t ret;
					keyVal_t hash_value = currState->getHash();
					ret = popStateMap.insert(make_pair(hash_value, currState));

					currState->hit_count = 1;
					if (ret.second == false)
						(ret.first->second)->hit_count++;
				}

				// Add branch hits --> bCoverage
				GetBranchCounters(cktVar, tmpIndiv->branchCov);
				for (int br = 0; br < CONST_NUM_BRANCH; ++br)
					bCoverage[br] += tmpIndiv->branchCov[br];

				int cCov = GetCoverage(cktVar, false);
				cout << "Branch Coverage: [" << i << "]" << cCov << endl;

				avgCov += cCov;

			}
			
			cout << "Average Coverage: " << (double) avgCov / (double) POP_SIZE;

			// Compute Fitness 
			for (int i = 0; i < gen0Pop.pop_size; ++i) {
				
				// Add fitness for states
				gaIndiv_t* tmpIndiv = gen0Pop.indiv_vec[i];
				int state_fitness = 0;
				int state_ind = 0, step = 1;
				for (stateVec_iter it = tmpIndiv->state_list.begin(); 
						it != tmpIndiv->state_list.end(); ++it) {
					state_fitness += (step) * popStateMap[(*it)->getHash()]->hit_count;
					state_ind++;
					step += (state_ind >> step);
				}

				// Add fitness for branches
				int branch_fitness = 0;
				for (int it = 0; it < CONST_NUM_BRANCH; ++it)
					if(tmpIndiv->branchCov[it])
						branch_fitness += bCoverage[it];

				fit_state[i] = state_fitness;
				fit_branch[i] = branch_fitness;
			}

			// Evolve the individuals
			mapDisplay(popStateMap);
			popStateMap.clear();
				
			cout << "Branch Coverage" << endl;
			printVec(bCoverage);
			cout << endl;

			cout << "Fitness: " << endl;
			for (int i = 0; i < POP_SIZE; ++i) {
				gen0Pop.indiv_vec[i]->fitness 
					= (int) ((double)fit_state[i] * WT_FIT_STATE 
					+ (double)fit_branch[i] * WT_FIT_BRANCH);

				cout << i << ": " << fit_state[i] 
					 << "\t+ " << fit_branch[i]
					 << "\t=> " << gen0Pop.indiv_vec[i]->fitness << endl;
			}
			cout << endl;
			
			if (gen < NUM_GEN - 1)
				gen0Pop.gaEvolve();
			cout << endl << "END GEN: R " << round << ", G " << gen << endl;

		}

		//Set starting state
		std::sort(gen0Pop.indiv_vec.begin(), gen0Pop.indiv_vec.end(), compFitness);
		gaIndiv_t *ind_ = gen0Pop.indiv_vec[0];
		objGen->inputVec += ind_->input_vec;
				
		cout << "Indiv " << ind_->index << endl
			 << "Vectors after R " << round << endl;
		PrintVectorSet(objGen->inputVec);

		delete startState;
		startState = new state_t(*ind_->state_list[VEC_LEN - 1]);
		//ind_->state_list[VEC_LEN - 1] = NULL;

		cout <<"END ROUND: R " << round << endl;

	}

	objGen->startState = startState;

}

void mapDisplay(stateMap_t& stateMap) {
	for (stateMap_iter it = stateMap.begin(); it!= stateMap.end(); ++it) {
		cout /*<< it->first << "\t"*/ << it->second->hit_count << endl;
		(it->second)->printState();
		cout << endl;
	}
}

void printVec(vector<int>& vec_) {
	for (vector<int>::iterator vt = vec_.begin();
			vt != vec_.end(); ++vt)
		cout << *vt << " ";
}

void PrintVectorSet(const vecIn_t& inputVec ) {

	cout << endl << "# Vectors: " << inputVec.size()/CONST_NUM_INPUT_BITS << endl;
	for (uint i = 0; i < inputVec.length(); i++) {
		cout << inputVec[i];
		if ((i+1) % CONST_NUM_INPUT_BITS == 0)
			cout << endl;
	}

}
