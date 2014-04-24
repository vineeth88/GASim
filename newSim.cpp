#include "verInclude.h"
#include "incl.h"
#include "verilated.h"
#include "gaLib.h"

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
		branchHit = vector<int>(CONST_NUM_BRANCH, 0);
	}
};

void ga_Gen0_generateVectors(ga_Gen0_Param* objGen0);
int checkCoverage(const vecIn_t&);
void mapDisplay(stateMap_t&);
void printVec(vector<int>&);
void printCnt(vector<int>&);
void PrintVectorSet(const vecIn_t& inputVec);

int main(int argc, char* argv[]) {

//#define random_seed
#ifdef random_seed
    uint seed = time(NULL);
    cout << seed << endl;
    srand(seed);
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

   	ga_Gen0_Param *objGen0 = new ga_Gen0_Param;
	objGen0->NUM_GEN = 8;
	objGen0->POP_SIZE = 256;
	objGen0->VEC_LENGTH = 20/CONST_NUM_INPUT_BITS;
	objGen0->NUM_INDIV = 5;
	objGen0->NUM_TOP_INDIV = 4;

#ifdef __b12
	objGen0->inputVec += vecIn_t("00000");

#endif
	ga_Gen0_generateVectors(objGen0);
	checkCoverage(objGen0->inputVec);
	PrintVectorSet(objGen0->inputVec);

}

void ga_Gen0_generateVectors(ga_Gen0_Param* objGen) {

	int NUM_GEN	= objGen->NUM_GEN;
	int POP_SIZE = objGen->POP_SIZE;
	int VEC_LEN	= objGen->VEC_LENGTH;
	int NUM_INDIV = objGen->NUM_INDIV;
	int TOP_INDIV = objGen->NUM_TOP_INDIV;
	
	cout << "GA Stage 1: " << endl
		 << POP_SIZE << " individuals / " << VEC_LEN << " vectors" << endl;

	// Parameters
	double WT_FIT_STATE = 0.7;
	double WT_FIT_BRANCH = 0.3;

	int CONST_FIT_COV = 500;

	// Circuit & Starting state
	Vtop* cktVar = new Vtop;
	
	state_t *startState;
	if (objGen->inputVec.length()/CONST_NUM_INPUT_BITS)
		SimMultiCycle(cktVar,objGen->inputVec);
	else
		SimMultiCycle(cktVar,0);

	for (int i = 0; i < CONST_NUM_BRANCH; ++i)
		objGen->branchHit[i] = GetCoverage(cktVar, i);

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
			int maxCov = 0, maxInd = -1;

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
				int cCov = 0;
				for (int br = 0; br < CONST_NUM_BRANCH; ++br) {
					bCoverage[br] += tmpIndiv->branchCov[br];
					if (tmpIndiv->branchCov[br])
						cCov++;
				}
				
				tmpIndiv->nBranches = cCov;
				cout << "Branch Cov[" << i << "]: " << cCov << endl;

				if (maxCov < cCov) {
					maxCov = cCov;
					maxInd = i;
				}
				avgCov += cCov;

			}
				
			avgCov = (((double)avgCov / (double)POP_SIZE) + 0.5);
			cout << endl
				 << "Average Coverage: " << avgCov << endl
				 << "Max Coverage: " << maxCov << " (" << maxInd << ")" << endl;


			// Compute Fitness 
			for (int i = 0; i < gen0Pop.pop_size; ++i) {
				
			 	gaIndiv_t* tmpIndiv = gen0Pop.indiv_vec[i];
			 	
			// 	if (tmpIndiv->nBranches > avgCov / POP_SIZE)
			 		tmpIndiv->printIndiv(1);
			// 	else
			// 		tmpIndiv->printIndiv(0);

				// Fitness for coverage
				int cov_fitness = ((avgCov - tmpIndiv->nBranches) * CONST_FIT_COV);
				cout << "Cov F[" << i << "]: " << cov_fitness << endl;

				// Fitness for each state (branches hit)
				int state_fitness = 0;
				int state_fit_arr[VEC_LEN], fit_ind = 0;
				int max_fit = (2 << 29), max_ind = -1;
				int step = 1;

				cout << "St/Br F[" << i << "]: ";
			 	for (stateVec_iter it = tmpIndiv->state_list.begin(); 
			 			it != tmpIndiv->state_list.end(); ++it) {
					int num_branch_hit = 0;
					state_fit_arr[fit_ind] = 0;
					for (vector<int>::iterator vt = (*it)->branch_index.begin();
							vt != (*it)->branch_index.end(); ++vt) {
						if (bCoverage[*vt] != VEC_LEN * POP_SIZE) {
							num_branch_hit++;
							state_fit_arr[fit_ind] += bCoverage[*vt];
						}
					}
					if (num_branch_hit)
						state_fit_arr[fit_ind] /= num_branch_hit;

					state_fitness += step * state_fit_arr[fit_ind];
					cout << "[" << step * state_fit_arr[fit_ind] 
						 << ", " << state_fitness/(fit_ind+1) << "] ";

					if (max_fit > state_fit_arr[fit_ind]) {
						max_fit = state_fit_arr[fit_ind];
						max_ind = fit_ind;
					}

					fit_ind++;
					step += (fit_ind >> step);

			 	}
				cout << endl << "Max: " << max_fit << "(" << max_ind << ")" << endl;

				cout << "St F[" << i << "]: ";
				int state_fit = 0;
			 	for (stateVec_iter it = tmpIndiv->state_list.begin(); 
			 			it != tmpIndiv->state_list.end(); ++it) {
					int tmp = popStateMap[(*it)->getHash()]->hit_count;
			 		state_fit += tmp;
					cout << tmp << " ";
			 	}
				cout << endl << state_fit << endl;

			// 	// Add fitness for branches
			// 	if (tmpIndiv->nBranches == maxCov)
			// 		cout << "Branch Fitness: " << endl;
			// 	int branch_fitness = 0;
			// 	for (int it = 0; it < CONST_NUM_BRANCH; ++it)
			// 		if(	(tmpIndiv->branchCov[it]) && 
			// 				(bCoverage[it] != VEC_LEN * POP_SIZE)) {

			// 			if (tmpIndiv->nBranches == maxCov)
			// 				cout << bCoverage[it] << " ";

			// 			branch_fitness += bCoverage[it];
			// 		}
			// 	if (tmpIndiv->nBranches == maxCov)
			// 		cout << " = " << branch_fitness << endl << endl;

			 	fit_state[i] = state_fitness;
			 	fit_branch[i] = cov_fitness;
			}

			// Evolve the individuals
			mapDisplay(popStateMap);
			popStateMap.clear();
				
			cout << "Branch Coverage" << endl;
			printCnt(bCoverage);
			cout << endl;

			cout << "Fitness: " << endl;
			for (int i = 0; i < POP_SIZE; ++i) {
				gen0Pop.indiv_vec[i]->fitness 
					= (int) ((double)fit_state[i] * WT_FIT_STATE 
					//+ (double)fit_branch[i] * WT_FIT_BRANCH);
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
		gaIndiv_t *top_indiv_ = gen0Pop.indiv_vec[0];
		objGen->inputVec += top_indiv_->input_vec;
				
		cout << "Indiv " << top_indiv_->index << endl
			 << "Vectors after R " << round << endl;
		PrintVectorSet(objGen->inputVec);

		cout << "Branches covered: " << endl;
		for (int brInd = 0; brInd < CONST_NUM_BRANCH; ++brInd) {
			if (top_indiv_->branchCov[brInd])
				cout << brInd << " ";
			objGen->branchHit[brInd] += top_indiv_->branchCov[brInd];			
		}
		cout << endl;

		delete startState;
		startState = new state_t(*top_indiv_->state_list[VEC_LEN - 1]);

		cout <<"END ROUND: R " << round << endl;

	}

	objGen->endState = startState;

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

void printCnt(vector<int>& vec_) {
	for (int v = 0; v < vec_.size(); ++v)
		cout << v << "(" << vec_[v] << ") ";
	cout << endl;
}

void PrintVectorSet(const vecIn_t& inputVec ) {

	cout << endl << "# Vectors: " << inputVec.size()/CONST_NUM_INPUT_BITS << endl;
	for (uint i = 0; i < inputVec.length(); i++) {
		cout << inputVec[i];
		if ((i+1) % CONST_NUM_INPUT_BITS == 0)
			cout << endl;
	}

}
