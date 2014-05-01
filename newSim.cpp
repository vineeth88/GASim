#include "verInclude.h"
#include "incl.h"
#include "verilated.h"
#include "gaLib.h"

rstIn_t gVarClass::resetInput = rstIn_t(CONST_NUM_INPUT_BITS, 'X');
vector<int> leafBranch(CONST_NUM_BRANCH, 0);

class Stage0_Param {

public:

	string fName; 

	int 	NUM_INDIV, INDIV_LEN;
	int 	NUM_TOP_INDIV;
	int 	NUM_GEN, POP_SIZE;

	vecIn_t			inputVec;
	state_t			*startState, *endState;
	vector<int>		branchHit;
	
	Stage0_Param() {
		startState = endState = NULL;
		branchHit = vector<int>(CONST_NUM_BRANCH, 0);
	}

	~Stage0_Param() {
		if(startState) {
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

class Stage1_Param : public Stage0_Param {
	
	state_t *endState;
	public:

	state_t 		*startState;
	vector<int>		branchHit;

	Stage1_Param() {	
		startState = NULL;
		branchHit = vector<int>(CONST_NUM_BRANCH, 0);
	}

	Stage1_Param(Stage0_Param* obj) {	
		NUM_INDIV 	= obj->NUM_INDIV;	
		NUM_GEN 	= obj->NUM_GEN;
		INDIV_LEN 	= obj->INDIV_LEN;
		POP_SIZE 	= obj->POP_SIZE;
		
		fName 		= obj->fName;

		startState = NULL;
		branchHit = obj->branchHit;
	}

	~Stage1_Param() {
		if (startState) {
			delete startState;
			startState = NULL;
		}
	}

};

void Stage0_GenerateVectors(Stage0_Param*);
void readParam(Stage0_Param*);
void readParam(Stage1_Param*);

void Stage1_GenerateVectors(Stage1_Param*);

int checkCoverage(const vecIn_t&);
void DisplayMap(stateMap_t&);
void printVec(vector<int>&);
void printCnt(vector<int>&);
void PrintVectorSet(const vecIn_t& inputVec);
bool compCoverage(gaIndiv_t*, gaIndiv_t*);

int main(int argc, char* argv[]) {

#define random_seed
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


   	Stage0_Param *paramObj1 = new Stage0_Param;
	paramObj1->NUM_GEN = 8;
	paramObj1->POP_SIZE = 256;
	paramObj1->INDIV_LEN = 20;
	paramObj1->NUM_INDIV = 5;
	paramObj1->NUM_TOP_INDIV = 4;

	paramObj1->fName = string(paramFile);
	readParam(paramObj1);
	
	Stage0_GenerateVectors(paramObj1);
	//checkCoverage(paramObj1->inputVec);
	PrintVectorSet(paramObj1->inputVec);

//	#if defined(__b11) || defined(__b12)
	cout << "Stage 2 " << endl << endl;
	Stage1_Param *paramObj2 = new Stage1_Param(paramObj1);;
	paramObj2->inputVec = paramObj1->inputVec;
	readParam(paramObj2);
	Stage1_GenerateVectors(paramObj2);

	delete paramObj2;
//	#endif 

	delete paramObj1;
}

void Stage1_GenerateVectors(Stage1_Param* paramObj) {

	cout << "GA Stage 2: " << endl
		 << "Stage 1 Coverage: " << endl;
	printCnt(paramObj->branchHit);
	cout << endl;
	
	cout << "Uncovered Branches: " << endl;
	int num_branch_uncovered = 0;
	for (int ind = 0; ind < CONST_NUM_BRANCH; ++ind) {
		if(IsDefaultBranch(ind))
			paramObj->branchHit[ind] = -5;

		if (paramObj->branchHit[ind] == 0) {
			cout << ind << " ";
			num_branch_uncovered++;
		}
	}
	cout << endl << "% Coverage " << (float) (CONST_NUM_BRANCH - num_branch_uncovered) * 100.0f / (float) (CONST_NUM_BRANCH) << endl;

}

void Stage0_GenerateVectors(Stage0_Param* paramObj) {
	
	// Parameters
	int NUM_GEN	= paramObj->NUM_GEN;
	int POP_SIZE = paramObj->POP_SIZE;
	int VEC_LEN	= paramObj->INDIV_LEN / CONST_NUM_INPUT_BITS;
	int NUM_INDIV = paramObj->NUM_INDIV;
	int TOP_INDIV = paramObj->NUM_TOP_INDIV;
	
	double WT_FIT_STATE = 0.7;
	double WT_FIT_BRANCH = 0.3;
	int WT_FIT_COV = 500;

	cout << "GA Stage 1: " << endl
		 << POP_SIZE << " individuals / " << VEC_LEN << " vectors" << endl;
	
	Vtop *cktVar = new Vtop;

	state_pVec startPool;
	vector<int> branch_counters(CONST_NUM_BRANCH, 0);
	state_t *rstState;

	SimMultiCycle(cktVar, 0);
	for (int br = 0; br < CONST_NUM_BRANCH; ++br)
		if (GetCoverage(cktVar, br))
			branch_counters[br] = 1;

	rstState = new state_t(cktVar, 0);
	rstState->pIndiv = NULL;
	startPool.push_back(rstState);
	
	cout << "Reset state: " << endl;
	rstState->printState();
	cout << endl;

	{
		
		gaPopulation_t stage0Pop(POP_SIZE, VEC_LEN);
		/*	Init population with one of the states in the pool 
			of start states and string of random vectors	*/
		stage0Pop.initPopulation(startPool);
	
		stateMap_t currStateMap;
		int prevMaxCov = 0;

		bool gaTerminate = false;
		for (int gen = 0; !gaTerminate && (gen < NUM_GEN); ++gen) {
			
			cout << "GEN " << gen << endl << endl;
			vector<int> currBranchCov(CONST_NUM_BRANCH, 0);

			bool improv_cov = false; 
			bool new_state = false;

			int maxCov = 0, avgCov = 0;
			vector<int> maxInd;

			for (int ind = 0; ind < POP_SIZE; ++ind) {
				
				ResetCounters(cktVar);
				gaIndiv_t* indiv = stage0Pop.indiv_vec[ind];
				indiv->simCkt(cktVar);

				/* Add states to stateMap	*/
//				for (uint st = 0; st < indiv->state_list.size(); ++st) {
//					state_t *curr = indiv->state_list[st];
//					keyVal_t hash_val_ = curr->getHash();
//					retVal_t ret = currStateMap.insert(make_pair(hash_val_, curr));
//					
//					curr->hit_count = 1;
//					if (ret.second == false) {
//						//cout << (ret.first->second)->getHash();
//						(ret.first->second)->printState();
//						(ret.first->second)->hit_count++;
//					}
//					else {
//						new_state = true;
//						cout << "New State " << curr->getHash() << endl
//							 << "Gen " << gen << " Indiv " << ind << endl;
//					}
//				}

				for (state_pVec_iter st = indiv->state_list.begin(); 
						st != indiv->state_list.end(); ++st) {
					state_t *curr = *st;
					keyVal_t hash_val_ = curr->getHash();
					retVal_t ret = currStateMap.insert(make_pair(hash_val_, curr));
					
					curr->hit_count = 1;
					if (ret.second == false) {
						//cout << (ret.first->second)->getHash();
						if (ret.first->second)
							(ret.first->second)->hit_count++;
						else 
							(ret.first->second) = curr;

					//	cout << "Ex State: " << (ret.first->second)->getHash() << endl;
					//	(ret.first->second)->printState();
					}
					else {
						new_state = true;
						cout << "New State " << curr->getHash() << endl
							 << "Gen " << gen << " Indiv " << ind << endl;
					}
				}

				/* Add branch hits to currBranchCov */
				for (int br = 0; br < CONST_NUM_BRANCH; ++br) {
					currBranchCov[br] += indiv->branch_cov[br];
				}

				/* Computing max and avg branch coverage */
				if (maxCov < indiv->num_branch)
					maxCov = indiv->num_branch;
				
				avgCov += indiv->num_branch;

			}
			
			cout << "Avg Coverage: " << (double)avgCov / (double)POP_SIZE << endl
				 << "Max Coverage: " << maxCov << endl;

			avgCov = (fitness_t)((double)avgCov / (double)POP_SIZE + 0.5);
			improv_cov = (maxCov >= prevMaxCov);

			cout << "State Map: " << endl;
			DisplayMap(currStateMap);
				
			cout << "Branch Coverage: " << endl;
			printCnt(currBranchCov);
			cout << endl;

			for (int ind = 0; ind < POP_SIZE; ++ind) {

				gaIndiv_t *indiv = stage0Pop.indiv_vec[ind];
				if (indiv->num_branch == maxCov) {
					cout << ind << " ";
					maxInd.push_back(ind);
				}

			}
			cout << endl;	

			/* Fittest Individuals	*/
			for (vector<int>::iterator it = maxInd.begin();
					it != maxInd.end(); ++it)
				stage0Pop.indiv_vec[*it]->printIndiv(1);
			cout << endl;
	
			/* Compute Fitness	*/
			for (int ind = 0; ind < POP_SIZE; ++ind) {

				gaIndiv_t *indiv = stage0Pop.indiv_vec[ind];

				/* Fitness for #branches covered */
				fitness_t fitness_cov = avgCov - indiv->num_branch;

				/* Fitness for branch coverage	*/
				fitness_t fitness_branch = 0;
//				int state_fit_arr[VEC_LEN], fit_ind = 0;
//				int max_fit = (2 << 29), max_ind = -1;
//				int step = 1;
//			 	for (state_pVec_iter it = indiv->state_list.begin(); 
//			 			it != indiv->state_list.end(); ++it) {
//					int num_branch_hit = 0;
//					state_fit_arr[fit_ind] = 0;
//					for (vector<int>::iterator vt = (*it)->branch_index.begin();
//							vt != (*it)->branch_index.end(); ++vt) {
////						if ((currBranchCov[*vt] != VEC_LEN * POP_SIZE) &&
////								(leafBranch[*vt])) {
//						if (currBranchCov[*vt] != VEC_LEN * POP_SIZE) {
//							num_branch_hit++;
//							
//							if(leafBranch[*vt])
//								state_fit_arr[fit_ind] += currBranchCov[*vt];
//
//						}
//					}
//					if (num_branch_hit)
//						state_fit_arr[fit_ind] /= num_branch_hit;
//
//					fitness_branch += step * state_fit_arr[fit_ind];
//					cout << "[" << step * state_fit_arr[fit_ind] 
//						 << ", " << fitness_branch/(fit_ind+1) << "] ";
//
//					if (max_fit > fitness_branch/(fit_ind+1)) {
//						max_fit = fitness_branch/(fit_ind+1);
//						max_ind = fit_ind;
//					}
//
//					fit_ind++;
//					step += (fit_ind >> step);
//
//			 	}
//				cout << endl << "Max fitness: " << max_fit << ", " << max_ind << endl;

				/* Fitness for states reached 	*/
				fitness_t fitness_state = 0;
//			 	for (state_pVec_iter it = indiv->state_list.begin(); 
//			 			it != indiv->state_list.end(); ++it) {
//					int tmp = currStateMap[(*it)->getHash()]->hit_count;
//			 		fitness_state += tmp;
//					cout << tmp << " ";
//			 	}
//				cout << endl;

				/* Combining all fitness values	*/
				indiv->fitness 
					= (fitness_cov * WT_FIT_COV) 
					+ (fitness_branch * WT_FIT_BRANCH)
					+ (fitness_state * WT_FIT_STATE);
				cout << "Fitness (" << ind << ") " << indiv->fitness 
					 << " [" << fitness_cov 
					 << ", " << fitness_branch
					 << ", " << fitness_state << "]" << endl 
					 << endl;
			}	
			

			/* Terminate GA Condition:
				- gen == NUM_GEN
				- coverage[gen] <= coverage[gen-1]
				- no new states reached	*/
			
//			if (gen == NUM_GEN)
//				gaTerminate = false;
//			else if (!improv_cov)
//				gaTerminate = false;
//			else if (!new_state)
//				gaTerminate == false;
			
			gaTerminate = (gen == NUM_GEN) | (!improv_cov) | (!new_state);

			if (!gaTerminate) {

				cout << "States found " << gen << endl;
				for (stateMap_iter st = currStateMap.begin(); 
						st != currStateMap.end(); ++st) {
					cout << (st->first) << endl;
					(st->second) = NULL;
				}
				DisplayMap(currStateMap);

				stage0Pop.gaEvolve();
				
				prevMaxCov = maxCov;
			}
			else
				cout << "Terminating after gen " << gen << endl;

		}
		
		/* Mine for interesting individuals and states 	*/
		cout << "States reached after Round 0" << endl;
		DisplayMap(currStateMap);
		cout << endl;

		std::sort(stage0Pop.indiv_vec.begin(), stage0Pop.indiv_vec.end(), compCoverage);
		cout << "Population after Round 0" << endl;
		int pInd = 0;
		for (gaIndiv_pVec_iter indiv = stage0Pop.indiv_vec.begin();
				indiv != stage0Pop.indiv_vec.end(); ++indiv) {
			cout << pInd << endl;
			(*indiv)->printIndiv(0);
			cout << (*indiv)->num_branch << " branches with "
				 << (*indiv)->max_index + 1 << " vectors" << endl;
			//getchar();
			pInd++;
		}
		
		gaIndiv_t *indiv = stage0Pop.indiv_vec[0];

		paramObj->inputVec = (indiv->input_vec).substr(0,CONST_NUM_INPUT_BITS*(indiv->max_index+1));
		paramObj->endState = new state_t(*indiv->state_list[indiv->max_index]);

		paramObj->branchHit = vector<int>(CONST_NUM_BRANCH, 0);
		SimMultiCycle(cktVar,0);
		GetBranchCounters(cktVar, paramObj->branchHit);
		for (int ind = 0; ind <= indiv->max_index; ++ind) {
			state_t *st = indiv->state_list[ind];
			for (vector<int>::iterator it = st->branch_index.begin();
					it != st->branch_index.end(); ++it)
				paramObj->branchHit[*it]++;
		}
		
		cout << "Branches covered after Stage 1: " << endl;
		printCnt(paramObj->branchHit);
	}

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
	delete top;

	return index;
}

void readParam(Stage0_Param* paramObj) {

	ifstream paramIn;
	paramIn.open((paramObj->fName).c_str(), ios::in);

	if (!paramIn) {
		cout << "Unable to read " << paramObj->fName << endl
			 << "Using default values. " << endl
			 << "NUM_GEN_0 	: 4 generations" << endl
			 << "POP_SIZE_0	: 256 individuals" << endl
			 << "INDIV_LEN_0: 20 bits" << endl
			 << "NUM_INDIV_0: 2 rounds" << endl;

		paramObj->NUM_GEN = 4;
		paramObj->POP_SIZE = 256;
		paramObj->INDIV_LEN = 20;
		paramObj->NUM_INDIV = 2;
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
					cout << "NUM_GEN = " << val << endl;
					paramObj->NUM_GEN = val;
				}
				else 
					paramObj->NUM_GEN = 4;
			}
			else if (curr.find("POP_SIZE_0") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "POP_SIZE_0 = " << val << endl;
					paramObj->POP_SIZE = val;
				}
				else 
					paramObj->POP_SIZE = 256;
			}
			else if (curr.find("INDIV_LEN_0") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "INDIV_LEN_0 = " << val << endl;
					paramObj->INDIV_LEN = val;
				}
				else 
					paramObj->INDIV_LEN = 20;
			}
			else if (curr.find("NUM_INDIV_0") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "NUM_INDIV_0 = " << val << endl;
					paramObj->NUM_INDIV = val;
				}
				else 
					paramObj->NUM_INDIV = 2;
			}

		}
		cout << "Finished parsing " << paramObj->fName << endl;
		paramIn.close();
	}
}

void readParam(Stage1_Param* paramObj) {

	ifstream paramIn;
	paramIn.open((paramObj->fName).c_str(), ios::in);

	if (!paramIn) {
		cout << "Unable to read " << paramObj->fName << endl
			 << "Using default values. " << endl
			 << "NUM_GEN_1 	: 4 generations" << endl
			 << "POP_SIZE_1	: 256 individuals" << endl
			 << "INDIV_LEN_1: 20 bits" << endl;

		paramObj->NUM_GEN = 4;
		paramObj->POP_SIZE = 256;
		paramObj->INDIV_LEN = 20;
	}	
	else {
		while(paramIn) {
			string curr;
			getline(paramIn, curr);

			if (curr.find("NUM_GEN_1") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "NUM_GEN_1 = " << val << endl;
					paramObj->NUM_GEN = val;
				}
				else 
					paramObj->NUM_GEN = 4;
			}
			else if (curr.find("POP_SIZE_1") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "POP_SIZE_1 = " << val << endl;
					paramObj->POP_SIZE = val;
				}
				else 
					paramObj->POP_SIZE = 256;
			}
			else if (curr.find("INDIV_LEN_1") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "INDIV_LEN_1 = " << val << endl;
					paramObj->INDIV_LEN = val;
				}
				else 
					paramObj->INDIV_LEN = 20;
			}
		}
		cout << "Finished parsing " << paramObj->fName << endl;
		paramIn.close();
	}
}

void DisplayMap(stateMap_t& stateMap) {
	for (stateMap_iter it = stateMap.begin(); it!= stateMap.end(); ++it) {

		cout << it->first << "\t";
		if (it->second) {
			cout << it->second->hit_count << endl;
			(it->second)->printState();
		}
		else 
			cout << "NULL" << endl;

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

void PrintVectorSet(const vecIn_t& inputVec ) {

	cout << endl << "# Vectors: " << inputVec.length()/CONST_NUM_INPUT_BITS << endl;
	for (uint i = 0; i < inputVec.length(); i++) {
		cout << inputVec[i];
		if ((i+1) % CONST_NUM_INPUT_BITS == 0)
			cout << endl;
	}

}

bool compCoverage(gaIndiv_t* A, gaIndiv_t* B) {
	
	if (A->num_branch != B->num_branch)
		return (A->num_branch > B->num_branch);
	else 
		return (A->max_index < B->max_index);
	
}
