#include "verInclude.h"
#include "incl.h"
#include "verilated.h"
#include "gaLib.h"
#include "graphLib.h"

rstIn_t gVarClass::resetInput = rstIn_t(CONST_NUM_INPUT_BITS, 'X');
int_vec IsBranchLeaf(CONST_NUM_BRANCH, 0);

vector<vecIn_t> BranchInputVec(CONST_NUM_BRANCH, string(""));
vector<string> BranchStateVal(CONST_NUM_BRANCH, string(""));
int_vec BranchNumTries(CONST_NUM_BRANCH, 0);

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

class Stage2_Param : public Stage1_Param {
	
	public:

	int MAX_ROUNDS;
	int MAX_TRIES;

	state_t *	startState;
	Vtop *		cktVar;

	set<int> 	favorSet;
	set<int>	unCovered;

	int_vec		branch_index;
	int_vec 	branchHit;
	int_vec 	lastBranchHit;

	brGraph_t*	branchGraph;
	int 		curr_val, end_val;
	int			tar_edge, tar_node;

	Stage2_Param() {

		MAX_ROUNDS = 20;
		cktVar = NULL;
		branchHit = int_vec(CONST_NUM_BRANCH, 0);
		lastBranchHit = int_vec(CONST_NUM_BRANCH, 0);
		favorSet.clear();
		unCovered.clear();
	}

	Stage2_Param(Stage1_Param* obj) {	
		NUM_GEN 	= obj->NUM_GEN;
		INDIV_LEN 	= obj->INDIV_LEN;
		POP_SIZE 	= obj->POP_SIZE;
		
		fName 		= obj->fName;

		branchHit 	= obj->branchHit;
		branch_index = (obj->stateList.back())->branch_index;

		cktVar = NULL;
		MAX_ROUNDS = 20;

		favorSet = set<int>();
		unCovered = set<int>();
		
	}

//	~Stage2_Param() {
//		if(cktVar) {
//			delete cktVar;
//			cktVar = NULL;
//		}
//		if (branchGraph) {
//			delete branchGraph;
//			branchGraph = NULL;
//		}
//	}

};

void Stage1_GenerateVectors(Stage1_Param*);
void readParam(Stage1_Param*);
void readParam(Stage2_Param*);

bool readTopNodes(covGraph_t&, brGraph_t&);
void readBranchGraph(brGraph_t&);

void Stage2_GenerateVectors(Stage2_Param*);
int getPathBFS(brGraph_t*, int, int, string&);
int AddVector2Branch(int, vecIn_t);
int AddState2Branch(int, state_t*);

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

	if (num_branch_uncovered == 0) {
		PrintVectorSet(paramObj1->inputVec, true);
		delete paramObj1;
		return 0;
	}
	
	#if defined(__b10)
		PrintVectorSet(paramObj1->inputVec, true);
		delete paramObj1;
		return 0;
	#endif
	
	cout << endl
		 << "Memory allocation details: " << endl
		 << "gaIndiv_t: " << gaIndiv_t::mem_alloc_cnt << endl
		 << "state_t: " << state_t::mem_alloc_cnt << endl;

//	delete paramObj1;	

	covGraph_t covGraph;
	brGraph_t branchGraph;
	sprintf(covGraph.fName, "%s.graph", benchCkt);
	readGraph(covGraph);
	bool brFlag = readTopNodes(covGraph, branchGraph);
	
	if (!brFlag) {
		PrintVectorSet(paramObj1->inputVec, true);
		delete paramObj1;
		return 0;
	}		
	
	vecIn_t TestVectorSet = paramObj1->inputVec;

	cout << "Stage 2 " << endl << endl;
	Stage2_Param *paramObj2 = new Stage2_Param(paramObj1);
	paramObj2->inputVec = paramObj1->inputVec;
	paramObj2->branchGraph = &branchGraph;

	paramObj2->startState = paramObj1->stateList.back();
	paramObj2->stateList = paramObj1->stateList;
	paramObj1->stateList.clear();
	
	paramObj2->cktVar = new Vtop;
	SimMultiCycle(paramObj2->cktVar, paramObj1->inputVec);

	readParam(paramObj2);
	
	cout << "Last state reached at the end of Stage 1" << endl;
	paramObj2->startState->printState();
	
	int_vec &branch_index = paramObj2->branch_index;
	int_vec &branchHit = paramObj2->branchHit;
	set<int> &favorSet = paramObj2->favorSet;
//	set<int> &unCovered = paramObj2->unCovered;

	int_vec unCovered;
	for (int br = 0; br < CONST_NUM_BRANCH; ++br) {
		if(IsDefaultBranch(br))
			branchHit[br] = -5;
		else if ((branchHit[br] == 0) && (IsBranchLeaf[br])) {
			bEdge_t* edge = branchGraph.getEdge(br);
			if (edge) 
				unCovered.push_back(br);
		}
	}

	int MAX_ROUNDS = paramObj2->MAX_ROUNDS;
	BranchNumTries = int_vec(CONST_NUM_BRANCH, 0);
	for (int num_round = 0; num_round < MAX_ROUNDS; ++num_round) {
	
	cout << endl 
		 << "ROUND : " << num_round << " / " << MAX_ROUNDS
		 << endl << endl;
	favorSet.clear();
	
	/*	For hitting branch 86, 
		1. Target and hit branch 67
		2. Target and hit branch 68 next.
		3. Target and hit branch 86.	
		
		1)	Either this can be read from a file.
		2)	You cant target the same branch more than 10 times in a row 
			unless its the only branch left, or if the paths of the other 
			branches go through the targeted branch		*/

	if (unCovered.size() == 0)
		break;

	/*	edgeVec = The list of edges traversed in the last cycle	*/
	int_vec edgeVec;
	for (int_vec_iter br = branch_index.begin(); br != branch_index.end(); ++br) {
		if (IsBranchLeaf[*br])	{
			bEdge_t* curr_edge = branchGraph.getEdge(*br);
			if (curr_edge) 
				edgeVec.push_back(*br);
		}
	}
	
	/* 	curr_val = Selected edge with endpoints start_val -> end_val */
	int curr_val = edgeVec.back();
	bEdge_t* curr_edge = branchGraph.getEdge(curr_val);
	int start_val = curr_edge->startTop;
	int end_val = curr_edge->endTop;

	bNode_t* curr_node = branchGraph.getNode(end_val);
	assert(curr_node);
	
	string target_path = "Unreachable";
	int target_lvl = (2 << 20);
	int target_node = -1;
	int target_tries = paramObj2->MAX_TRIES;

	/*	- Find path from curr_node to all the start_nodes of the uncovered edges
		- Find the uncovered edge with the shortest path
		- Set the topNode(start_node) as the target_node */
	#ifdef S2_DBG_PRINT_EXTRA
	cout << "Number of tries (of " << paramObj2->MAX_TRIES << ")" << endl;
	printCnt(BranchNumTries);
	cout << endl;
	#endif

	for (int_vec_iter it = unCovered.begin(); it != unCovered.end(); ++it) {
		string path;
		int lvl = getPathBFS(&branchGraph, end_val, *it, path);
		#ifdef S2_DBG_PRINT
		cout << end_val << "-> " 
			 << *it << ": " 
			 << path << endl;
		#endif
		if ((target_lvl > lvl) && (path.compare("Unreachable"))) {
			target_lvl = lvl;
			target_node = *it;
			target_path = path;
		}
	}
	
	cout << "Target	: " << target_node << endl
		 << "Level 	: " << target_lvl << endl
		 << "Path 	: " << target_path << endl;
	
	paramObj2->curr_val = curr_val;
	paramObj2->end_val = end_val;
	
	if (target_path.compare("Unreachable")) {
		BranchNumTries[target_node]++;
		int iter_val = end_val;
		bNode_t* iter_node = branchGraph.getNode(iter_val);
		assert(iter_node);
		for (int st = 0; (uint) st < target_path.length(); ++st) {
			int brInd = target_path[st] - 48;
			int tar_edge = iter_node->outEdges[brInd];

			cout << " Path[" << st << "/" << target_path.length() << "]: " << tar_edge << endl;

			int_vec self_loop_br;
			for (int br = 0 ; br < iter_node->outNodes.size(); ++br) {
				if (iter_node->outNodes[br] == iter_val) {
					self_loop_br.push_back(iter_node->outEdges[br]);
				}
			}

			if (self_loop_br.size()) {
				cout << "Self-loop: ";
				printVec(self_loop_br);
				cout << endl;
			}

			/* Run stage 2	*/
			paramObj2->tar_edge = tar_edge;
			paramObj2->tar_node = iter_node->outNodes[brInd];

			Stage2_GenerateVectors(paramObj2);
			int branch_hit = 20;
			for (int br = 0; br < iter_node->outEdges.size(); ++br) {
				int tmp_edge = iter_node->outEdges[br];
				if ((paramObj2->lastBranchHit[tmp_edge]) && 
					(iter_node->outNodes[br] != iter_val))
						branch_hit = 0;
			}
			if (branch_hit) {
				if(self_loop_br.size()) {
					while (branch_hit) {
						/*	Run stage 2	*/
						Stage2_GenerateVectors(paramObj2);
						if (paramObj2->lastBranchHit[tar_edge]) {
							branch_hit = 0;
							break;
						}
						for (int br = 0; br < iter_node->outEdges.size(); ++br) {
							int tmp_edge = iter_node->outEdges[br];
							if ((paramObj2->lastBranchHit[tmp_edge]) && 
								(iter_node->outNodes[br] != iter_val))
									branch_hit = 1;
						}
						branch_hit--;
						cout << "In branch_hit : " << branch_hit << endl;
					}
				}
				else {
					cout << "Currently Unreachable" << endl;
					break;
				}
			}

			iter_val = iter_node->outNodes[brInd];
			iter_node = branchGraph.getNode(iter_val);
			assert(iter_node);
		}

		cout << endl << "END OF PATH" << endl;

		for (int br = 0; br < unCovered.size(); ++br) {
			if (unCovered[br] == target_node)
				unCovered[br] = -100;
			else if (branchHit[unCovered[br]])
				unCovered[br] = -100;
		}
			
		int_vec newVec;
		for (int_vec_iter br = unCovered.begin(); br != unCovered.end(); ++br) {
			if (*br != -100)
				newVec.push_back(*br);
		}
		if (branchHit[target_node] == 0)
			newVec.push_back(target_node);
		
		unCovered.clear();
		unCovered = newVec;
		newVec.clear();

		cout << "unCovered: " << endl;
		printVec(unCovered);
		cout << endl;
	}
	else {
		cout << endl
			 << " * * * * * * * * * * * * * " << endl
			 << "No reachable path. Simulating vector for next state." << endl;

		stateMap_t nxtStateMap;
		map<keyVal_t, vecIn_t> vecMap;
		gaPopulation_t nxtStatePop(paramObj2->POP_SIZE, 1);
		
		state_pVec initPool;
		initPool.push_back(paramObj2->stateList.back());
		
		cout << "Start state: " << endl;
		initPool[0]->printState();
		cout << endl;

		nxtStatePop.initPopulation(initPool);

		Vtop *cktVar = paramObj2->cktVar;
		for (int ind = 0; ind < paramObj2->POP_SIZE; ++ind) {
			
			ResetCounters(cktVar);
			gaIndiv_t* indiv = nxtStatePop.indiv_vec[ind];
			indiv->simCkt(cktVar);

			int vt = 0;
			for (state_pVec_iter st = indiv->state_list.begin(); 
					st != indiv->state_list.end(); ++st, ++vt) {
				keyVal_t hash = (*st)->getHash();
				retVal_t ret = nxtStateMap.insert(make_pair(hash, *st));
				if (ret.second == false) {
					(ret.first->second)->hit_count++;
				}
				vecMap.insert(make_pair(hash, indiv->input_vec)); 
			}
		}

		vector<vecIn_t> nxtVec;
		vector<int> nxtPaths;
		for (stateMap_iter st = nxtStateMap.begin(); st != nxtStateMap.end(); ++st) {
			cout << "State: ";
			st->second->printState();
			cout << "Hits : " << st->second->hit_count << endl;
			cout << "Vector: " << vecMap[st->first] << endl;
			cout << endl;

			int_vec edgeVec;
			edgeVec.clear();
			state_t *curr = st->second;
			for (int_vec_iter br = curr->branch_index.begin(); 
					br != curr->branch_index.end(); ++br) {
				if (IsBranchLeaf[*br])	{
					bEdge_t* curr_edge = branchGraph.getEdge(*br);
					if (curr_edge) 
						edgeVec.push_back(*br);
				}
			}

			int curr_val = edgeVec.back();
			bEdge_t* curr_edge = branchGraph.getEdge(curr_val);
			int start_val = curr_edge->startTop;
			int end_val = curr_edge->endTop;

			bNode_t* curr_node = branchGraph.getNode(end_val);
			assert(curr_node);
			
			string target_path = "Unreachable";
			int target_lvl = (2 << 20);
			int target_node = -1;

			/*	- Find path from curr_node to all the start_nodes of the uncovered edges
				- Find the uncovered edge with the shortest path
				- Set the topNode(start_node) as the target_node */
			int num_paths;
			for (int_vec_iter it = unCovered.begin(); it != unCovered.end(); ++it) {
				string path;
				int lvl = getPathBFS(&branchGraph, end_val, *it, path);
				cout << *it << ": " << path << endl;
				if (path.compare("Unreachable")) {
					num_paths++;
					if (target_lvl > lvl) {
						target_lvl = lvl;
						target_node = *it;
						target_path = path;
					}
				}
			}

			if (target_path.compare("Unreachable")) {
				nxtVec.push_back(vecMap[st->first]);
				nxtPaths.push_back(num_paths);
			}
		}

		int max_num_paths = -1, max_path_index = -1;
		if (nxtPaths.size() == 0) {
			//PrintVectorSet(paramObj2->inputVec, true);
			//getDominator(covGraph, unCovered);
			assert(nxtPaths.size());
		}

		for (int np = 0; np < nxtPaths.size(); ++np) {
			if (nxtPaths[np] > max_num_paths) {
				max_num_paths = nxtPaths[np];
				max_path_index = np;
			}
		}
		
		cout << endl;

		(paramObj2->stateList.back())->setCktState(cktVar);
		SimOneCycle(cktVar, nxtVec[max_path_index]);
		state_t *tmpState = new state_t(cktVar, paramObj2->stateList.size());
		(paramObj2->stateList).push_back(tmpState);
		paramObj2->inputVec += nxtVec[max_path_index];

		cout << "Adding vector: " << nxtVec[max_path_index] << endl
			 << "State: " << endl;
		tmpState->printState();
		cout << endl; 

		paramObj2->lastBranchHit = int_vec(CONST_NUM_BRANCH, 0);
		for (int_vec_iter it = tmpState->branch_index.begin();
				it != tmpState->branch_index.end(); ++it)
			paramObj2->lastBranchHit[*it]++;

		paramObj2->branch_index = tmpState->branch_index;
		for (int ind = 0; ind < CONST_NUM_BRANCH; ++ind)
			paramObj2->branchHit[ind] += paramObj2->lastBranchHit[ind];
		printCnt(paramObj2->lastBranchHit);
		printCnt(paramObj2->branchHit);

		for( set<int>::iterator br = favorSet.begin(); br != favorSet.end(); ++br) {
			if (paramObj2->lastBranchHit[*br] != 0)
				paramObj2->favorSet.erase(*br);
		}
		cout << "Elements in favorSet after = ";
		printSet(paramObj2->favorSet);
		cout << endl;
				
//		getchar();
		cout << " * * * * * * * * * * * * * * * * * * * * " << endl << endl;
	}
	}

	#ifdef S2_DBG_PRINT
	int i = 0;
	for (state_pVec_iter st = paramObj2->stateList.begin();
			st != paramObj2->stateList.end(); ++st, ++i) {
		cout << paramObj2->inputVec.substr(i*CONST_NUM_INPUT_BITS, CONST_NUM_INPUT_BITS);
		cout << endl;
		(*st)->printState(1);
		cout << endl << endl;
	}
	#endif

	PrintVectorSet(paramObj2->inputVec, true);
	
	cout << endl
		 << "Memory allocation details: " << endl
		 << "gaIndiv_t: " << gaIndiv_t::mem_alloc_cnt << endl
		 << "state_t: " << state_t::mem_alloc_cnt << endl;

	exit(0);
	delete paramObj2;

	return 0;
}

void Stage2_GenerateVectors(Stage2_Param* paramObj) {

	cout << endl 
		 << "GA Stage 2: " << endl
		 << "Previous Coverage: " << endl;
	printCnt(paramObj->branchHit);
	cout << endl;
	
	cout << "Current params: " << endl
		 << "Edge: " << paramObj->curr_val << endl
		 << "Node: " << paramObj->end_val << endl;

	int NUM_GEN	= paramObj->NUM_GEN;
	int POP_SIZE = paramObj->POP_SIZE;
	int VEC_LEN = paramObj->INDIV_LEN/CONST_NUM_INPUT_BITS;

	brGraph_t *branchGraph = paramObj->branchGraph;
	set<int> &favorSet = paramObj->favorSet;
	int_vec &branchHit = paramObj->branchHit;
	int_vec &lastBranchHit = paramObj->lastBranchHit;
	Vtop *cktVar = paramObj->cktVar;

	lastBranchHit = int_vec(CONST_NUM_BRANCH, 0);

	state_pVec startPool;
	state_t* start_state = paramObj->stateList.back();
	start_state->printState(1);
	startPool.push_back(start_state);

	start_state->setCktState(cktVar);
	printCktState(cktVar);
	ResetCounters(cktVar);

	#ifdef S2_DBG_PRINT
	for (int br = 0; br < CONST_NUM_BRANCH; ++br) {
		cout << "Branch " << br << ":\t " << BranchInputVec[br] 
			 //<< "\t " << BranchNumTries[br]
			 << endl << BranchStateVal[br] << endl;
	}
	cout << endl;
	#endif

	cout << "Elements in favorSet before = { ";
	printSet(paramObj->favorSet);
	cout << " } " <<  endl;

	/*	Run GA	*/
	gaPopulation_t stage2Pop(POP_SIZE, VEC_LEN);
	stage2Pop.initPopulation(startPool);
	
	int WT_TAR_EDGE = -200;
	int WT_TAR_NODE = -100;
	int WT_SELF_EDGE = -50;
	int WT_OUT_EDGE = 100;
	int WT_UNIMPT	= 200;

	int tar_edge = paramObj->tar_edge;
	int tar_node = paramObj->tar_node;

	bEdge_t* tar_edge_obj = branchGraph->getEdge(tar_edge);
	assert(tar_edge_obj);
	if (tar_edge_obj->endTop != tar_node) {
		cout << "Edge " << tar_edge 
			 << " ends in "	<< tar_edge_obj->endTop
			 << " not in " 	<< tar_node << endl;
	}
	assert(tar_edge_obj->endTop == tar_node);

	bNode_t* tar_node_obj = branchGraph->getNode(tar_node);
	assert(tar_edge_obj);

	bool gaTerminate = false;
	fitness_t prev_gen_fitness = (2 << 24), curr_gen_fitness = (2 << 25);

	for (int gen = 0; !gaTerminate && (gen < NUM_GEN); ++gen) {

		cout << "GEN " << gen << endl << endl;
		int_vec currBranchCov(CONST_NUM_BRANCH, 0);

		fitness_t curr_gen_max_fit = - (2 << 25), curr_gen_min_fit = (2 << 25);
		for (int ind = 0; ind < POP_SIZE; ++ind) {
			
			ResetCounters(cktVar);
			gaIndiv_t* indiv = stage2Pop.indiv_vec[ind];
			#if defined(__b12)
			for (int x = 0; x < indiv->input_vec.length(); x += 5) 
				indiv->input_vec[x] = '0';
			#endif
			indiv->simCkt(cktVar);

			int_vec state_fit_vec;

//			cout << "States: " << endl;
			int branch_val = -1;
			bool exit_state = false;
			int sv = 0;
			for (state_pVec_iter st = indiv->state_list.begin();
					st != indiv->state_list.end(); ++st, ++sv) {
				
				int state_fit = 0;
				vecIn_t inp = indiv->input_vec.substr(sv*CONST_NUM_INPUT_BITS, CONST_NUM_INPUT_BITS);
				for (int_vec_iter br = (*st)->branch_index.begin(); 
						br != (*st)->branch_index.end() && !exit_state; ++br) {

					AddVector2Branch(*br, inp);

					if (IsBranchLeaf[*br] == 0)
						continue;

					bEdge_t* curr_edge_obj = branchGraph->getEdge(*br);
					if (curr_edge_obj == NULL)
						continue;

					if (*br == tar_edge) {
						state_fit += WT_TAR_EDGE;
						exit_state = true;
						branch_val = *br;
						gaTerminate = true;
					}
					else if (curr_edge_obj->endTop == tar_edge_obj->endTop) {
						state_fit += WT_TAR_NODE;
						branch_val = *br;
						exit_state = true;
					}
					else {
						int curr_node = curr_edge_obj->startTop;
						bNode_t* curr_node_obj = branchGraph->getNode(curr_node);

						bool unimpt_branch = true;
						for (int bt = 0; (uint) bt < curr_node_obj->outEdges.size(); ++bt) { 
							if (curr_node_obj->outEdges[bt] == *br) {
								unimpt_branch = false;
								if (curr_node_obj->outNodes[bt] == curr_node) {
									state_fit += WT_SELF_EDGE;
									exit_state = false;
								}
								else {
									state_fit += WT_OUT_EDGE;
									exit_state = true;
									branch_val = *br;
								}
							}
						}
						
						if(unimpt_branch)
							state_fit += WT_UNIMPT;
					}

				}

				state_fit_vec.push_back(state_fit);
				if (exit_state) {
					#ifdef S2_DBG_PRINT
					cout << branch_val << " reached in " << state_fit_vec.size() << " vectors" << endl;
					#endif
					break;
				}

			}

			#ifdef S2_DBG_PRINT
			cout << "Fitness: ";
			printVec(state_fit_vec);
			cout << endl;
			#endif

			indiv->fitness = 0;			
			int max_ind = -1, max_fit = (2 << 20);
			for (int it = 0; (uint)it < state_fit_vec.size(); ++it) {
				if (max_fit >= state_fit_vec[it]) {
					max_ind = it;
					max_fit = state_fit_vec[it];
				}
				indiv->fitness += state_fit_vec[it];
			}
//			indiv->fitness /= indiv->state_list.size();
			if (max_ind != -1)
				indiv->max_index = max_ind;
//			indiv->max_index = state_fit_vec.size() - 1;

			if (curr_gen_fitness >= indiv->fitness)
				curr_gen_fitness = indiv->fitness;
			
			if (curr_gen_min_fit >= indiv->fitness)
				curr_gen_min_fit = indiv->fitness;
			
			if (curr_gen_max_fit <= indiv->fitness)
				curr_gen_max_fit = indiv->fitness;
			
			#ifdef S2_DBG_PRINT
			cout << "Fitness(" << indiv->index << "): " 
				 << indiv->fitness << " : " 
				 << indiv->max_index + 1 << " vectors " 
				 << endl << endl;
		 	#endif
		}	
		
		cout << "Max: " << curr_gen_max_fit
			 << " Min: " << curr_gen_min_fit << endl;
		if (curr_gen_max_fit == curr_gen_min_fit)
			gaTerminate = true;

		gaTerminate = gaTerminate || (gen == NUM_GEN-1) || (prev_gen_fitness < curr_gen_fitness);
		if (!gaTerminate) {
			stage2Pop.gaEvolve();
			cout << "Evolved" << endl;
		}

		prev_gen_fitness = curr_gen_fitness;
		curr_gen_fitness = (2 << 25);
	}
	
	cout << "After GA : " << endl;
	std::sort(stage2Pop.indiv_vec.begin(), stage2Pop.indiv_vec.end(), compFitness);
	gaIndiv_t *indiv = stage2Pop.indiv_vec[0];
	indiv->printIndiv(1);
	cout << endl << indiv->max_index + 1 << " vectors copied" << endl;
	
	paramObj->inputVec += indiv->input_vec.substr(0, CONST_NUM_INPUT_BITS * (indiv->max_index + 1));
	for (int ind = 0; ind <= indiv->max_index; ++ind) {
		state_t *st = indiv->state_list[ind];
		if (st == NULL) {
			cout << "GG" << endl;
			break;
		}
		for (int_vec_iter it = st->branch_index.begin();
				it != st->branch_index.end(); ++it)
			//paramObj->branchHit[*it]++;
			lastBranchHit[*it]++;

		paramObj->stateList.push_back(indiv->state_list[ind]);
		indiv->state_list[ind] = NULL;
	}

	paramObj->branch_index = paramObj->stateList.back()->branch_index;
	for (int ind = 0; ind < CONST_NUM_BRANCH; ++ind)
		branchHit[ind] += lastBranchHit[ind];
	#ifdef S2_DBG_PRINT
	#ifdef S2_DBG_PRINT_EXTRA
	printCnt(lastBranchHit);
	#endif
	printCnt(paramObj->branchHit);
	#endif

	for( set<int>::iterator br = favorSet.begin(); br != favorSet.end(); ++br) {
		if (lastBranchHit[*br] != 0)
			favorSet.erase(*br);
	}
	#ifdef S2_DBG_PRINT
	cout << "Elements in favorSet after = ";
	printSet(paramObj->favorSet);
	cout << endl;
	#endif

}

int getPathBFS(brGraph_t* brGraph, int start, int target, string& path) {
	
	#ifdef S2_DBG_PRINT
	cout << "Finding path from " << start << " -> " << target << endl;
	#endif
	map<int, string> label;
	map<int, int> level;
	
	typedef pair< map<int, string>::iterator, bool> retStr;
	
	int max_level = -1;

	vector<int> bfsQueue;
	bfsQueue.push_back(start);
	level.insert(make_pair(start, 0));
	label.insert(make_pair(start, ""));

	const int THRESH_LEVEL = 20;
	const int MAX_QUEUE_SIZE = 1000;
	for (int qInd = 0; (uint) qInd < bfsQueue.size(); ++qInd) {

		int front = bfsQueue[qInd];
		bNode_t* curr = brGraph->getNode(front);
		assert(curr);

//		cout << "F: " << front << endl;
		for (int eInd = 0; (uint) eInd < curr->outNodes.size(); ++eInd) {
			int node_ = curr->outNodes[eInd];
			
			retStr ret;
			char ch[2];
			ch[0] = eInd + 48; ch[1] = '\0';
			string tmpLabel = label[front] + string(ch);
			ret = label.insert(make_pair(node_, tmpLabel));

			int tmpLevel = level[front] + 1;
			if (ret.second) {
				level.insert(make_pair(node_, tmpLevel));
			}
//			if (node_ == target) {
//				path = tmpLabel;
//				return tmpLevel;
//			} 

			int edge_ = curr->outEdges[eInd];
			if (edge_ == target) {
				path = tmpLabel;
				return tmpLevel;
			}

			if (max_level < tmpLevel)
				max_level = tmpLevel;

			if (ret.second) {
				bfsQueue.push_back(node_);
			}

//			cout << "Queued " << node_ 
//				 << " Level: " << tmpLevel
//				 << " Label: " << tmpLabel
//				 << " Max_Lvl: " << max_level << endl;
		}

//		cout << "Queue (" << bfsQueue.size() << ")" << endl;
//		printVec(bfsQueue);
//		cout << endl;

		if ((max_level > THRESH_LEVEL) || (qInd > MAX_QUEUE_SIZE)) {
			cout << "Lvl: " << max_level << "/" << THRESH_LEVEL << "\t"
				 << "Q: " << qInd << "/" << MAX_QUEUE_SIZE << endl;
			path = "Unreachable";
			return -1;
		}

	}
	
	path = "Unreachable";
	return -1;
}

bool readTopNodes(covGraph_t& graphCov, brGraph_t& branchGraph) {

	int_vec leafNodes;
	int_vec topNodes;
	for (int i = 1; (unsigned) i <= graphCov.numNodes; ++i) {
		if ((graphCov.gNodes[i]).branch_index == -1)
			continue;

		int branch_ = (graphCov.gNodes[i]).branch_index;
		int parent_ = graphCov.getTopNode(branch_);

		if(IsDefaultBranch(branch_))
			continue;

		if (branch_ == parent_)
			topNodes.push_back(parent_);

		if (graphCov.gNodes[i].num_child == 0) {
			leafNodes.push_back(branch_);
			IsBranchLeaf[branch_] = 1;
		}
	}

	cout << "Leaf nodes : "; 
	printVec(leafNodes);
	cout << endl;

	cout << "Top nodes : "; 
	printVec(topNodes);
	cout << endl;
	
	for (int_vec_iter it = leafNodes.begin(); 
			it != leafNodes.end(); ++it)
		branchGraph.branchType[*it]++;

	for (int_vec_iter it = topNodes.begin(); 
			it != topNodes.end(); ++it)
		branchGraph.branchType[*it] += 2;

	char fName[100];
	sprintf(fName, "%s.bg", benchCkt);
	
	ifstream brFile;
	brFile.open(fName, ios::in);
	if (brFile == NULL) {
		cout << "Unable to open file " << fName << endl;
		return false;
	}
	
	for (int lNum = 0; brFile && (uint) lNum < topNodes.size(); ++lNum) {
		string tmpStr;
		getline(brFile, tmpStr);
//		cout << tmpStr << " l:" << tmpStr.length() << endl;

		stringstream ss;
		ss << tmpStr;

		int node_index;
		ss >> node_index;

		int num;
		ss >> num;

		int_vec edge_vec;
		while (ss) {
			edge_vec.push_back(num);

			int topNode = graphCov.getTopNode(num);

			branchGraph.addNode(node_index);
			branchGraph.addNode(topNode);
			branchGraph.addEdge(num, topNode, node_index);

//			cout << branchGraph.addNode(node_index) << " ";
//			cout << branchGraph.addNode(topNode) << " ";
//			cout << branchGraph.addEdge(num, topNode, node_index) << " ";
//			cout << topNode << "--" << num << "-->" << node_index << endl;

			ss >> num;
		}

//		cout << endl;
	}
	
	brFile.close();
	branchGraph.printGraph();

    return true;
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
			branch_counters[br] = 5;

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

	for (int round = 1; round < MAX_ROUNDS && !Rnd_terminate; ++round) {
		
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
		bool new_start_state = true;
		for (int st = 0; st <= indiv->max_index; ++st) {	
			state_t *curr = indiv->state_list[st];
			keyVal_t hash_val_ = curr->getHash();
			retVal_t ret = glStateMap.insert(make_pair(hash_val_, curr));

			if (ret.second == true) {
				new_start_state = true;
				indiv->state_list[st] = NULL;
			}
			else {
				state_t* tmp = ret.first->second;
				if (tmp)
					tmp->hit_count++;
				else
					glStateMap[ret.first->first] = curr;
			}
			for (int_vec_iter bt = curr->branch_index.begin();
					bt != curr->branch_index.end(); ++bt)
				curr_branch_cnt[*bt]++;

		}

		bool new_branch_ = false;
		for (int br = 0; br < CONST_NUM_BRANCH; ++br) {
			if ((curr_branch_cnt[br]) && (branch_counters[br] == 0))
				new_branch_ = true;
			branch_counters[br] += curr_branch_cnt[br];
		}

		Rnd_terminate = new_start_state || new_branch_ || (round < MAX_ROUNDS-1);

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

	paramObj->inputVec = (indiv->input_vec).substr(0,CONST_NUM_INPUT_BITS*(indiv->max_index+1));
	paramObj->branchHit = int_vec(CONST_NUM_BRANCH, 0);
	SimMultiCycle(cktVar,0);
	GetBranchCounters(cktVar, paramObj->branchHit);
	paramObj->stateList = state_pVec(indiv->max_index+1, NULL);
	for (int ind = 0; ind <= indiv->max_index; ++ind) {
		state_t *st = indiv->state_list[ind];
//		if (st == NULL) {
//			cout << "NULL state found at " << ind << endl;
//			assert(0);
//		}
		for (int_vec_iter it = st->branch_index.begin();
				it != st->branch_index.end(); ++it)
			paramObj->branchHit[*it]++;
		//paramObj->stateList[ind] = new state_t(*indiv->state_list[ind]);
		paramObj->stateList[ind] = indiv->state_list[ind];
		indiv->state_list[ind] = NULL;
	}

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

int AddVector2Branch(int branch_index, vecIn_t inp) {
	assert(inp.length() == CONST_NUM_INPUT_BITS);
	if (BranchInputVec[branch_index].compare("") == 0) {
		BranchInputVec[branch_index] = inp;
		return 0;
	}
	else {
		vecIn_t& brInp = BranchInputVec[branch_index];
		bool t = false;
		for (int ind = 0; ind < CONST_NUM_INPUT_BITS; ++ind) {
			if (brInp[ind] == 'X')
				continue;
			else if (brInp[ind] != inp[ind]) {
				brInp[ind] = 'X';
				t = true;
			}
		}
		if (t)
			return -1;
		else
			return 1;
	}
}

int AddState2Branch(int branch_index, state_t* st) {
	if (BranchStateVal[branch_index].compare("") == 0) {
		BranchStateVal[branch_index] = st->state_val;
		return 0;
	}
	else {
		string &brSt = BranchStateVal[branch_index];
		string inp = st->state_val;
		bool t = false;
		for (int ind = 0; ind < CONST_NUM_STATE_BITS; ++ind) {
			if (brSt[ind] == 'X')
				continue;
			else if (brSt[ind] != inp[ind]) {
				brSt[ind] = 'X';
				t = true;
			}
		}
		if (t)
			return -1;
		else
			return 1;
	}
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

void readParam(Stage2_Param* paramObj) {

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
		paramObj->MAX_ROUNDS = 100;
		paramObj->MAX_TRIES = 20;
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
			else if (curr.find("MAX_ROUNDS") != string::npos) {
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
					paramObj->MAX_ROUNDS = 20;
			}
			else if (curr.find("MAX_TRIES") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "MAX_TRIES = " << val << endl;
					paramObj->MAX_TRIES = val;
				}
				else 
					paramObj->MAX_TRIES = 20;
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
