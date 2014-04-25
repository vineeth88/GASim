#include "verInclude.h"
#include "incl.h"
#include "verilated.h"
#include "gaLib.h"

rstIn_t gVarClass::resetInput = rstIn_t(CONST_NUM_INPUT_BITS, 'X');

class ga_Gen0_Param {
	
public:
	string fName;

	int 	NUM_INDIV_0, INDIV_LEN_0;
	int 	NUM_TOP_INDIV_0;
	int 	NUM_GEN_0, POP_SIZE_0;

	vecIn_t			inputVec;
	state_t			*startState, *endState;
	vector<int>		branchHit;
	
	ga_Gen0_Param() {
		startState = endState = NULL;
		branchHit = vector<int>(CONST_NUM_BRANCH, 0);
	}

	~ga_Gen0_Param() {
		if (startState) {
			delete startState;
			startState = NULL;
		}
		if(endState) {
			delete endState;
			endState = NULL;
		}
		branchHit = vector<int>();
	}
};

void readParam(ga_Gen0_Param*);
void ga_Gen0_generateVectors(ga_Gen0_Param*);

int checkCoverage(const vecIn_t&, vector<int>&);
void mapDisplay(stateMap_t&);
void printVec(vector<int>&);
void printCnt(vector<int>&);
void PrintVectorSet(const vecIn_t& inputVec, int vecIndex = -1);

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
	
	char paramFile[100];
	sprintf(paramFile, "%s.param", benchCkt);

	if (argc >= 2) {
        if (strcmp(argv[1], "-h") == 0) {
            cout << "Input format: " << endl
                << argv[0] << " "
                << argv[1] << " <ckt>.param " << endl;
        }
        else {
            sprintf(paramFile, "%s", argv[1]);
            cout << "Reading " << paramFile << endl;
        }
	}		

   	ga_Gen0_Param *objGen0 = new ga_Gen0_Param;
	objGen0->fName = string(paramFile);
	readParam(objGen0);

//	objGen0->NUM_GEN = 8;
//	objGen0->POP_SIZE = 256;
//	objGen0->VEC_LENGTH = 20/CONST_NUM_INPUT_BITS;
//	objGen0->NUM_INDIV = 5;
//	objGen0->NUM_TOP_INDIV = 4;

#ifdef __b12
	objGen0->inputVec += vecIn_t("00000");
#endif

	ga_Gen0_generateVectors(objGen0);
	
	vector<int> brCoverage;
	int vecIndex = checkCoverage(objGen0->inputVec, brCoverage);
	PrintVectorSet(objGen0->inputVec, vecIndex);
	if (objGen0->inputVec.length() > (uint) (vecIndex * CONST_NUM_INPUT_BITS))
		cout << "Max coverage achieved in " << vecIndex << " vectors" << endl;
	else {



	}

	delete objGen0;

}


void ga_Gen0_generateVectors(ga_Gen0_Param* objGen) {

	int NUM_GEN	= objGen->NUM_GEN_0;
	int POP_SIZE = objGen->POP_SIZE_0;
	int VEC_LEN	= objGen->INDIV_LEN_0 / CONST_NUM_INPUT_BITS;
	int NUM_INDIV = objGen->NUM_INDIV_0;
	//int TOP_INDIV = objGen->NUM_TOP_INDIV;
	
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

			int covThresh = avgCov + (maxCov - avgCov + 1)/2;
			// Compute Fitness 
			for (int i = 0; i < gen0Pop.pop_size; ++i) {
				
			 	gaIndiv_t* tmpIndiv = gen0Pop.indiv_vec[i];
				 	
			 	if (tmpIndiv->nBranches > covThresh)
			 		tmpIndiv->printIndiv(1);
			 	else
			 		tmpIndiv->printIndiv(0);

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
	delete cktVar;

	cout << "End of Stage 1" << endl << endl;
}

int checkCoverage(const vecIn_t& inputVec, vector<int>& brCoverage) {
	
	Vtop* ckt = new Vtop;

	int numVec = inputVec.length()/CONST_NUM_INPUT_BITS;
	brCoverage = vector<int>(CONST_NUM_BRANCH, 0);
	
	// Add reset branches to the set 
	vector<int> resetBranch;
	GetResetBranch(ckt, resetBranch);
	for (vector<int>::iterator it = resetBranch.begin();
			it != resetBranch.end(); ++it) {
		brCoverage[*it] = 1;
	}

	int index = -1;
	int nBranch = resetBranch.size();

	for (int i = 0; i < numVec; ++i) {
		string inputStr = inputVec.substr(i*CONST_NUM_INPUT_BITS, CONST_NUM_INPUT_BITS);
		cout << inputStr << endl;
		SimOneCycle(ckt, inputStr);
//		printCktState(ckt);

		vector<int> currBranch;
		GetCoverage(ckt, currBranch);
		for (vector<int>::iterator it = currBranch.begin();
				it != currBranch.end(); ++it) {
			if (brCoverage[*it] == 0)
				nBranch++;
			brCoverage[*it]++;
		}
	
		if(nBranch == CONST_NUM_BRANCH-1) {
			index = i;
			cout << "#Vectors = " << index << endl;
			break;
		}

	}
	cout << "Coverage: " << GetCoverage(ckt, true) << endl;

	delete ckt;
	return index;

}

void readParam(ga_Gen0_Param* paramObj) {
	
	ifstream paramIn;
	paramIn.open((paramObj->fName).c_str(), ios::in);

	if (!paramIn) {
		cout << "Unable to read " << paramObj->fName << endl
			 << "Using default values. " << endl
			 << "NUM_GEN_0 	: 4 generations" << endl
			 << "POP_SIZE_0	: 256 individuals" << endl
			 << "INDIV_LEN_0: 20 bits" << endl
			 << "NUM_INDIV_0: 2 rounds" << endl;
		
		paramObj->NUM_GEN_0 = 4;
		paramObj->POP_SIZE_0 = 256;
		paramObj->INDIV_LEN_0 = 20;
		paramObj->NUM_INDIV_0 = 2;
	}	
	else {
		while(paramIn) {
			string curr;
			getline(paramIn, curr);

			if (curr.find("NUM_GEN_0") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "NUM_GEN_0 = " << val << endl;
					paramObj->NUM_GEN_0 = val;
				}
				else 
					paramObj->NUM_GEN_0 = 4;
			}
			else if (curr.find("POP_SIZE_0") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "POP_SIZE_0 = " << val << endl;
					paramObj->POP_SIZE_0 = val;
				}
				else 
					paramObj->POP_SIZE_0 = 256;
			}
			else if (curr.find("INDIV_LEN_0") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "INDIV_LEN_0 = " << val << endl;
					paramObj->INDIV_LEN_0 = val;
				}
				else 
					paramObj->INDIV_LEN_0 = 20;
			}
			else if (curr.find("NUM_INDIV_0") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "NUM_INDIV_0 = " << val << endl;
					paramObj->NUM_INDIV_0 = val;
				}
				else 
					paramObj->NUM_INDIV_0 = 2;
			}

		}
		cout << "Finished parsing " << paramObj->fName << endl;
		paramIn.close();
	}
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
	for (uint v = 0; v < vec_.size(); ++v)
		cout << v << "(" << vec_[v] << ") ";
	cout << endl;
}

void PrintVectorSet(const vecIn_t& inputVec, int vecIndex) {
	
	uint len = 0;
	if (vecIndex < 0)
		len = inputVec.length();
	else {
		len = (uint) (vecIndex * CONST_NUM_INPUT_BITS);
		if (len > inputVec.length())
			len = inputVec.length();
	}
	cout << endl << "# Vectors: " << len/(uint) CONST_NUM_INPUT_BITS << endl;
	for (uint i = 0; i < len; i++) {
		cout << inputVec[i];
		if ((i+1) % CONST_NUM_INPUT_BITS == 0)
			cout << endl;
	}

}
