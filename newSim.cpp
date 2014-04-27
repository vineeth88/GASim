#include "verInclude.h"
#include "incl.h"
#include "verilated.h"
#include "gaLib.h"

rstIn_t gVarClass::resetInput = rstIn_t(CONST_NUM_INPUT_BITS, 'X');
vector<int> leafBranch(CONST_NUM_BRANCH, 0);

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

class Stage0_Param {

public:
	int 	NUM_INDIV, VEC_LENGTH;
	int 	NUM_TOP_INDIV;
	int 	NUM_GEN, POP_SIZE;

	vecIn_t			inputVec;
	state_t			*startState, *endState;
	vector<int>		branchHit;
	
	Stage0_Param() {
		startState = endState = NULL;
		branchHit = vector<int>(CONST_NUM_BRANCH, 0);
	}
};

void ga_Gen0_generateVectors(ga_Gen0_Param*);
void Stage0_GenerateVectors(Stage0_Param*);

int checkCoverage(const vecIn_t&);
void DisplayMap(stateMap_t&);
void printVec(vector<int>&);
void printCnt(vector<int>&);
void PrintVectorSet(const vecIn_t& inputVec);
bool compCoverage(gaIndiv_t*, gaIndiv_t*);

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

   	Stage0_Param *paramObj = new Stage0_Param;
	paramObj->NUM_GEN = 8;
	paramObj->POP_SIZE = 256;
	paramObj->VEC_LENGTH = 20/CONST_NUM_INPUT_BITS;
	paramObj->NUM_INDIV = 5;
	paramObj->NUM_TOP_INDIV = 4;

	Stage0_GenerateVectors(paramObj);
	checkCoverage(paramObj->inputVec);
	PrintVectorSet(paramObj->inputVec);
	
	delete paramObj;
}

void Stage0_GenerateVectors(Stage0_Param* paramObj) {
	
	// Parameters
	int NUM_GEN	= paramObj->NUM_GEN;
	int POP_SIZE = paramObj->POP_SIZE;
	int VEC_LEN	= paramObj->VEC_LENGTH;
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
	
#ifdef __b01
	int leafIndex[16] = { 1, 2, 4, 5, 7, 8, 10, 11, 13, 14, 16, 17, 19, 20, 22, 23}; 

	for (int i = 0; i < 15; ++i)
		leafBranch[leafIndex[i]] = 1;

#endif

#ifdef __b06
	int leafIndex[16] = { 1, 2, 3, 4, 5, 7, 8, 10, 11, 13, 14, 16, 17, 19, 20, 22}; 

	for (int i = 0; i < 15; ++i)
		leafBranch[leafIndex[i]] = 1;

#endif

#ifdef __b12
	int leafIndex[18] = { 1, 2, 4, 5, 6, 8, 9, 10, 12, 15, 16, 19, 21, 23, 25, 27, 29, 30 }; 

	for (int i = 0; i < 17; ++i)
		leafBranch[leafIndex[i]] = 1;

#endif

#ifdef __b12
	int leafIndex[68] = { 1, 2, 4, 5, 7, 8, 10, 11, 13, 14, 16, 17, 19, 21, 26, 29, 30, 31, 32, 	// 19
						33, 34, 35, 36, 37, 38, 40, 41, 43, 45, 46, 47, 48, 49, 51, 52, 54, 55,	// 18
						57, 58, 60, 63, 64, 65, 67, 68, 69, 71, 73, 74, 75, 77, 78, 81, 82, 83,	// 18
						84, 86, 87, 89, 91, 92, 94, 95, 97, 98, 101, 101, 103}; 				// 13

	for (int i = 0; i < 67; ++i)
		leafBranch[leafIndex[i]] = 1;

#endif

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
		for (gaIndiv_pVec_iter indiv = stage0Pop.indiv_vec.begin();
				indiv != stage0Pop.indiv_vec.end(); ++indiv) {
			(*indiv)->printIndiv(1);
			cout << (*indiv)->num_branch << " branches with "
				 << (*indiv)->max_index + 1 << " vectors" << endl;
		}
		
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
	return index;
}



void DisplayMap(stateMap_t& stateMap) {
	for (stateMap_iter it = stateMap.begin(); it!= stateMap.end(); ++it) {

		cout << it->first << "\t";
		if (it->second) {
			cout << it->second->hit_count << endl;
			(it->second)->printState();
		}
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

	cout << endl << "# Vectors: " << inputVec.size()/CONST_NUM_INPUT_BITS << endl;
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
