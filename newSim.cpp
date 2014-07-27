#include "verInclude.h"
#include "incl.h"
#include "verilated.h"
#include "gaLib.h"
#include "graphLib.h"

rstIn_t gVarClass::resetInput = rstIn_t(CONST_NUM_INPUT_BITS, 'X');
int_vec IsBranchLeaf(CONST_NUM_BRANCH, 0);

int gaIndiv_t::mem_alloc_cnt = 0;
int state_t::mem_alloc_cnt = 0;
//int Vtop::mem_alloc_cnt = 0;

class Stage1_Param {

public:

	string fName; 

	int 	NUM_INDIV, INDIV_LEN;
	int 	NUM_TOP_INDIV;
	int 	NUM_GEN, POP_SIZE;
	int 	MAX_ROUNDS;

	vecIn_t			inputVec;
	int_vec			branchHit;

	state_pVec		stateList;	
	
	Stage1_Param() {
		branchHit = int_vec(CONST_NUM_BRANCH, 0);
		stateList = state_pVec();

	}

	~Stage1_Param() {
		branchHit = int_vec();
		for (int st = 0; (uint)st < stateList.size(); ++st) {
			if (stateList[st]) {
				delete stateList[st];
				stateList[st] = NULL;
			}
		}
	}

};

void Stage1_GenerateVectors(Stage1_Param*);
void readParam(Stage1_Param*);

int checkCoverage(const vecIn_t&);
void DisplayMap(stateMap_t&);
void printVec(int_vec&);
void printSet(set<int>&);
void printCnt(int_vec&);
void PrintVectorSet(const vecIn_t& inputVec, bool printFlag = false);
bool compCoverage(gaIndiv_t*, gaIndiv_t*);

int main(int argc, char* argv[]) {

#define random_seed
#ifdef random_seed
    uint seed = time(NULL);
    cout << seed << endl;
    srand(seed);
#endif

	char paramFile[100];
	sprintf(paramFile, "%s.param", benchCkt);

	if (argc >= 2) {
        if (strcmp(argv[1], "-h") == 0) {
            cout << "Input format: " << endl
                << argv[0] << " "
                << argv[1] << " [<ckt>.param]" << endl
				<< "Required files for b11/b12 : " 
				<< benchCkt << ".graph, " 
				<< benchCkt << ".bg " << endl;
        }
        else {
            sprintf(paramFile, "%s", argv[1]);
            cout << "Using parameters from " << paramFile << endl;
        }
	}		
	
   	Stage1_Param *paramObj1 = new Stage1_Param;
	paramObj1->NUM_GEN = 8;
	paramObj1->POP_SIZE = 256;
	paramObj1->INDIV_LEN = 20;
	paramObj1->NUM_INDIV = 5;
	paramObj1->NUM_TOP_INDIV = 4;

	paramObj1->fName = string(paramFile);
	readParam(paramObj1);
	
	Stage1_GenerateVectors(paramObj1);
	PrintVectorSet(paramObj1->inputVec);

	cout << "Uncovered Branches: " << endl;
	int num_branch_uncovered = 0;
	for (int ind = 0; ind < CONST_NUM_BRANCH; ++ind) {
		if(IsDefaultBranch(ind))
			paramObj1->branchHit[ind] = -5;

		if (paramObj1->branchHit[ind] == 0) {
			cout << ind << " ";
			num_branch_uncovered++;
		}
	}
	cout << endl << "% Coverage " 
		 << (float) (CONST_NUM_BRANCH - num_branch_uncovered) * 100.0f 
		 		/ (float) (CONST_NUM_BRANCH) 
		 << endl;

	cout << endl
		 << "Memory allocation details: " << endl
		 << "gaIndiv_t: " << gaIndiv_t::mem_alloc_cnt << endl
		 << "state_t: " << state_t::mem_alloc_cnt << endl;

	PrintVectorSet(paramObj1->inputVec, true);
	delete paramObj1;
	return 0;
	
}

void Stage1_GenerateVectors(Stage1_Param* paramObj) {
	
	// Parameters
	int NUM_GEN	= paramObj->NUM_GEN;
	int POP_SIZE = paramObj->POP_SIZE;
	int VEC_LEN	= paramObj->INDIV_LEN / CONST_NUM_INPUT_BITS;
	int NUM_INDIV = paramObj->NUM_INDIV;
	int TOP_INDIV = paramObj->NUM_TOP_INDIV;
	int MAX_ROUNDS = paramObj->MAX_ROUNDS;

	double WT_FIT_STATE = 0.7;
	double WT_FIT_BRANCH = 0.3;
	int WT_FIT_COV = 500;

	cout << "GA Stage 1: " << endl
		 << POP_SIZE << " individuals / " << VEC_LEN << " vectors" << endl;
	
	Vtop *cktVar = new Vtop;

	int_vec branch_counters(CONST_NUM_BRANCH, 0);

	SimMultiCycle(cktVar, 0);
	for (int br = 0; br < CONST_NUM_BRANCH; ++br)
		if (GetCoverage(cktVar, br))
			branch_counters[br] = -5;

	state_t *rstState;
	rstState = new state_t(cktVar, 0);
	rstState->pIndiv = NULL;

	state_pVec startPool;
	startPool.push_back(rstState);
	
	cout << "Reset state: " << endl;
	rstState->printState();
	cout << endl;

	gaPopulation_t stage0Pop(POP_SIZE, VEC_LEN);
	/*	Init population with one of the states in the pool 
		of start states and string of random vectors	*/
	stage0Pop.initPopulation(startPool);

	stateMap_t currStateMap, glStateMap;
	currStateMap.clear();
	int prevMaxCov = 0;

	bool gaTerminate = false;
	for (int gen = 0; !gaTerminate && (gen < NUM_GEN); ++gen) {
		
		cout << "GEN " << gen << endl << endl;
		int_vec currBranchCov(CONST_NUM_BRANCH, 0);

		bool improv_cov = false; 
		bool new_state = false;

		int maxCov = 0, avgCov = 0;
		int_vec maxInd;

		for (int ind = 0; ind < POP_SIZE; ++ind) {
			
			ResetCounters(cktVar);
			gaIndiv_t* indiv = stage0Pop.indiv_vec[ind];
			indiv->simCkt(cktVar);

			/* Add states to stateMap	*/
			for (state_pVec_iter st = indiv->state_list.begin(); 
					st != indiv->state_list.end(); ++st) {
				state_t *curr = *st;
				keyVal_t hash_val_ = curr->getHash();
				retVal_t ret = currStateMap.insert(make_pair(hash_val_, curr));
				
				curr->hit_count = 1;
			/*	If the key for the state is present 
				=>	New state was not found
				If the value for the key = NULL	
				=> 	The state was found in the previous cycle
					Therefore, set the new found state as the value for that key.
				Else
					=> The state was found in this cycle
					Therefore, just increment the count of that state		*/
				if (ret.second == false) {
					if (ret.first->second)
						(ret.first->second)->hit_count++;
					else 
						(ret.first->second) = curr;
				}
				else {
					new_state = true;
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

		/* Fittest Individuals
			i.e. individuals with maximum coverage	*/
		for (int ind = 0; ind < POP_SIZE; ++ind) {

			gaIndiv_t *indiv = stage0Pop.indiv_vec[ind];
			if (indiv->num_branch == maxCov) {
				cout << ind << " ";
				maxInd.push_back(ind);
			}

		}
		cout << endl;	

		/* Compute Fitness	*/
		for (int ind = 0; ind < POP_SIZE; ++ind) {

			gaIndiv_t *indiv = stage0Pop.indiv_vec[ind];

			/* Fitness for #branches covered */
			fitness_t fitness_cov = avgCov - indiv->num_branch;

			/* Fitness for branch coverage	*/
			fitness_t fitness_branch = 0;

			/* Fitness for states reached 	*/
			fitness_t fitness_state = 0;

			/* Combining all fitness values	*/
			indiv->fitness 
				= (fitness_cov * WT_FIT_COV) 
				+ (fitness_branch * WT_FIT_BRANCH)
				+ (fitness_state * WT_FIT_STATE);
		}	
		

		/* Terminate GA Condition:
			- gen == NUM_GEN
			- coverage[gen] <= coverage[gen-1]
			- no new states reached	*/
		
		gaTerminate = (gen == NUM_GEN - 1) | (!improv_cov) | (!new_state);

		if (!gaTerminate) {

			for (stateMap_iter st = currStateMap.begin(); 
					st != currStateMap.end(); ++st) 
				currStateMap[st->first] = NULL;

			stage0Pop.gaEvolve();
			
			prevMaxCov = maxCov;
		}
		else
			cout << "Terminating after gen " << gen << endl;

	}
		
	/* Mine for interesting individuals and states 	*/
	cout << "States reached after Round 0" << endl;

	cout << "States allocated: " << state_t::mem_alloc_cnt << endl;

	/* N.logN for sort instead of N.TOP_INDIV for finding TOP_INDIV top indivs*/
	std::sort(stage0Pop.indiv_vec.begin(), stage0Pop.indiv_vec.end(), compCoverage);
	
	/* ***** ROUND 1:MAX_ROUNDS	***** */

//	int NUM_TOP_INDIV = 4
//	vecIn_t	startVec[NUM_TOP_INDIV];
//	startPool = state_pVec(NUM_TOP_INDIV, NULL);

//	typedef map<keyVal_t, int> stateIndMap_t;
//	typedef pair<stateIndMap_t::iterator, bool> retPair_t;
//
//	stateIndMap_t stIndMap;

	/*	Need to add the final state to statePool only if its unique	
		If not unique, the indiv with greater coverage should be added
		If all coverages are same, the indiv with lesser max_index should be added
	*/

	vecIn_t startVec;
	startPool.clear();

	gaIndiv_t *indiv = stage0Pop.indiv_vec[0];

	for (int st = 0; st <= indiv->max_index; ++st) {	
		state_t *curr = indiv->state_list[st];
		keyVal_t hash_val_ = curr->getHash();
		glStateMap.insert(make_pair(hash_val_, curr));

		for (int_vec_iter bt = curr->branch_index.begin();
				bt != curr->branch_index.end(); ++bt)
			branch_counters[*bt]++;

	}
	state_t* st = indiv->state_list[indiv->max_index];
	startVec = indiv->input_vec.substr(0,CONST_NUM_INPUT_BITS*(indiv->max_index+1));
	startPool.push_back(st);
	
	cout << "After Round 0: " << endl;
	st->printState();
	cout << " --------------- " << endl;
	
	printCnt(branch_counters);
	cout << " --------------- " << endl;

	bool Rnd_terminate = false;
	int WT_NEW_BRANCH = 100;

	bool all_done = true;
	for (int br = 0; br < CONST_NUM_BRANCH; ++br) {
		if (!IsDefaultBranch(br) && (branch_counters[br] == 0))
			all_done = false;
	}
		
	for (int round = 1; !all_done && round < MAX_ROUNDS && !Rnd_terminate; ++round) {
		
		gaPopulation_t stage0Pop(POP_SIZE, VEC_LEN);
		/*	Init population with one of the states in the pool 
			of start states and string of random vectors	*/
		stage0Pop.initPopulation(startPool);

		stateMap_t currStateMap;
		//stateMap_t currStateMap = glStateMap;
		int prevMaxCov = 0;

		bool gaTerminate = false;
		for (int gen = 0; !gaTerminate && (gen < NUM_GEN); ++gen) {
			
			cout << "GEN " << gen << endl << endl;

			bool improv_cov = false; 
			bool new_state = false;

			int maxCov = 0, avgCov = 0;
			int_vec maxInd;

			for (int ind = 0; ind < POP_SIZE; ++ind) {
				
				ResetCounters(cktVar);
				gaIndiv_t* indiv = stage0Pop.indiv_vec[ind];
				indiv->simCkt(cktVar);

				/* Add states to stateMap	*/
				for (state_pVec_iter st = indiv->state_list.begin(); 
						st != indiv->state_list.end(); ++st) {
					state_t *curr = *st;
					keyVal_t hash_val_ = curr->getHash();
					retVal_t ret = currStateMap.insert(make_pair(hash_val_, curr));
					
					curr->hit_count = 1;
					/*	If the key for the state is present 
						=>	New state was not found
						If the value for the key = NULL	
							=> The state was found in the previous cycle
							Therefore, set the new found state as the value for that key.
						Else
							=> The state was found in this cycle
							Therefore, just increment the count of that state		*/
					if (ret.second == false) {
						if (ret.first->second)
							(ret.first->second)->hit_count++;
						else 
							(ret.first->second) = curr;
					}
					else {
						new_state = true;
					}
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

			/* Compute Fitness	*/
			for (int ind = 0; ind < POP_SIZE; ++ind) {

				gaIndiv_t *indiv = stage0Pop.indiv_vec[ind];

				/* Fitness for #branches covered */
				fitness_t fitness_cov = avgCov - indiv->num_branch;

				/* Fitness for branch coverage	*/
				fitness_t fitness_branch = 0;
				for (int br = 0; br < CONST_NUM_BRANCH; ++br) {
					if (indiv->branch_cov[br] && (branch_counters[br] == 0))
						fitness_branch -= WT_NEW_BRANCH;
				}

				/* Fitness for states reached 	*/
				fitness_t fitness_state = 0;

				/* Combining all fitness values	*/
				indiv->fitness 
					= (fitness_cov * WT_FIT_COV) 
					+ (fitness_branch * WT_FIT_BRANCH)
					+ (fitness_state * WT_FIT_STATE);
			}	
			

			/* Terminate GA Condition:
				- gen == NUM_GEN
				- coverage[gen] <= coverage[gen-1]
				- no new states reached	*/
			
			gaTerminate = (gen == NUM_GEN - 1) | (!improv_cov) | (!new_state);

			if (!gaTerminate) {

				for (stateMap_iter st = currStateMap.begin(); 
						st != currStateMap.end(); ++st) 
					currStateMap[st->first] = NULL;

				stage0Pop.gaEvolve();
				
				prevMaxCov = maxCov;
			}
			else {
				cout << "Terminating after gen " << gen << endl;
				if (prevMaxCov < maxCov)
					prevMaxCov = maxCov;
			}

		}
			
		/* N.logN for sort instead of N.TOP_INDIV for finding TOP_INDIV top indivs*/
		std::sort(stage0Pop.indiv_vec.begin(), stage0Pop.indiv_vec.end(), compCoverage);
	
		gaIndiv_t *indiv = stage0Pop.indiv_vec[0];
		cout << "Fittest indiv after round " << round << endl;
		indiv->printIndiv(1);

		int_vec curr_branch_cnt (CONST_NUM_BRANCH, 0);
		bool new_start_state = false;
		for (int st = 0; st <= indiv->max_index; ++st) {	
			state_t *curr = indiv->state_list[st];
			keyVal_t hash_val_ = curr->getHash();
			retVal_t ret = glStateMap.insert(make_pair(hash_val_, curr));

			if (ret.second == true) {
				cout << "X";
				new_start_state = true;
//				indiv->state_list[st] = NULL;
			}
			else {
				cout << "-";
//				state_t* tmp = ret.first->second;
//				if (tmp)
//					tmp->hit_count++;
//				else
//					glStateMap[ret.first->first] = curr;
			}
			for (int_vec_iter bt = curr->branch_index.begin();
					bt != curr->branch_index.end(); ++bt)
				curr_branch_cnt[*bt]++;

		}
		
		cout << endl;

		bool new_branch_ = false;
		for (int br = 0; br < CONST_NUM_BRANCH; ++br) {
			if ((curr_branch_cnt[br]) && (branch_counters[br] == 0))
				new_branch_ = true;
			branch_counters[br] += curr_branch_cnt[br];
		}
	
		bool all_done_2 = true;
		for (int br = 0; br < CONST_NUM_BRANCH; ++br) {
			if (!IsDefaultBranch(br) && (branch_counters[br] == 0)) {
				all_done_2 = false;
				break;
			}
		}

		Rnd_terminate = !(new_start_state || new_branch_) || (round == MAX_ROUNDS-1) || (all_done_2);
		
		startVec += indiv->input_vec.substr(0,CONST_NUM_INPUT_BITS*(indiv->max_index+1));

		for (state_pVec_iter st = startPool.begin(); st != startPool.end(); ++st)
			if (*st == NULL) {
				delete *st;
				*st = NULL;
			}

		startPool.clear();
		state_t* st = indiv->state_list[indiv->max_index];
		indiv->state_list[indiv->max_index] = NULL;
		
		startPool.push_back(st);

		cout << "After Round " << round << " : " << endl;
		st->printState();
		cout << " --------------- " << endl;
		
		printCnt(branch_counters);
		cout << " --------------- " << endl;

		if (Rnd_terminate)
			cout << "Exiting after round " << round << endl;
	}		

//	if (pst->getHash().compare(st->getHash()) == 0)
	rstState->setCktState(cktVar);
	SimMultiCycle(cktVar, startVec);
	int_vec branch_cov;
	GetBranchCounters(cktVar, branch_cov);
	
	PrintVectorSet(startVec);
	cout << "Final coverage: " << endl;
	printCnt(branch_cov);

	exit(0);

	paramObj->inputVec = startVec;
	paramObj->branchHit = branch_cov;
	paramObj->stateList = state_pVec(startVec.size(), NULL);

	cout << "Branches covered after Stage 1: " << endl;
	printCnt(paramObj->branchHit);

	delete rstState;
	delete cktVar;
}


int checkCoverage(const vecIn_t& inputVec) {
	
	Vtop* top = new Vtop;

	int numVec = inputVec.length()/CONST_NUM_INPUT_BITS;
	int_vec brCoverage(CONST_NUM_BRANCH, 0);
	set<int> brSet;
	
	// Add reset branches to the set 
	int_vec resetBranch;
	GetResetBranch(top, resetBranch);
	for (int_vec_iter it = resetBranch.begin();
			it != resetBranch.end(); ++it) {
		brSet.insert(*it);
	}

	int index;

	for (int i = 0; i < numVec; ++i) {
		string inputStr = inputVec.substr(i*CONST_NUM_INPUT_BITS, CONST_NUM_INPUT_BITS);
		cout << inputStr << endl;
		SimOneCycle(top, inputStr);
//		printCktState(top);

		int_vec currBranch;
		GetCoverage(top, currBranch);
		for (int_vec_iter it = currBranch.begin();
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

void readParam(Stage1_Param* paramObj) {

	ifstream paramIn;
	paramIn.open((paramObj->fName).c_str(), ios::in);

	if (!paramIn) {
		cout << "Unable to read " << paramObj->fName << endl
			 << "Using default values. " << endl
			 << "NUM_GEN_0 	: 4 generations" << endl
			 << "POP_SIZE_0	: 256 individuals" << endl
			 << "INDIV_LEN_0: 20 bits" << endl
			 << "MAX_ROUNDS_0: 10 rounds" << endl;

		paramObj->NUM_GEN = 4;
		paramObj->POP_SIZE = 256;
		paramObj->INDIV_LEN = 20;
		paramObj->NUM_INDIV = 2;
		paramObj->MAX_ROUNDS = 10;
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
			else if (curr.find("MAX_ROUNDS_0") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "MAX_ROUNDS = " << val << endl;
					paramObj->MAX_ROUNDS = val;
				}
				else 
					paramObj->MAX_ROUNDS = 10;
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

void printVec(int_vec& vec_) {
	for (int_vec_iter vt = vec_.begin();
			vt != vec_.end(); ++vt)
		cout << *vt << " ";
}

void printSet(set<int>& vec_) {
	for (set<int>::iterator vt = vec_.begin();
			vt != vec_.end(); ++vt)
		cout << *vt << " ";
}

void printCnt(int_vec& vec_) {
	int num_branch = 0;
	for (uint v = 0; v < vec_.size(); ++v) {
		cout << v << "(" << vec_[v] << ") ";
		if (vec_[v] == 0)
			num_branch++;
	}
	cout << endl << "#Branch " << CONST_NUM_BRANCH - num_branch << endl;
}

void PrintVectorSet(const vecIn_t& inputVec, bool printFlag ) {

	ofstream vecOut;
	char fName[100];

	if (printFlag) {
		sprintf(fName, "%s_%d.vec", benchCkt, (int)time(NULL) % 10000);
		
		cout << "Writing vectors into file -> " << fName << endl; 
		vecOut.open(fName, ios::out);
		
		vecOut << CONST_NUM_INPUT_BITS << endl;
	}
		
	cout << endl << "# Vectors: " << inputVec.length()/CONST_NUM_INPUT_BITS << endl;

	for (uint i = 0; i < inputVec.length(); i++) {
		cout << inputVec[i];
		if(printFlag)
			vecOut << inputVec[i];
		if ((i+1) % CONST_NUM_INPUT_BITS == 0) {
			cout << endl;
			if(printFlag)
				vecOut << endl;
		}
	}
	
	if(printFlag) {
		vecOut << "END" << endl;
		vecOut.close();
	}
}

bool compCoverage(gaIndiv_t* A, gaIndiv_t* B) {
	
	if (A->num_branch != B->num_branch)
		return (A->num_branch > B->num_branch);
	else 
		return (A->max_index < B->max_index);
	
}
